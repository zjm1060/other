/********************************************************************************************************
FILENAME:	hdbwrite.c

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


struct chara_seg
{	
	int			id;
	char		LogiName[55];
	char		TimeTag[22];
	DATE_TIME	updatetime;
	float		offset;
	float		cur;
	float		max;
	float		min;
	unsigned short	max_no;
	unsigned short	min_no;
	unsigned short	max_change_no;
	unsigned short	min_change_no;
	float		avg;
	float		standard;
	float		max_change;
	float		min_change;
	char		ChgFlag;
	char		IsValid;
}chara[MAX_CHARA_NUM];


struct fdjh_seg
{
	char		TimeTag[22];
	short		num;
	float		value[3][MAX_CURVE_SEG_NUM];
}jh[MAX_JH_NUM];

struct sms_rec
{ 
	unsigned char		type_id;				
	char			message[SMS_MESSAGE_SIZE];
	char			TimeTag[22];				/* 报警时间 */
	short			ChgFlag;				/* 1表示该点需要写入报警记录表，0表示不写 */
} sms[MAX_SMS_NUM];

typedef struct _data_record_hdb
{
	char	user_name[20];
	int		user_id;
	int		host_id;
	int		groups_id;
	char	datetime[22];
	short	ms;
	UCHAR	type;
	char	content[100];
	UCHAR	station_id;	
	UCHAR	dev_id;	
	int		task_id;
	int		send_task_id;
	char	logicalname[55];
	UCHAR	type_id;
	short	status;
	float	fvalue;
} DATA_RECORD_HDB;
struct yc_seg yc_all[MAX_YC_NUM];
struct yx_seg yx_all_update[MAX_YX_NUM];
struct yx_seg yx_all_insert[MAX_YX_NUM];
struct yx_seg yx_fix[MAX_YX_NUM];

int	YcChg=1, YxChg=1, AlChg=1, SmsChg=1,CharaChg=1,YcStatusChg=0,TrendChg=1, StatChg=0;
int	YcNum,	YxNum, AlNum, SmsNum,CharaNum,TrendNum;
int InitHdb = 1;
int	alm_end=0;
int info_end=0;
int	oldday_chara,oldday,oldhour,oldminute;
int	HostOldStat, HostNewStat;
char LoginStr[50];
INFO_RECORD		*shm_info_rec_addr; 
int ReadDBChara(struct chara_seg *db);
int CvtAlToRec(	DATA_RECORD_HDB *p_rec,	struct al_rec al_data);
int CvtSmsToRec(DATA_RECORD_HDB *p_rec, struct sms_rec sms_data);
int CvtMessage(char *sms,char *pt_id,short status);
int GetStatId(int id);
int HdbConn_ctx = FALSE;
		

/********************************************************************************************************
 main function
*********************************************************************************************************/

void main(int argc, char *argv[])
{	
	int	i,n, acc_link=0, acc_yc=0;	
 	char hostname[20];
	DATE_TIME jh_time_cur,jh_time_old;
	DATE_TIME al_time_cur,al_time_old;
 	IND_DATA status;
	char log_str[256];
	wsa_startup();
	SetLogLevel(argc, argv);
#ifdef SUN
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
#endif
	gethostname(hostname,20); 
/*	app_id = HDB_APP;*/
  	ReadDefFile();
	sprintf(strPwd,"%s/%s@",strUsername,strPassword);
 	strcpy(LoginID,strPwd);
 	strcat(LoginID,SIDSTR);
	strcpy(LoginStr,LoginID);
	if(-1==InitSysLog(H9000_MODULE_HDBWRITE, "HdbWrite"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	if(MountDB(DB_NAME) == -1)
	{
		printf("==== hdbwrite.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	if(  InitStrTextDefine() ==-1)
	{
		printf("hdbwrite::: while ReadDefineFile Error!!!\n");
		exit(-1) ;
	}
	if(lan_init_all() == FALSE)
	{
		printf("==== hdbwrite.c: call Lan_init_all() error\n");		
		exit(0);
	}
	/* added by yyp 07.05.29 */
	if(strcmp(OUTNET_HOST,hostname)==0)
	{
		g_wranyhost = TRUE;
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

	if(get_rec_shm() == FALSE )
	{
		printf("\nhdbwrite.c: call get_rec_shm() error\n");		
		exit(0);
	}
	if (get_ots_shm() == FALSE)
	{
		printf("\nhdbwrite.c: call get_ots_shm() error\n");
		exit(0);
	}
	shm_info_rec_addr = (INFO_RECORD*) LinktoShareMem( SHM_INFO_RECORD_KEY_CHAR );
	if( shm_info_rec_addr == (INFO_RECORD*)-1 )
	{
		err_write("Fail to link shm_info_rec_addr shm");
		exit(0);
	}	
	//shm_hdb_addr->write_alm_flag=TRUE;
 	HostOldStat = CUR_WS_IN_VICE;
	al_time_old.day = -1;
	GetSysTime(&jh_time_cur);
	jh_time_old = jh_time_cur; 
	oldday_chara = jh_time_cur.day; 
	oldday = jh_time_cur.day;
	oldhour = jh_time_cur.hour;
	oldminute = jh_time_cur.minute;
	HdbConn=FALSE;
 	for(;;)
	{	
		MySleep(2000);

		HostNewStat=GetLocalStat( hostname );
		if(g_wranyhost == TRUE)
		{
			HostNewStat = CUR_WS_IN_HOST;
		}

		if( HostNewStat==CUR_WS_IN_HOST)
		{
			if( (++acc_link)>=100000|| HdbConn==FALSE )
			{	
				sprintf(log_str,"\n==== Connect '%s' acclink=%d, HdbConn =%d ==== \n",
					SIDSTR,acc_link,HdbConn);
				log_write(INF,1,log_str);
				if(HostOldStat!=CUR_WS_IN_VICE)
				{
					DisconnectHDB(&lda_rw,&cda_rw);
				}
				MySleep(2000);
				HdbConn=FALSE;
				if( ConnectHDB(&lda_rw,&cda_rw,LoginStr)!=TRUE )
				{ 
					sprintf(log_str,"\n==== hdbwrite Connect '%s' failed ==== \n",SIDSTR);
					log_write(ERR,1,log_str);
					BdHdbStat(HDB_ERR);
					MySleep(60000); 
					continue; 
				}
				else
				{
					sprintf(log_str,"\n==== hdbwrite Connect '%s' Success ==== \n",SIDSTR);
					log_write(INF,1,log_str);					
					BdHdbStat(HDB_ON);
					HdbConn=TRUE;
				}
				acc_link=0;
#ifdef SUN
				signal(SIGINT,sig_termp);
#endif
			}
			if( HdbConn_ctx==FALSE )
			{	
				DisconnectHDB_B(&ctx);
				MySleep(2000);
				HdbConn_ctx=FALSE;
				for(;;)
				{
					if( ConnectHDB_B(&ctx,SIDSTR,strUsername,strPassword)==FALSE )
					{	
						sprintf(log_str,"\n==== Connect ctx '%s' failed ==== \n",SIDSTR);
						err_write(log_str); 
						HdbConn_ctx = FALSE;
						MySleep(10000);
						continue; 
					}
					else
					{	
						sprintf(log_str,"\n==== Connect ctx '%s' Success  ====\n",SIDSTR); 
						log_write(INF,1,log_str);
						HdbConn_ctx = TRUE;
						break;
					}
				}	

#ifdef SUN
				signal(SIGINT,sig_termp);
#endif
				acc_link=0;
			}

		}
		if( HostNewStat==CUR_WS_IN_HOST && HostOldStat==CUR_WS_IN_VICE || 
			oldhour!=jh_time_cur.hour || shm_hdb_addr->readpt_real == 1)
		{
			shm_hdb_addr->readpt_real = 0;
			GenInfoTable();
			if(SelectIdFromHdb(cda_rw)== FALSE)
			{
				err_write("==== Hdbwrite: SelectIdFromHdb() failed!");
				exit(0);
			}
			else
			{

			}

			if(SelectStatPtFromHdb(cda_rw)== FALSE)
			{
				err_write("==== Hdbwrite: GetStatPtFromHdb() failed!");
				exit(0);
			}
			else
			{
				sprintf(log_str,"==== StaNum=%d ",StatNum);		
				log_write(INF,1,log_str);
			}
			for(i=0;i<YcNum;i++)
			{
				yc[i].id = -1;
				if(GetHdbIDByName((unsigned int *)&(yc[i].id),yc[i].LogiName)==FALSE)
				{
					sprintf(log_str,"\nGetHdbIDByName( %s ) error",yc[i].LogiName);
					//err_write(log_str);
					continue;
				}
				if(g_wroffset == TRUE)
				{
					if(GetPtHdbOffsetByName( yc[i].LogiName,&(yc[i].offset) ) ==FALSE)
					{
						continue;
					}
				}
				else
				{
					yc[i].offset = 0;
				}

			}

			for(i=0;i<CharaNum;i++)
			{
				chara[i].id = -1;
				if(GetHdbIDByName((unsigned int *)&(chara[i].id),chara[i].LogiName)==FALSE)
				{
					sprintf(log_str,"\nGetHdbIDByName( %s ) error",chara[i].LogiName);
					//err_write(log_str);
					continue;
				}
				if(GetPtHdbOffsetByName( chara[i].LogiName,&(chara[i].offset) ) ==FALSE)
				{
					continue;
				}
			}
			for(i=0;i<YxNum;i++)
			{
				yx[i].id = -1;
				if(GetHdbIDByName((unsigned int *)&(yx[i].id),yx[i].LogiName)==FALSE)
				{
					sprintf(log_str,"\nGetHdbIDByName( %s ) error",yx[i].LogiName);
					//err_write(log_str);
					continue;
				}
			}
			oldhour=jh_time_cur.hour;

#ifdef SUN
			signal(SIGINT,sig_termp);
#endif
			if( InitHdb == 1 )
			{
				if(g_updatefirst != 1 )
				{
					YcChg=0;
					YxChg=0;
				}
				for(i=0;i<YxNum;i++)
				{
					if( ( n = GetStatId(yx[i].id) ) != -1 ) 
					{
						memcpy(&status,&yx[i].status,2);					
						yxstat[n].oldstatus = status.state;
						yxstat[n].curstatus = yxstat[n].oldstatus;
						strcpy(yxstat[n].lasttime,yx[i].TimeTag);
						strcpy(yxstat[n].starttime,yx[i].TimeTag);
						yxstat[n].ischg = 1;
						StatChg = 1;
					}
				}
				if (shm_ots_addr->hdb_write_flag == 1)
				{
					InsertYxInHDB();
					UpdateYcInHDB();
					UpdateYxInHDB();
					UpdateStatInHDB();
				}
				InitHdb = 0;
			}
			printf("\n==== hdbwrite.dxe: start update HDB... \n");							
			HostOldStat = HostNewStat;
			alm_end = shm_rec_addr->head.header_ptr; 
			info_end = shm_info_rec_addr->head.header_ptr; 			
		}
		else if( HostNewStat==CUR_WS_IN_VICE && HostOldStat==CUR_WS_IN_HOST )
		{
			DisconnectHDB(&lda_rw,&cda_rw);
			printf("\n==== hdbscan.dxe: stop update HDB... \n");
			HostOldStat = HostNewStat;
			for(i=0;i<YcNum;i++) yc[i].ChgFlag= 0 ;
			YcChg=0;
			for(i=0;i<YxNum;i++) yx[i].ChgFlag= 0 ;	
			YxChg=0;
			for(i=0;i<AlNum;i++) al[i].ChgFlag= 0 ;			
			AlChg=0;
			continue;	
		}
		else
		{
			if(HostNewStat!=CUR_WS_IN_HOST)
			{
   
				MySleep(10000);
			}
		}
		if( shm_hdb_addr->data_revert.queryflag !=0 && shm_hdb_addr->data_revert.queryflag !=2)
		{
			continue;
		}
		if( HostNewStat==CUR_WS_IN_HOST  )
		{	
			GetSysTime(&jh_time_cur);

			GetYxData( HostNewStat );
			if (shm_ots_addr->hdb_write_flag == 1)
				InsertYxInHDB();
			if (shm_ots_addr->hdb_write_flag == 1)
				UpdateYxInHDB(); 
			if (shm_ots_addr->hdb_write_flag == 1)
				UpdateStatInHDB();

			GetYcData( HostNewStat );
			if (shm_ots_addr->hdb_write_flag == 1)
				UpdateYcInHDB();
			/*
			GetCharaData( HostNewStat );
			if (shm_ots_addr->hdb_write_flag == 1)
				UpdateCharaInHDB();
			*/
			GetAlData( HostNewStat );
			
			UpdateAlInHDB();
			if (shm_ots_addr->hdb_write_flag == 1)
				UpdateObjAlInHDB();

			GetSysTime(&al_time_cur);
			if(al_time_cur.day != al_time_old.day  && al_time_cur.minute>15 && al_time_cur.day==1) 
			{
				al_time_old = al_time_cur;
				if (shm_ots_addr->hdb_write_flag == 1)
					UpdateAlFixInHdb();
			}
			/* removed by yyp 10.08.04
			if( shm_hdb_addr->write_alm_flag==TRUE )	
			{
				UpdateAlFixInHdb();
				shm_hdb_addr->write_alm_flag=FALSE;
			}
			*/			
			GetSysTime(&jh_time_cur);
			/* removed by yyp 08.01.24 because of abnormal exit 
			if(jh_time_old.hour != jh_time_cur.hour &&  jh_time_cur.minute>5 )
			{
				GetJhData( HostNewStat );
				UpdateJhInHDB(); 				
				jh_time_old = jh_time_cur;
			}
			*/
		}
		else
		{
			continue;
		}
	}
}

/********************************************************************************************************
 Update Yc changed point to HDB
*********************************************************************************************************/
void UpdateYcInHDB()
{	
	time_t now;
	struct tm *datetime, *datetime2;
	int	i,	n=0;
	char	TimeStr[30];
	char log_str[256];
	time(&now);
	datetime=localtime(&now);
	if( YcChg==1 )
	{		
		printf("==== Start UpdateYcInHDB()... \n");
		tmtostring(*datetime,TimeStr);
		sprintf(log_str,"==== UpdateYcInHDB:starttime: %s",TimeStr);
		log_write(INF,1,log_str);	
		for(i=0;i<YcNum;i++)
		{	
			if( yc[i].ChgFlag==1 ) 
			{	
				yc_all[n].id = yc[i].id;
				strcpy(yc_all[n].TimeTag,yc[i].TimeTag);
				yc_all[n].value = yc[i].value;
				yc_all[n].status = yc[i].status;
				n++;
				/*
				if(UpdateHdbByYc( cda_rw, yc[i].id, yc[i].TimeTag,
					 &(yc[i].value), yc[i].status ) !=TRUE)
				{
					HdbConn=FALSE; 	
					return;
				}
				*/
				yc[i].ChgFlag = 0;
				//MySleep(2);	
			}
			if( yc[i].StatusChgFlag==1 ) 
			{	
				if(InsertHdbByYcStatus( cda_rw, yc[i].id, yc[i].TimeTag, yc[i].status ) !=TRUE)
				{
					HdbConn=FALSE; 	
					return;
				}
				yc[i].StatusChgFlag = 0;
				//MySleep(2);	
			}		
		}
		if(n >= 0 )
		{
			if( UpdateHdbByYc_A(&ctx, yc_all, n) != TRUE)
			{
				HdbConn_ctx=FALSE; 	
				return;	
			}
		}
		time(&now);
		datetime2=localtime(&now);
		tmtostring(*datetime2,TimeStr);
		sprintf(log_str,"==== UpdateYcInHDB: endtime:  %s",TimeStr);		
		log_write(INF,1,log_str);
		printf("==== End UpdateYcInHDB(), ChgYcNum=%d\n",n);
		YcChg=0;
	} 	
}

/********************************************************************************************************
 Update Yx changed point to HDB
*********************************************************************************************************/

void UpdateYxInHDB()
{ 	
	time_t now;
	struct tm *datetime, *datetime2;	
	int	i,	n=0;
	char	TimeStr[30];
	unsigned short state;
	char log_str[256];
	time(&now);
	datetime=localtime(&now);	
	if( YxChg==1 )
	{		
		printf("==== Start UpdateYxInHDB()... \n");
		tmtostring(*datetime,TimeStr);
		sprintf(log_str,"==== UpdateYxInHDB: starttime: %s",TimeStr);
		log_write(INF,1,log_str);	
		for(i=0;i<YxNum;i++)
		{	
			if( yx[i].ChgFlag==1 )
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
				/*
				if(UpdateHdbByYx( cda_rw, yx[i].id, 
					yx[i].TimeTag, state, yx[i].quality ) != TRUE)
				{
					HdbConn=FALSE;
					return;
				}
				*/
				yx_all_update[n].id = yx[i].id;
				strcpy(yx_all_update[n].TimeTag,yx[i].TimeTag);
				yx_all_update[n].fvalue = (float) state;
				yx_all_update[n].quality = yx[i].quality;
				n++;
				//MySleep(2);
				//yx[i].ChgFlag = 0;
			}	
		}

		if(n >= 0 )
		{
			if( UpdateHdbByYx_A(&ctx, yx_all_update, n) != TRUE)
			{
				HdbConn_ctx=FALSE; 	
				return;	
			}
		}

		time(&now);
		datetime2=localtime(&now);
		tmtostring(*datetime2,TimeStr);
		sprintf(log_str,"==== UpdateYxInHDB: endtime:  %s",TimeStr);		
		log_write(INF,1,log_str);
		sprintf(log_str,"==== End UpdateYxInHDB(), ChgYxNum=%d \n",n);
		log_write(INF,1,log_str);
		//YxChg=0;
	}
}


/********************************************************************************************************
 Update Chara changed point to HDB
*********************************************************************************************************/
void UpdateCharaInHDB()
{	
	time_t now;
	struct tm *datetime, *datetime2;
	int	i,	n=0;
	char TimeStr[30];
	char log_str[256];
	time(&now);
	datetime=localtime(&now);
	if( CharaChg==1 )
	{		
		printf("==== Start UpdateCharaInHDB()... \n");
		tmtostring(*datetime,TimeStr);
		sprintf(log_str,"==== UpdateCharaInHDB:starttime: %s",TimeStr);
		log_write(INF,1,log_str);	
		for(i=0;i<CharaNum;i++)
		{	
			if( chara[i].ChgFlag==1 ) 
			{
				n++;
				if(UpdateHdbByChara( cda_rw, chara[i].id, chara[i].TimeTag,
					chara[i].max , chara[i].min, chara[i].avg,chara[i].max_change,
					chara[i].min_change,chara[i].standard,chara[i].max_no,
					chara[i].min_no,chara[i].max_change_no,chara[i].min_change_no) !=TRUE)
				{
					HdbConn=FALSE; 	
					return;
				}
				chara[i].ChgFlag = 0;
				MySleep(2);	
			}
		
		}
		time(&now);
		datetime2=localtime(&now);
		tmtostring(*datetime2,TimeStr);
		sprintf(log_str,"==== UpdateCharaInHDB: endtime:  %s, ChgCharaNum=%d\n",TimeStr,n);		
		log_write(INF,1,log_str);
		CharaChg=0;
	}
}

/********************************************************************************************************
 Update Statics changed point to HDB
*********************************************************************************************************/
void UpdateStatInHDB()
{	
	time_t now;
	struct tm *datetime, *datetime2;
	int	i,	n=0;
	char TimeStr[30];
	int status_id;
	char log_str[256];
	time(&now);
	datetime=localtime(&now);
	if( StatChg==1 )
	{		
		printf("==== Start UpdateStatInHDB()... \n");
		tmtostring(*datetime,TimeStr);
		sprintf(log_str,"==== UpdateStatInHDB:starttime: %s",TimeStr);
		log_write(INF,1,log_str);	
		for(i=0;i<StatNum;i++)
		{	
			if( yxstat[i].ischg==1 ) 
			{
				status_id = yxstat[i].curstatus;
				if(yxstat[i].isstat[status_id] != 1 )
				{
					continue;
				}
				n++;
				if(UpdateHdbByStat( cda_rw, &(yxstat[i])) !=TRUE)
				{
					yxstat[i].ischg = 0;
					HdbConn=FALSE; 	
					return;
				}
				yxstat[i].ischg = 0;
				MySleep(2);	
			}
		
		}
		time(&now);
		datetime2=localtime(&now);
		tmtostring(*datetime2,TimeStr);
		sprintf(log_str,"==== UpdateStatInHDB: endtime:  %s, num=%d",TimeStr,n);		
		log_write(INF,1,log_str);
		StatChg=0;
	} 	
}

/********************************************************************************************************
 Update Jh changed point to HDB
*********************************************************************************************************/

void UpdateJhInHDB()
{ 	
	time_t now;
	struct tm *datetime, *datetime2;	
	int	i,	j=0;
	char	TimeStr[30];
	char log_str[256];
	time(&now);
	datetime=localtime(&now);	
	printf("==== Start UpdateJhInHDB()... \n");
	tmtostring(*datetime,TimeStr);
	sprintf(log_str,"==== UpdateJhInHDB: starttime: %s",TimeStr);
	log_write(INF,1,log_str);

	for(i=0;i<MAX_JH_NUM;i++)
	{	
		if(InsertHdbByJh( cda_rw, i, jh[i].num) != TRUE)
		{
			HdbConn=FALSE;
			return;
		}
		for(j=0;j<jh[i].num;j++)
		{
			if(UpdateHdbByJh( cda_rw, i, j, jh[i].value[0][j], jh[i].value[1][j],
				jh[i].value[2][j]) != TRUE)
			{
				HdbConn=FALSE;
				return;
			}
		}
	}
	time(&now);
	datetime2=localtime(&now);
	tmtostring(*datetime2,TimeStr);
	sprintf(log_str,"==== UpdateJhInHDB: endtime:  %s",TimeStr);		
	log_write(INF,1,log_str);;			
	printf("==== End UpdateJhInHDB()\n");
}


/********************************************************************************************************
 Update Al Info to HDB
*********************************************************************************************************/
void UpdateAlInHDB()
{ 	
	int	i,	n=0;
	int id;
	char log_str[256];
	DATA_RECORD_HDB seg;
	if( AlChg==1 )
	{		
		sprintf(log_str,"==== Start UpdateAlInHDB(),AlNum=%d... \n",AlNum);
		log_write(INF,1,log_str);		
		for(i=0;i<AlNum;i++)
		{	
			if( al[i].ChgFlag==1 )
			{	
				n++;
				id = -1;
				if(GetHdbIDByName((unsigned int * )&id,al[i].l_name)==FALSE)
				{
					sprintf(log_str,"\nGetHdbIDByName( %s ) error",al[i].l_name);
					//err_write(log_str);
					continue;
				}
				if (shm_ots_addr->hdb_write_flag == 1)
				{
					if(InsertHdbAlarm( cda_rw, al[i].TimeTag, id, al[i].status, 
						al[i].msecond, al[i].fvalue, al[i].type_id) != TRUE)
					{
						HdbConn=FALSE;
						return;
					}
					CvtAlToRec(	&seg,al[i]);			
					if(InsertHdbRecord( cda_rw, seg.user_id,seg.host_id,seg.groups_id,seg.datetime,seg.ms, 
						seg.type, seg.content, seg.station_id,seg.dev_id,seg.task_id,seg.send_task_id,
						seg.logicalname,seg.type_id,seg.status,seg.fvalue) != TRUE)
					{
						HdbConn=FALSE;
						return;
					}
				}
				else if (shm_ots_addr->login_status == TRUE && (shm_ots_addr->groups_id == -1 || shm_ots_addr->groups_role == 1))
				{
					CvtAlToRec(	&seg,al[i]);			
					if(InsertHdbRecord( cda_rw, seg.user_id,seg.host_id,seg.groups_id,seg.datetime,seg.ms, 
						seg.type, seg.content, seg.station_id,seg.dev_id,seg.task_id,seg.send_task_id,
						seg.logicalname,seg.type_id,seg.status,seg.fvalue) != TRUE)
					{
						HdbConn=FALSE;
						return;
					}
				}
			
				MySleep(2);
				al[i].ChgFlag = 0;
			}
		}
		sprintf(log_str,"==== End UpdateAlInHDB(), ChgAlNum=%d\n",n);
		log_write(INF,1,log_str);		
		AlChg=0;
	}
	n=0;
	if(SmsChg==1)
	{
		sprintf(log_str,"==== Start UpdateSmsInHDB(),SmsNum=%d... \n",SmsNum);
		log_write(INF,1,log_str);
		for(i=0;i<SmsNum;i++)
		{	
			if( sms[i].ChgFlag==1 )
			{	
				n++;
				if (shm_ots_addr->hdb_write_flag == 1)
				{
					if(InsertHdbSmsAlarm( cda_rw, sms[i].TimeTag, sms[i].message,
						sms[i].type_id) != TRUE)
					{
						HdbConn=FALSE;
						return;
					}
					CvtSmsToRec( &seg,sms[i]);
					if(InsertHdbRecord( cda_rw, seg.user_id,seg.host_id,seg.groups_id,seg.datetime,seg.ms, 
						seg.type, seg.content, seg.station_id,seg.dev_id,seg.task_id,seg.send_task_id,
						seg.logicalname,seg.type_id,seg.status,seg.fvalue) != TRUE)
					{
						HdbConn=FALSE;
						return;
					}
				}
			
				else if (shm_ots_addr->login_status == TRUE && (shm_ots_addr->groups_id == -1 || shm_ots_addr->groups_role == 1))
				{
					CvtSmsToRec( &seg,sms[i]);
					if(InsertHdbRecord( cda_rw, seg.user_id,seg.host_id,seg.groups_id,seg.datetime,seg.ms, 
						seg.type, seg.content, seg.station_id,seg.dev_id,seg.task_id,seg.send_task_id,
						seg.logicalname,seg.type_id,seg.status,seg.fvalue) != TRUE)
					{
						HdbConn=FALSE;
						return;
					}
				}
				sms[i].ChgFlag = 0;	
			}
		}
		sprintf(log_str,"==== End UpdateSmsInHDB(), ChgSmsNum=%d\n",n);
		log_write(INF,1,log_str);
		SmsChg=0;		
	}
}

/********************************************************************************************************
 Update OBJ changed point to HDB 
 added by yyp 07.07.03
*********************************************************************************************************/

void UpdateObjAlInHDB()
{ 	
	time_t now;
	struct tm *datetime, *datetime2;	
	int	i,	n=0;
	char	TimeStr[30];
	short status;
	int id;
	char log_str[256];
	time(&now);
	datetime=localtime(&now);	
	if( YxChg==1 )
	{		
		printf("==== Start UpdateObjAlInHDB()... \n");
		tmtostring(*datetime,TimeStr);
		sprintf(log_str,"==== UpdateObjAlInHDB: starttime: %s",TimeStr);
		log_write(INF,1,log_str);	
		for(i=0;i<YxNum;i++)
		{
			if( yx[i].ChgFlag==1 && yx[i].data_type == DEVOBJ_TYPE )
			{	
				n++;
				id = -1;
				if(GetHdbIDByName((unsigned int *)&id,yx[i].LogiName)==FALSE)
				{
					sprintf(log_str,"\nGetHdbIDByName( %s ) error",yx[i].LogiName);
					//err_write(log_str);
					continue;
				}
				memcpy(&status,&(yx[i].status),2);
				if( InsertHdbAlarm( cda_rw, yx[i].TimeTag,id, 
					status, yx[i].ms, -1, DPS_DEV_FB) != TRUE )
				{
					HdbConn=FALSE;
					return;
				}
				MySleep(2);
			}
		}
		time(&now);
		datetime2=localtime(&now);
		tmtostring(*datetime2,TimeStr);
		sprintf(log_str,"==== UpdateObjAlInHDB: endtime:  %s",TimeStr);		
		log_write(INF,1,log_str);			
		sprintf(log_str,"==== End UpdateObjAlInHDB(), ChgObjNum=%d \n",n);
		log_write(INF,1,log_str);	
	}
}


/********************************************************************************************************
 Update All yx In almcmd to HDB once a day
*********************************************************************************************************/
void UpdateAlFixInHdb()
{ 	
	int	i,	n=0;
	int id;
	short status;
	char log_str[256];
	sprintf(log_str,"==== Start UpdateAlFixInHdb(),AlNum=%d... \n",YxNum);
	log_write(INF,1,log_str);		
	for(i=0;i<YxNum;i++)
	{	
		if(yx[i].IsValid == TRUE)
		{
			memcpy(&status,&(yx[i].status),2);
			id = -1;
			if(GetHdbIDByName((unsigned int *)&id,yx[i].LogiName)==FALSE)
			{
				sprintf(log_str,"\nGetHdbIDByName( %s ) error",yx[i].LogiName);
			//	err_write(log_str);
				//MySleep(100);
				continue;
			}
			//modify by dm 2010.10.28
			/*if(InsertHdbAlarm( cda_rw, yx[i].ReadTime, id, 
			status, yx[i].ms, -1, DPS_IND_ALM) != TRUE)
			{
				HdbConn=FALSE;
				return;
			}
			MySleep(1);
			n++;
			if(n%1000 == 0)
			{
				MySleep(2000);
			}*/
			
			yx_fix[n].id = id;
			strcpy(yx_fix[n].ReadTime,yx[i].ReadTime);
			yx_fix[n].status_id = (int )status;
			yx_fix[n].ms = yx[i].ms;
			n++;
			}
		}			
	if(n > 0 )
	{
		if( InsertHdbAlarm_A(&ctx, yx_fix, n) != TRUE)
		{
			HdbConn_ctx=FALSE; 	
			return;	
		}
	}
	sprintf(log_str,"==== End UpdateAlFixInHdb(), ChgAlNum=%d\n",n);
	log_write(INF,1,log_str);		
}

/********************************************************************************************************
 Insert Yx changed point to HDB add by dm 2010.10.27
*********************************************************************************************************/

void InsertYxInHDB()
{
	time_t now;
	struct tm *datetime, *datetime2;	
	int	i,	n=0;
	char	TimeStr[30];
	unsigned short state;
	char log_str[256];
	time(&now);
	datetime=localtime(&now);	
	if( YxChg==1 )
	{
		printf("==== Start InsertYxInHDB()... \n");
		tmtostring(*datetime,TimeStr);
		sprintf(log_str,"==== InsertYxInHDB: starttime: %s",TimeStr);
		log_write(INF,1,log_str);	
		for(i=0;i<YxNum;i++)
		{	
			if( yx[i].ChgFlag==1 )
			{	
				/*
				if(yx[i].data_type != DEVOBJ_TYPE)
				{
					state = yx[i].status.state;
				}
				else
				{
					GetObjPtStateByNameStr( yx[i].LogiName, &state );					
				}
				*/
				memcpy(&state,&(yx[i].status),2);
				yx_all_insert[n].id = yx[i].id;
				//strcpy(yx_all_insert[n].TimeTag,yx[i].TimeTag);
				strcpy(yx_all_insert[n].TimeTag,TimeStr);
				yx_all_insert[n].status_id = (int) state;
				yx_all_insert[n].quality = yx[i].quality;
				yx_all_insert[n].ms = yx[i].ms;
				n++;
				//MySleep(2);
				yx[i].ChgFlag = 0;
			}	
		}
		if(n > 0 )
		{
			if( InsertHdbByYx_A(&ctx, yx_all_insert, n) != TRUE)
			{
				HdbConn_ctx=FALSE; 	
				return;	
			}
		}
		time(&now);
		datetime2=localtime(&now);
		tmtostring(*datetime2,TimeStr);
		sprintf(log_str,"==== InsertYxInHDB: endtime:  %s",TimeStr);		
		log_write(INF,1,log_str);
		sprintf(log_str,"==== End InsertYxInHDB(), ChgYxNum=%d \n",n);
		log_write(INF,1,log_str);
		YxChg=0;
	}
}
/********************************************************************************************************
 Get Chara Changed Point data from real db
*********************************************************************************************************/

void GetCharaData(int HostStat)
{ 
	int		i;
	char log_str[256];
	time_t now;
	struct tm *datetime;
	sprintf(log_str,"\n==== start GetCharaData() CharaNum=%d\n",CharaNum);
	log_write(INF,1,log_str);

	time(&now);
	datetime=localtime(&now);	
	for(i=0;i<CharaNum;i++)
	{
		chara[i].ChgFlag	=	0;
		if(ReadDBChara( &(chara[i]) )==-1)
		{	
			sprintf(log_str,"%s invalid!",chara[i].LogiName);
			err_write(log_str);
		}

	}	
}

/********************************************************************************************************
 Get Yc Changed Point data from real db
*********************************************************************************************************/

void GetYcData(int HostStat)
{ 
	int		i;
  	DB_DATA		db_data;
  	float		off_val;
	time_t now;
	char log_str[256];
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
		// modified by yyp 10.08.10 if( fabs(off_val)>yc[i].offset || db_data.status!=yc[i].status )
		 if( fabs(off_val)>0.001 || db_data.status!=yc[i].status )
		{	
			sprintf(log_str,"%s data changed!",yc[i].LogiName);
			log_write(INF,3, log_str);
			YcChg 		= 1;
			if(db_data.year<=1) 
			{
				db_data.year = datetime->tm_year+1900;
			}
			sprintf(yc[i].TimeTag,"%04d-%02d-%02d %02d:%02d:%02d",
				db_data.year,db_data.month,db_data.day,
				db_data.hour,db_data.minute,db_data.second );
			if(db_data.status!=yc[i].status )
			{
				YcStatusChg 		= 1;
				yc[i].StatusChgFlag	=	1;
			}
			yc[i].value	=	db_data.fvalue;
			yc[i].status	=	db_data.status ;		
			yc[i].ChgFlag	=	1;
		}

	}	
}


/********************************************************************************************************
 Get Yx Changed Point data from real db
*********************************************************************************************************/

void GetYxData(int HostStat)
{ 
	int		i,n;
  	DB_DATA	db_data;
  	IND_DATA status;
  	unsigned short state1,state2;
	time_t now;
	struct tm *datetime;	
	char TimeStr[22];
	char log_str[256];
	time(&now);
	datetime=localtime(&now);	
	tmtostring(*datetime,TimeStr);	
	printf("\n==== start GetYxData() YxNum=%d\n",YxNum);
	YxChg=0;
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
						StatChg = 1;
					}
				}
			}
			yx[i].status    = status;
			yx[i].quality    = (unsigned char)db_data.quality;			
			yx[i].ms	= db_data.ms;

			YxChg 		= 1;
			yx[i].ChgFlag	= 1;

		}
	}
}

/********************************************************************************************************
 Get fdjh Changed Point data from real db
*********************************************************************************************************/

void GetJhData(int HostStat)
{ 
	int		i,j;
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	char	logicalname[MAX_JH_NUM][3][38];
	DMS_COMMON      rs;
	stnid =1;
	devid = g_sys_lcu[stnid];
 	if(GetStnNameById(stnid,tmp) == -1) return;
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return;
	strcat(buf,tmp);
	sprintf(logicalname[0][0],"%s.%s",buf,"CUR.JC0_JRFHQX_SD");
	sprintf(logicalname[0][1],"%s.%s",buf,"CUR.JC0_JRFHQX_XZ");
	sprintf(logicalname[0][2],"%s.%s",buf,"CUR.JC0_JRDYQX");
	sprintf(logicalname[1][0],"%s.%s",buf,"CUR.JC0_MRFHQX_SD");
	sprintf(logicalname[2][1],"%s.%s",buf,"CUR.JC0_MRFHQX_XZ");
	sprintf(logicalname[3][2],"%s.%s",buf,"CUR.JC0_MRDYQX");
	printf("\n==== start GetJhData() \n");
	for(i=0;i<MAX_JH_NUM;i++)
	{	
		for(j=0;j<3;j++)
		{
			if(ReadEntryByNameStr( logicalname[i][j], &rs) ==-1) return ;
			if(i==0)
			{
				jh[i].num = rs.var.curve.seg_num;
				sprintf (jh[i].TimeTag, "%4d-%02d-%02d %02d:%02d:%02d",
				rs.var.curve.update_time.year,
				rs.var.curve.update_time.month,
				rs.var.curve.update_time.day,
				rs.var.curve.update_time.hour,
				rs.var.curve.update_time.minute,
				rs.var.curve.update_time.second );
			}
 			memcpy(jh[i].value[j], &rs.var.curve.data,4 * MAX_CURVE_SEG_NUM);
		}
	}	
}


/********************************************************************************************************
 get alarm record from alarm shared memory added by yyp 05.12.14
*********************************************************************************************************/

void GetAlData(int HostStat)
{ 
	ALM_STR			alarm_str ;
	BOOL			blink ;
	COLOR_PAIR		color ;
	POINT_NAME pt_name;	
	time_t tmptime;
	struct tm tm_time;
	int i,j,k;
	int ID=0;								
	long LastID=-1;							
	int alm_head;
	int info_head;
	int	head,end;
	REC_ENTRY_UNION		rec;
	printf("\n==== Start to get AL record! \n");
	j=1;
	for(i=0;i<MAX_AL_NUM;i++)
	{
		al[i].ChgFlag=0;
	}
	for(i=0;i<MAX_SMS_NUM;i++)
	{
		sms[i].ChgFlag=0;
	}	
	i=0;
	j=0;
	alm_head = shm_rec_addr->head.header_ptr;
	info_head = shm_info_rec_addr->head.header_ptr;
	for(k=0;k<2;k++)
	{
		if(k==0)
		{
			head = alm_head;
			end = alm_end;
		}
		else
		{
			head = info_head;
			end = info_end;
		}

		while( end != head )

		{

			if(k==0)
			{
				rec = shm_rec_addr->rec[alm_end];
			}
			else
			{
				rec = shm_info_rec_addr->rec[info_end];
			}
			GetAlmStrings(&rec, &alarm_str , &blink , &color );
			al[i].type_id = rec.ana_rec.type_id;

			sprintf(al[i].alarm_str,"%s	%s	%s",alarm_str.longname,alarm_str.state_str,alarm_str.cmd_result);
			
			al[i].type_id = rec.ana_rec.type_id;
			al[i].al_type = 1;
			switch(rec.ana_rec.type_id)
			{
			case	DPS_ANA_ALM:
			case	DPS_IMP_ALM:
				al[i].pointer.stn_id= rec.ana_rec.stn_id;
				al[i].pointer.dev_id= rec.ana_rec.dev_id;
				al[i].pointer.data_type= rec.ana_rec.data_type;
				al[i].pointer.pt_id= rec.ana_rec.pt_id;
				if(IsPtIntoHdb(&(al[i].pointer)) != TRUE) 
				{
					break;
				}
				memcpy(&(al[i].status),&(rec.ana_rec.status),2);
				sprintf (al[i].TimeTag, "%4d-%02d-%02d %02d:%02d:%02d",
					rec.ana_rec.year,
					rec.ana_rec.month,
					rec.ana_rec.day,
					rec.ana_rec.hour,
					rec.ana_rec.minute,
					rec.ana_rec.second ) ;
				al[i].fvalue=(float)rec.ana_rec.data;		/* modified by yyp 2008.12.22 */
				i++;
				break;

			case	DPS_IND_ALM:
				al[i].pointer.stn_id= rec.ind_rec.stn_id;
				al[i].pointer.dev_id= rec.ind_rec.dev_id;
				al[i].pointer.data_type= rec.ind_rec.data_type;
				al[i].pointer.pt_id= rec.ind_rec.pt_id;
				if(IsPtIntoHdb(&(al[i].pointer)) != TRUE) 
				{
					break;
				}
				memcpy(&(al[i].status),&(rec.ind_rec.ind_status),2);
				sprintf (al[i].TimeTag, "%4d-%02d-%02d %02d:%02d:%02d",
					rec.ind_rec.year,
					rec.ind_rec.month,
					rec.ind_rec.day,
					rec.ind_rec.hour,
					rec.ind_rec.minute,
					rec.ind_rec.second ) ;
				al[i].msecond=rec.ind_rec.ms;
				al[i].fvalue = 0;
				i++;				
				break;

			case	DPS_IOST_SET:
			case	DPS_IOATTR_SET:
			case	DPS_MAN_VALUE_SET:
			case	DPS_MAN_STAT_SET:
			case	DPS_MAN_CURVE_SET:
			case	DPS_ANA_PARAM_SET:
			case	DPS_IMP_PARAM_SET:
			case	DPS_C_ONOFF_SET:
				al[i].pointer.stn_id= rec.param_set_rec.stn_id;
				al[i].pointer.dev_id= rec.param_set_rec.dev_id;
				al[i].pointer.data_type= rec.param_set_rec.data_type;
				al[i].pointer.pt_id= rec.param_set_rec.pt_id;
				memcpy(&(al[i].status),&(rec.param_set_rec.cmd_status),2);
				al[i].fvalue= (float)rec.param_set_rec.data_new.iValue;
				sprintf (al[i].TimeTag, "%4d-%02d-%02d %02d:%02d:%02d",
					rec.param_set_rec.year,
					rec.param_set_rec.month,
					rec.param_set_rec.day,
					rec.param_set_rec.hour,
					rec.param_set_rec.minute,
					rec.param_set_rec.second);
				i++;
				al[i].al_type = 0;
				break;

			/* case	DPS_LMS: */
			case	DPS_SMS:
			case	DPS_SM_ALM:
				sms[j].type_id = rec.sms_rec.type_id;
				memcpy(&tmptime,&rec.sms_rec.time,4);

				tm_time=*localtime(&tmptime);

				strcpy(sms[j].message,rec.sms_rec.message);
				tmtostring(tm_time,sms[j].TimeTag);	
								
				SmsChg 	= 1;
				sms[j].ChgFlag=1;
				
				j++;
				
				break;
			
			default:
				break;
			}
			strcpy(al[i].user_name,"");
			if(rec.ana_rec.type_id >= TIME_SYNC)
			{		
				al[i].pointer.stn_id= rec.cmd_rec.stn_id;
				al[i].pointer.dev_id= rec.cmd_rec.dev_id;
				al[i].pointer.data_type= rec.cmd_rec.data_type;
				al[i].pointer.pt_id= rec.cmd_rec.pt_id;
				memcpy(&(al[i].status),&(rec.cmd_rec.cmd_status),2);
				al[i].fvalue = (float)rec.cmd_rec.data.iValue;
				sprintf (al[i].TimeTag, "%4d-%02d-%02d %02d:%02d:%02d",
					rec.cmd_rec.year,
					rec.cmd_rec.month,
					rec.cmd_rec.day,
					rec.cmd_rec.hour,
					rec.cmd_rec.minute,
					rec.cmd_rec.second ) ;
				al[i].al_type = 0;
				strcpy(al[i].user_name,alarm_str.usrname);
				i++;
			}
			
			if(k==0)
			{
				alm_end++;		
				if(alm_end >= MAX_SHM_RECORD_NUM)
				{
					alm_end = 0;
				}
				end = alm_end;

			}
			else
			{
				info_end++;		
				if(info_end >= MAX_INFO_RECORD_NUM)
				{
					info_end = 0;
				}
				end = info_end;
			}
			if(i >= MAX_AL_NUM)
				break;	
			if(j >= MAX_SMS_NUM)
				break;				
		}/* end while */
	}/* end for */	
	AlNum=i;
	SmsNum=j;
	for(i=0;i<AlNum;i++)
	{
		al[i].ChgFlag=0;
		if(GetPtNameById( &( al[i].pointer ), al[i].l_name, &pt_name ) ==-1)
		{
			continue;
		}
		AlChg 	= 1;
		al[i].ChgFlag=1;
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
	YcNum = YxNum = CharaNum =0;
	for(i=0;i<MAX_YC_NUM;i++) strcpy(yc[i].LogiName,"");
	for(i=0;i<MAX_YX_NUM;i++) strcpy(yx[i].LogiName,"");
	for(i=0;i<MAX_CHARA_NUM;i++) strcpy(chara[i].LogiName,"");		
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

	for(i=1;i<=g_stn_num;i++)
	{
		if( g_cascade != YES )
		{
			if(i>1) break;
		}	
		if( g_dev_num[i]!=0 )
		{
			printf("\n==== Chara Station %d (%d--%d) \n",i,1, g_dev_num[i]);
			
			if( CreatCharaInfo( (unsigned char)i )==-1 )
				break;
		}
	}
	
	printf("\n==== CharaNum=%d\n",CharaNum);	
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
 create Chara point def file
*********************************************************************************************************/

int CreatCharaInfo(unsigned char stnid )
{	
	int			DevId,	DataType,	PointId,	Number;
	char			cname[100],	lname[100];
	POINTER			point;
	POINT_NAME		pt_name;
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
			if( DataType==CHARA_TYPE)
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
					
					strcpy(chara[CharaNum].LogiName,lname);  
					if( ReadDBChara( &(chara[CharaNum]) )!=TRUE )
					{
						printf("No %s in RDB\n",lname);					
						continue;
					}
					
					if( (++CharaNum)<MAX_CHARA_NUM ) continue; else return(-1); 
				}
			}
		}
	}
	CharaChg=0;
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

int ReadDBChara(struct chara_seg *db)
{
	DMS_COMMON      rs;
	DATE_TIME	update_time;

	if(ReadEntryByNameStr( db->LogiName, &rs) ==-1) return FALSE;
	update_time = rs.var.chara.update_time;
  	sprintf(db->TimeTag,"%04d-%02d-%02d %02d:%02d:%02d",
  		update_time.year,update_time.month,update_time.day,
  		update_time.hour,update_time.minute,update_time.second );
	db->max = rs.var.chara.chara_data.max_value;
	db->min = rs.var.chara.chara_data.min_value;

	db->max_no  = rs.var.chara.chara_data.max_no;
	db->min_no = rs.var.chara.chara_data.min_no;
	db->max_change  = rs.var.chara.chara_data.max_rate;
	db->min_change  = rs.var.chara.chara_data.min_rate;
	db->max_change_no = rs.var.chara.chara_data.max_rate_no;
	db->min_change_no = rs.var.chara.chara_data.min_rate_no;
	db->standard = rs.var.chara.chara_data.std_value;
	if(db->updatetime.second != update_time.second || db->updatetime.minute != update_time.minute
		|| db->avg != rs.var.chara.chara_data.avg_value)
	{	
		db->avg = rs.var.chara.chara_data.avg_value;		
		db->updatetime = update_time;
		db->ChgFlag = TRUE;
		CharaChg=1;
	}
	return TRUE;
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
int CvtAlToRec(	DATA_RECORD_HDB *p_rec,	struct al_rec al_data)
{
	p_rec->user_id = shm_ots_addr->user_id;
	p_rec->groups_id = shm_ots_addr->groups_id;
	strcpy(p_rec->user_name,al_data.user_name);
	p_rec->host_id = shm_ots_addr->host_id;
	strcpy(p_rec->datetime,al_data.TimeTag);
	p_rec->ms = al_data.msecond;
	p_rec->type = al_data.al_type;
	p_rec->station_id = al_data.pointer.stn_id;
	p_rec->dev_id = al_data.pointer.dev_id;
	strcpy(p_rec->content,al_data.alarm_str);
	strcpy(p_rec->logicalname,al_data.l_name);
	p_rec->type_id = al_data.type_id;
	p_rec->status = al_data.status;
	p_rec->fvalue = al_data.fvalue;
	if (shm_ots_addr->data_task.task_status == 2)
	{
		p_rec->task_id = shm_ots_addr->data_task.task_id;
		p_rec->send_task_id = shm_ots_addr->data_task.send_task_id;
	}
	else
	{
		p_rec->task_id = 0;
		p_rec->send_task_id = 0;
	}
	//yyp debug
	//strcpy(p_rec->content,"have a test");
	return TRUE;
}
int CvtSmsToRec(DATA_RECORD_HDB *p_rec, struct sms_rec sms_data)
{
	short status;
	char pt_id[20];
	POINTER pt; 
	char content[100];
	char longName[70],stnComment[20],ptName[100];
	char message[SMS_MESSAGE_SIZE];
	p_rec->user_id = shm_ots_addr->user_id;
	p_rec->groups_id = shm_ots_addr->groups_id;
	strcpy(p_rec->user_name," ");
	p_rec->host_id = shm_ots_addr->host_id;	
	strcpy(p_rec->datetime,sms_data.TimeTag);
	p_rec->ms = 0;
	p_rec->type = 2;//短消息
	p_rec->station_id = 0;
	p_rec->dev_id = 0;
	strcpy(message,sms_data.message);
	if (CvtMessage(message,pt_id,&status) == TRUE)
	{
		GetPtIdFromStr(pt_id,&pt);
		if (GetStnCommentById(pt.stn_id,stnComment) != -1 && GetPtLongnameById(&pt,longName) != -1 && GetPtNameById(&pt,ptName,NULL) != -1)
		{		
			strcpy(content,stnComment);
			strcat(content,longName);
			if (status == 0)
			{
				strcat(content,"检查正常");
			}
			else if (status == 1)
			{
				strcat(content,"检查异常");
			}
			strcpy(p_rec->content,content);
			p_rec->type = 4;//检查操作
			strcpy(p_rec->logicalname,ptName);
		}
		p_rec->status = status;
	}
	else
	{
		strcpy(p_rec->content,sms_data.message);
		strcpy(p_rec->logicalname," ");
		p_rec->status = 0;
	}
	p_rec->type_id = sms_data.type_id;
	p_rec->fvalue = 0;
	if (shm_ots_addr->data_task.task_status == 2)
	{
		p_rec->task_id = shm_ots_addr->data_task.task_id;
		p_rec->send_task_id = shm_ots_addr->data_task.send_task_id;
	}
	else
	{
		p_rec->task_id = 0;
		p_rec->send_task_id = 0;
	}
	//yyp debug
	//strcpy(p_rec->content,"have a test");
	return TRUE;
}
int CvtMessage(char *sms,char *pt_id,short *status)
{
	char str[] = "pt_id:";
	char *check_str = NULL;
	char *pt_id_tmp = NULL;
	if (strstr(sms,str) == NULL)
		return FALSE;
	pt_id_tmp = strtok(sms,":");
	pt_id_tmp = strtok(NULL,":");
	pt_id_tmp = strtok(pt_id_tmp," ");
	check_str = strtok(NULL," ");
	strcpy(pt_id,pt_id_tmp);
	if (strcmp(check_str,"正常") == 0)
		*status = 0;
	else if (strcmp(check_str,"异常") == 0)
		*status = 1;
	return TRUE;
}