/*****************************************************************************************

FILENAME:	commonlib.c

FUNCTION:	
	
	  
VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	start to update the file from H9000 V3.0 to V4.0		yyp

*****************************************************************************************/

#include "../inc/lanhead_all.h"

/*********************** Get ems config info from db ********************************/
int GetEmsConfig()
{
	int i,j,k;
	char typestr[20];
	UCHAR stnId;
	g_is_island = FALSE;
	g_is_grp_brd = FALSE;
	gethostname(g_hostname,50);
	if(GetSrcId(&g_localhost_id) == FALSE)
	{
		return FALSE;
	}
	if(IsHostIsland()==TRUE)
	{
		g_is_island = TRUE;
	}
	if( IsLocalHostDispatch() == TRUE )
	{
		g_cascade = YES;	
	}
	else
	{
		g_cascade = NO;
	}
	/******** Get SysNetNum ********/
	if(g_is_island == TRUE)
	{
		g_sys_netnum = 1;
	}
	else
	{
		g_sys_netnum =GetSysNetNum();
	}

	if( g_sys_netnum ==1 )
	{
		g_sys_dualnet = NO ;
	}
	else if( g_sys_netnum ==2 )
	{
		g_sys_dualnet = YES ;
	}
	else
	{
		printf("error: SysNetNum =%d invalid \n",g_sys_netnum);
		return FALSE;
	}

	/******** Get CtrlNetNum ********/
	g_ctrl_netnum = GetCtrlNetNum();
	if( g_ctrl_netnum ==1 )
	{
		g_ctrl_dualnet = NO ;
	}
	else if( g_ctrl_netnum ==2 )
	{
		g_ctrl_dualnet = YES ;
	}
	else
	{
		printf("error: CtrlNetNum =%d invalid \n",g_ctrl_netnum);
		return FALSE;
	}
	
	/********** Get Broadcast addr *************/
	for(i=0;i<g_sys_netnum;i++)
	{
		if(g_is_island == FALSE)
		{
			if(GetBroadcastIpaddr(i, g_brd_ip[i]) ==-1 )
			{
				printf("error: Get Brdcst Ip failed \n");
				return FALSE;
			}
		}
		else
		{
			strcpy(g_brd_ip[i],"127.0.0.1");
		}
	}
	
	/********** Get Station info *************/
	for(i=1;i<=MAX_STN_NUM;i++)
	{
	  g_dev_num[i]=0;
	}
	if(GetStationNum((unsigned int *)&g_stn_num) ==-1)
	{
		return FALSE;
	}
	for(i=1;i<=MAX_STN_ID;i++)
	{
		index_stn[i]=-1;
	}
	for(i=1;i<=g_stn_num;i++)
	{
		GetStnIdByIndex(i, &stnId);
		if( stnId<=0 || stnId >MAX_STN_ID )
		{
			printf("GetEmsConfig():stnId =%d err\n",stnId);
			return FALSE;
		}
		index_stn[stnId] = i;
		id_stn[i] = stnId;
	}
	if(g_stn_num <0) g_stn_num = 0;
 	for(i=1;i<=g_stn_num;i++) 
  	{  	
		if(GetGroupNumById( id_stn[i], (unsigned int *)&g_dev_num[i] )==-1)
		 {
		   return FALSE;
		 }
		if(g_dev_num[i] <0) g_dev_num[i] = 0;
	}
	/***************** get sys lcu id ***************/
	for(i=1;i<=g_stn_num;i++)
	{
		g_sys_lcu[i]=0;
		for(j=1;j<=g_dev_num[i];j++ )
		{
			GetPlcTypeStr(id_stn[i], j, typestr);
			if(strcmp(typestr,"SYS") == 0)
			{
				g_sys_lcu[i]=j;
				break;
			}
		}
	} 

	/*********** Get plc cpu net mode *************/
	for(i=1;i<=g_stn_num;i++)
	{
		for(j=1;j<=g_dev_num[i];j++)
		{
			g_cpu_net_mode[i][j] = 4;
			GetPlcMode(id_stn[i], j,&(g_cpu_net_mode[i][j]));
		}
	}

	/*********** Get all plc ip addr *************/
	if( g_cascade != YES )
	{
		for(i=1;i<=g_stn_num;i++)
		{
			for(j=1;j<=g_dev_num[i];j++)
			{
				g_plc_ip_num[i][j] =0;
				GetPlcTypeStr(id_stn[i], j, typestr);
				if(strcmp(typestr,"SYS") == 0 || strcmp(typestr,"RTP") == 0)
				{
					continue;
				}
				g_plc_ip_num[i][j] = GetLcuIpaddrNum(id_stn[i], j);
				if(g_plc_ip_num[i][j]>4) g_plc_ip_num[i][j]=4;
				if(g_plc_ip_num[i][j]<0) g_plc_ip_num[i][j]=0;
				if(g_plc_ip_num[i][j] <=0)
				{
					printf("lanlib: plc %d.%d ipnum =%d error\n",i,j,g_plc_ip_num[i][j]);
					return FALSE;
				}
				for(k=0;k<g_plc_ip_num[i][j];k++)
				{
					if(GetLcuIpaddr(id_stn[i], j, k, g_plc_ip[i][j][k])==-1)
					{
						return FALSE;
					}
				}
			}	
		}


		for(i=1;i<=g_stn_num;i++)
		{
			for(j=1;j<=g_dev_num[i];j++)
			{
				g_plcscan_host_num[i][j] =0;
				GetPlcTypeStr(id_stn[i], j, typestr);
				if(strcmp(typestr,"SYS") == 0 || strcmp(typestr,"RTP") == 0)
				{
					continue;
				}			
				g_plcscan_host_num[i][j]=GetPlcScanHostNum( id_stn[i] ,j);
				if(g_plcscan_host_num[i][j]>MAX_SCAN_HOST_NUM) 
				{
					g_plcscan_host_num[i][j] = MAX_SCAN_HOST_NUM;
				}
				if(g_plcscan_host_num[i][j]<0) 
				{
					g_plcscan_host_num[i][j] = 0;
				}
				for(k=0;k < g_plcscan_host_num[i][j];k++)
				{
					if(GetPlcScanHost( id_stn[i] ,j,k,g_plcscan_host[i][j][k])==-1)
					{
						return FALSE;
					}
				}
			}
		}
	}
	/*********** Get system rtu id *************/
	for(i=1;i<=g_stn_num;i++)
	{
		if(GetSysLcuId( id_stn[i] , &(g_sys_lcu[i])) == -1 )
		{
			return FALSE;
		}
	}
	/* get ctrl_net_independ */
	g_ctrl_net_independ=IsCtrlNetIndepend();

	ReadAppDefFile();
	
	return TRUE;
}

//added by yyp 2011.03.08
int ClearCommShm(unsigned char stn_id,int id,int type)
{
	int i;
	unsigned char idx;
	if( stn_id<1 || stn_id>MAX_STN_NUM )
	{
		printf("ClearCommShm():id =%d err\n",id);
		return FALSE;
	}
	idx = index_stn[stn_id];
	if(-1==WaitforSem(SEM_WR_COMMCMD, 2))                                                                                                         
	{	
		printf("ClearCommShm(): WaitforSem return -1 err\n");
		return FALSE;
	}
	if( id<0 || id >= MAX_NETCMD_SEG_NUM )
	{
		printf("ClearCommShm():id =%d err\n",id);
		return FALSE;
	}
	if(type == TYPE_ONE_SEG)
	{
		memset(&(shm_comm_cmd->stn_data[idx].seg[id]),0,sizeof(shm_comm_cmd->stn_data[idx].seg[id]));
	}
	else if(type == TYPE_ALL_SEG)
	{
		for(i=0;i<MAX_NETCMD_SEG_NUM;i++)
		{
			memset(&(shm_comm_cmd->stn_data[idx].seg[i]),0,sizeof(shm_comm_cmd->stn_data[idx].seg[i]));
		}
	}
	ReleaseWriteSem(SEM_WR_COMMCMD);
	return TRUE;
}
void ReadAppDefFile()
{
	char FileName[100]="/home/ems/h9000/def/appconfig.def";
	char StrTmp[100];
	FILE *fp;
	strcpy(g_sid,"HISTA");
	g_updatefirst = 1;
	g_wranyhost = 1;
	g_plc_mode = MIXPLC;
	g_sync_time = 0;
	g_simumode = 0;
	g_ipwithcpu = 0;
	g_is_simuserver = 0;
	if (( fp = fopen ( FileName,"r"))==NULL)
	{
		printf ("==== can't open (w) the file %s! \n",FileName );
		return;
	}
	else
	{
		/* printf ( "\n==== open (w) %s  success! \n",FileName  ); */
	}
	while ( fscanf(fp,"%s",StrTmp)!=EOF )
	{
		if(strcmp(StrTmp,"BEGIN")==0) break;
	}        
	while ( fscanf(fp,"%s",StrTmp)!=EOF )
	{
		if(strcmp(StrTmp,"SID")==0)
		{
			fscanf(fp,"%s",StrTmp);
			strcpy(g_sid,StrTmp);
		}
		if(strcmp(StrTmp,"UPDATEFIRST")==0)
		{
			fscanf(fp,"%d",&g_updatefirst);
		}
		if(strcmp(StrTmp,"WRANYHOST")==0)
		{
			fscanf(fp,"%d",&g_wranyhost);
		}
		if(strcmp(StrTmp,"WRCOND")==0)
		{
			fscanf(fp,"%d",&g_wrcond);
		}
		if(strcmp(StrTmp,"WROFFSET")==0)
		{
			fscanf(fp,"%d",&g_wroffset);
		}
		if(strcmp(StrTmp,"WRANYHOST")==0)
		{
			fscanf(fp,"%d",&g_wranyhost);
		}
		if(strcmp(StrTmp,"SYNCTIME")==0)
		{
			fscanf(fp,"%d",&g_sync_time);
		}
		if(strcmp(StrTmp,"SIMUMODE")==0)
		{
			fscanf(fp,"%d",&g_simumode);
		}
		if(strcmp(StrTmp,"GRP_BRD")==0)
		{
			fscanf(fp,"%d",&g_is_grp_brd);
			//needed to  be modified by yyp 2010.10.09
			g_sys_dualnet = NO;
		}
		/*ADD BY YYP 11-01-06*/
		if(strcmp(StrTmp,"IPWITHCPU")==0)
		{
			fscanf(fp,"%d",&g_ipwithcpu);		
		}
		/*ADD BY WAYNE 09-01-09*/
		if(strcmp(StrTmp,"HOSTADDRESSFORMYSQL")==0)
		{
			fscanf(fp,"%s",StrTmp);
			strcpy(g_hostname_mysql,StrTmp);			
		}
		if(strcmp(StrTmp,"SIMUSERVER")==0)
		{
			fscanf(fp,"%d",&g_is_simuserver);		
		}


		if(strcmp(StrTmp,"END")==0)
		{
			break;
		}
	}
	fclose(fp);
}

int MySleep(int mSec)
{
#ifdef MSWIN32
	Sleep(mSec);
#else
/*
	struct timespec curtime;
	curtime.tv_sec = mSec/1000;
	curtime.tv_nsec = (mSec%1000) * 1000000;
	nanosleep(&curtime,NULL);
*/	
	usleep(mSec * 1000);
#endif
	return TRUE;
}

int GetSrcId(unsigned char * src_id)
{
	int ret;
	ret =0;
	ret = GetLocalHostId( src_id);
	if(ret == -1 || (*src_id)<=0) 
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

int GetPlcTypeStr(unsigned char stnid,unsigned char devid, char * typestr)
{
	if(GetGroupTypeNameById( stnid,devid, typestr) == -1) 
	{
		return FALSE;
	}
	return TRUE;
}

unsigned char GetCmdBufId(unsigned char src_id)
{
	unsigned char cmd_id;
	if(GetHostCmdId(src_id, &cmd_id)==-1)
	{
		return -1;
	}
	return cmd_id;
}

int rdb_net( unsigned char stnid, unsigned char devid)
{

	char    buf[50];
	short   hot_net;
	unsigned short	status;
	LCUID	plcid;
	char	stn_name[STATION_NAME_SIZE];
	char	dev_name[GROUP_NAME_SIZE];

	if(g_ctrl_netnum <= 1)	return(0);
	if(GetStnNameById(stnid,stn_name)==-1) return(0);
	if(GetGroupNameById(stnid,devid,dev_name)==-1) return(0);
	if(devid>GROUP_ENTRY_NUMBER || devid<=0 || strcmp(dev_name,"")==0)
	{
		printf("\nrdb.c:: Rdb_net devid =%d  error!\n",devid);
		return(0);
	}
	strcpy(buf,stn_name);
	strcat(buf,".");
	strcat(buf,dev_name);
	strcat(buf,".SYS.NET_WK_STA");
	if(GetIndPtStateByNameStr(buf,&status)==-1)
	{
		printf("\ncommonlib.c::%s No this point in the database file!\n",buf);
		return(0);
	}
	switch( status )
	{
	case  1:        
		hot_net=1;
		break;

	case  0: 
		hot_net=0;
		break;

	default:        
		hot_net=0;
		plcid.stnid=stnid;
		plcid.devid=devid;
		BdNetHost(plcid,(unsigned char)0 );
		break;
	}
	return(hot_net);
}

/***** yyp debug?  04.14 *****/
int rdb_cpu( unsigned char stnid, unsigned char devid)
{

	char    buf[50];
	short   hot_cpu;
	unsigned short	status;
	LCUID	plcid;	
	char	stn_name[STATION_NAME_SIZE];
	char	dev_name[GROUP_NAME_SIZE];
	if(GetStnNameById(stnid,stn_name)==-1) return(0);
	if(GetGroupNameById(stnid,devid,dev_name)==-1) return(0);
	if(devid>MAX_LCU_NUM || devid<=0 || strcmp(dev_name,"")==0)
	{
		printf("\ncommonlib.c:: rdb_cpu devid =%d  error!\n",devid);
		return(0);
	}
	strcpy(buf,stn_name);
	strcat(buf,".");
	strcat(buf,dev_name);
	strcat(buf,".SYS.CPU_WK_STA");
	if(GetIndPtStateByNameStr(buf,&status)==-1)
	{
		printf("\ncommonlib.c::%s No this point in the database file!\n",buf);
		return(0);
	}
	switch( status )
	{
	case  1:        
		hot_cpu=1;
		break;

	case  0: 
		hot_cpu=0;
		break;

	default:        
		hot_cpu=0;
		plcid.stnid=stnid;
		plcid.devid=devid;		
		BdCpuHost(plcid,(unsigned char)0 );
		break;
	}
	return(hot_cpu);
}

int WriteIndDBByStr(char * name, short status)
{
	DMS_COMMON	common_aa;
	WR_VAR_UNION	wr_var={0};
	POINTER		point;
	IOST	iost={0};
	DATE_TIME	time={0};
	if(-1==GetPtIdFromStr(name, &point))
	{
		printf("No this point in database\n" );
		return FALSE;
	}

	if(-1==ReadEntryById( &point, &common_aa ))
	{
		printf("No this point %s in database\n", name);
		return FALSE;
	}
		
	GetSysTime(&time);
	wr_var.ind= common_aa.var.ind.status;
	if(wr_var.ind.rt_state != status)
	{
		wr_var.ind.rt_state= status;
		SetPtValueIostById( &point, wr_var, iost, time, RW_RT_VALUE );
	}	
	return(TRUE);
}


int BdcastMsg(unsigned char stnid,unsigned char dev_id,char type,short point_id,short status)
{
	LAN_HEAD        lan_head;
	SYS_MSG		lan_msg;
	time_t          tt,return_t;
	struct          tm *ttime;
	char		log_str[256];

	return_t=time(&tt);
	ttime = localtime( &tt);
	lan_head.dest_id=0;
	lan_head.dp_type=BJ_DB_TIME;
	lan_head.length=sizeof(SYS_MSG);
	lan_head.dest_stn=stnid;
	GetSrcId((UCHAR	*)&lan_head.src_id);

	lan_msg.type_id=DPS_IND_ALM;
	lan_msg.stn_id = stnid;
	lan_msg.dev_id = dev_id;
	lan_msg.data_type=type;
	lan_msg.number=1;
	lan_msg.point_id=point_id;
	status = status +256; /* added by yyp 06.11.10 */
	memcpy(&lan_msg.status, &status, 2);
	lan_msg.ms=0;
	GetSrcId((UCHAR	*)&lan_msg.host_id);
	lan_msg.sec=ttime->tm_sec;
	lan_msg.min=ttime->tm_min;
	lan_msg.hour=ttime->tm_hour;
	lan_msg.day=ttime->tm_mday;
	lan_msg.month=ttime->tm_mon+1;
	lan_msg.year=ttime->tm_year+1900;
#ifdef _BIG_ENDIAN_
	Swap2Byte(&lan_head.length);
	Swap2Byte(&lan_msg.point_id);
	Swap2Byte(&lan_msg.number);
	Swap2Byte(&lan_msg.status);
	Swap2Byte(&lan_msg.year);
#endif		
	lan_out ((char *)&lan_msg,lan_head,0);
		
	sprintf(log_str,"BdcastMsg:: pt %d.%d.%d.%d status=%d\n",stnid,dev_id,type,point_id,status); 
	log_write(H9000_LOG_INFO,1,log_str);
	return(TRUE);
}

int GetPtQlt(POINTER pt,short *p_qlt)
{
	FB_STATUS1 fb_status1;
	short tmp;
	if(pt.data_type != DEVOBJ_TYPE)
	{
		IsPtValueChanFailById( &pt, (int *)p_qlt);
	}
	else
	{
		GetObjPtFbStatus1ById( &pt, &fb_status1 );
		tmp = fb_status1.swgf_n;
		if(tmp == 0)
		{
			*p_qlt=1;
		}
		else
		{
			*p_qlt=0;
		}
	}
	return(TRUE);
}


int BdcastQltWithSta(POINTER pt, short rt_state,short qlt_state)
{
	LAN_HEAD        lan_head;
	SYS_MSG		lan_msg;
	IND_DATA	status;
	DMS_COMMON      rs;
	WR_VAR_UNION	new_value;
	IOST		iost;
	DATE_TIME	updatetime;
	time_t          tt,return_t;
	struct          tm *ttime;
	unsigned char 	stnid,dev_id,type;
	short 		point_id;
	char		log_str[256];

	if(qlt_state>1 || qlt_state<0 ) return FALSE;	
	if(ReadEntryById( &pt, &rs) ==-1) return FALSE;	
	status = rs.var.ind.status;
	if( status.chan_fail == qlt_state ) return TRUE;
	status.alm_type = IND_ALM_TYPE_CHANFAIL;
	status.chan_fail = qlt_state;
	status.rt_state = rt_state;
	stnid = pt.stn_id;
	dev_id = pt.dev_id;
	type = pt.data_type;
	point_id = pt.pt_id;	
	return_t=time(&tt);
	ttime = localtime( &tt);
	lan_head.dest_id=0;
	lan_head.dp_type=BJ_DB_TIME;
	lan_head.length=sizeof(SYS_MSG);
	lan_head.dest_stn=stnid;
	GetSrcId((UCHAR	*)&lan_head.src_id);

	lan_msg.type_id=DPS_IND_ALM;
	lan_msg.stn_id = stnid;
	lan_msg.dev_id = dev_id;
	lan_msg.data_type=type;
	lan_msg.number=1;
	lan_msg.point_id=point_id;
		
	memcpy(&lan_msg.status, &status, 2);
	lan_msg.ms=0;
	GetSrcId((UCHAR	*)&lan_msg.host_id);
	lan_msg.sec=ttime->tm_sec;
	lan_msg.min=ttime->tm_min;
	lan_msg.hour=ttime->tm_hour;
	lan_msg.day=ttime->tm_mday;
	lan_msg.month=ttime->tm_mon+1;
	lan_msg.year=ttime->tm_year+1900;
#ifdef _BIG_ENDIAN_
	Swap2Byte(&lan_head.length);
	Swap2Byte(&lan_msg.point_id);
	Swap2Byte(&lan_msg.number);
	Swap2Byte(&lan_msg.status);
	Swap2Byte(&lan_msg.year);
#endif		
	lan_out ((char *)&lan_msg,lan_head,0);
		
	sprintf(log_str,"BdcastQlt:: pt %d.%d.%d.%d qlt=%d\n",stnid,dev_id,type,point_id,qlt_state); 
	log_write(H9000_LOG_INFO,1,log_str);

	//added by yyp 2012.11.10
	rs.var.ind.status.chan_fail = qlt_state;
	new_value.ind=rs.var.ind.status ;
	SetPtValueIostById( &pt, new_value, iost, updatetime, RW_RT_VALUE);		

	return(TRUE);
}

int BdcastQltANA(POINTER pt,short qlt_state)
{
	LAN_HEAD        lan_head;
	ANLG_STATUS		status;
	unsigned char	stnid,dev_id;
	char 		type;
	int		data;
	short 		point_id;
	ANA_MSG		lan_msg;
	time_t          tt,return_t;
	struct          tm *ttime;
	short	oldqlt;
	float	value;
	char	log_str[256];
	if( GetPtQlt(pt,&oldqlt)!=TRUE ) return(FALSE);
	if( oldqlt == qlt_state )
	{
		return TRUE;
	}
	if( GetAnaPtfValueById( &pt, &value )==-1 ) return(FALSE);
	stnid=pt.stn_id;
	dev_id=pt.dev_id;
	point_id=pt.pt_id;
	type = pt.data_type;
	return_t=time(&tt);
	ttime = localtime( &tt);
	lan_head.dest_id=0;
	lan_head.dp_type=BJ_DB_TIME;
	lan_head.length=sizeof(ANA_MSG);
	lan_head.dest_stn=stnid;
	GetSrcId((UCHAR	*)&lan_head.src_id);
	
	lan_msg.type_id=DPS_ANALOG;
	lan_msg.stn_id = stnid;
	lan_msg.dev_id = dev_id;
	lan_msg.data_type=type;
	lan_msg.number=1;
	lan_msg.point_id=point_id;
	lan_msg.status = 256;
	status.chan_fail = qlt_state;
	memcpy(&lan_msg.status, &status,2);
	GetSrcId((UCHAR	*)&lan_msg.host_id);
	lan_msg.sec=ttime->tm_sec;
	lan_msg.min=ttime->tm_min;
	lan_msg.hour=ttime->tm_hour;
	lan_msg.day=ttime->tm_mday;
	lan_msg.month=ttime->tm_mon+1;
	lan_msg.year=ttime->tm_year+1900;
	
	data=(int)(value * 1000);
	memcpy(lan_msg.data,&data,4);
#ifdef _BIG_ENDIAN_
	Swap2Byte(&lan_head.length);
	Swap2Byte(&lan_msg.point_id);
	Swap2Byte(&lan_msg.number);
	Swap2Byte(&lan_msg.status);
	Swap2Byte(&lan_msg.year);
	Swap4Byte(&lan_msg.data);
#endif		
	lan_out ((char *)&lan_msg,lan_head,0);
	//yyp removed 2012.09.05	
	//sprintf(log_str,"BdANA:: pt %d.%d.%d.%d data=%.3f old=%.3f\n",stnid,dev_id,type,point_id,value,oldvalue); 
	//log_write(H9000_LOG_INFO,2,log_str);
	return(TRUE);
}      


int BdcastQlt(POINTER pt,short qlt_state)
{
	LAN_HEAD        lan_head;
	SYS_MSG		lan_msg;
	IND_DATA	status;
	DMS_COMMON      rs;
	WR_VAR_UNION	new_value;
	IOST		iost;
	DATE_TIME	updatetime;
	time_t          tt,return_t;
	struct          tm *ttime;
	unsigned char 	stnid,dev_id,type;
	short 		point_id;
	char		log_str[256];

	if(qlt_state>1 || qlt_state<0 ) return FALSE;	
	if(pt.data_type == ANA_TYPE) 
	{
		if(BdcastQltANA(pt,qlt_state) == TRUE)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	if(pt.data_type == DEVOBJ_TYPE) 
	{
		if(BdcastQltObj(pt,qlt_state) == TRUE)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	if(ReadEntryById( &pt, &rs) ==-1) return FALSE;
	
	status = rs.var.ind.status;
	if( status.chan_fail == qlt_state ) return TRUE;
	status.alm_type = IND_ALM_TYPE_CHANFAIL;
	status.chan_fail = qlt_state;
	stnid = pt.stn_id;
	dev_id = pt.dev_id;
	type = pt.data_type;
	point_id = pt.pt_id;	
	return_t=time(&tt);
	ttime = localtime( &tt);
	lan_head.dest_id=0;
	lan_head.dp_type=BJ_DB_TIME;
	lan_head.length=sizeof(SYS_MSG);
	lan_head.dest_stn=stnid;
	GetSrcId((UCHAR	*)&lan_head.src_id);

	lan_msg.type_id=DPS_IND_ALM;
	lan_msg.stn_id = stnid;
	lan_msg.dev_id = dev_id;
	lan_msg.data_type=type;
	lan_msg.number=1;
	lan_msg.point_id=point_id;
		
	memcpy(&lan_msg.status, &status, 2);
	lan_msg.ms=0;
	GetSrcId((UCHAR	*)&lan_msg.host_id);
	lan_msg.sec=ttime->tm_sec;
	lan_msg.min=ttime->tm_min;
	lan_msg.hour=ttime->tm_hour;
	lan_msg.day=ttime->tm_mday;
	lan_msg.month=ttime->tm_mon+1;
	lan_msg.year=ttime->tm_year+1900;
#ifdef _BIG_ENDIAN_
	Swap2Byte(&lan_head.length);
	Swap2Byte(&lan_msg.point_id);
	Swap2Byte(&lan_msg.number);
	Swap2Byte(&lan_msg.status);
	Swap2Byte(&lan_msg.year);
#endif		
	lan_out ((char *)&lan_msg,lan_head,0);
		
	sprintf(log_str,"BdcastQlt:: pt %d.%d.%d.%d qlt=%d\n",stnid,dev_id,type,point_id,qlt_state); 
	log_write(H9000_LOG_INFO,1,log_str);

	//added by yyp 2012.11.10
	rs.var.ind.status.chan_fail = qlt_state;
	new_value.ind=rs.var.ind.status ;
	SetPtValueIostById( &pt, new_value, iost, updatetime, RW_RT_VALUE);		

	return(TRUE);
}     


int BdcastQltObj(POINTER pt,short qlt_state)
{
	DMS_COMMON      rs;
	FB_STATUS1	fb_status1;
	LAN_HEAD        lan_head;
	unsigned char	stnid,dev_id;
	char 		type;
	short 		point_id;
	OBJ_MSG		lan_msg;
	short		real_state;
	time_t          tt,return_t;
	struct          tm *ttime;
	char	log_str[256];
	real_state = qlt_state;
	if( qlt_state==0 )
	{
		qlt_state=1;
	}
	else
	{
		qlt_state=0;
	}
	if(ReadEntryById( &pt, &rs) ==-1) 
		return FALSE;

	stnid=pt.stn_id;
	dev_id=pt.dev_id;
	point_id=pt.pt_id;
	type = pt.data_type;
	return_t=time(&tt);
	ttime = localtime( &tt);
	lan_head.dest_id=0;
	lan_head.dp_type=BJ_DB_TIME;
	lan_head.length=sizeof(OBJ_MSG);
	lan_head.dest_stn=stnid;
	GetSrcId((UCHAR	*)&lan_head.src_id);

	lan_msg.type_id=DPS_DEV_FB;
	lan_msg.stn_id = stnid;
	lan_msg.dev_id = dev_id;
	lan_msg.data_type=type;
	lan_msg.number=1;
	lan_msg.point_id=point_id;
	fb_status1 = rs.var.devobj.fb_status1;
	fb_status1.swgf_n = qlt_state;
	memcpy(&lan_msg.status1, &fb_status1, 2);
	memcpy(&lan_msg.status2, &rs.var.devobj.fb_status2, 2);
	memcpy(&lan_msg.param , &rs.var.devobj.param, 2);
	lan_msg.ms=0;
	GetSrcId((UCHAR	*)&lan_msg.host_id);
	lan_msg.sec=ttime->tm_sec;
	lan_msg.min=ttime->tm_min;
	lan_msg.hour=ttime->tm_hour;
	lan_msg.day=ttime->tm_mday;
	lan_msg.month=ttime->tm_mon+1;
	lan_msg.year=ttime->tm_year+1900;
#ifdef _BIG_ENDIAN_
	Swap2Byte(&lan_head.length);
	Swap2Byte(&lan_msg.point_id);
	Swap2Byte(&lan_msg.number);
	Swap2Byte(&lan_msg.status1);
	Swap2Byte(&lan_msg.status2);
	Swap2Byte(&lan_msg.year);
#endif		
	lan_out ((char *)&lan_msg,lan_head,0);
		
	sprintf(log_str,"BdcastQltObj:: pt %d.%d.%d.%d qlt=%d\n",stnid,dev_id,type,point_id,real_state); 
	log_write(H9000_LOG_INFO,2,log_str);


	//added by yyp 2012.11.10
	rs.var.devobj.fb_status1.swgf_n = qlt_state;
	SetDevObjPtVarById(&pt, rs.var.devobj);

	return(TRUE);
	
	
}    


              
int BdObjByID(POINTER pt,short status)
{
	DMS_COMMON      rs;
	FB_STATUS1	fb_status1;
	LAN_HEAD        lan_head;
	unsigned char	stnid,dev_id;
	char 		type;
	short 		point_id;
	OBJ_MSG		lan_msg;
	time_t          tt,return_t;
	struct          tm *ttime;
	char	log_str[256];

	if(ReadEntryById( &pt, &rs) ==-1) 
		return FALSE;

	stnid=pt.stn_id;
	dev_id=pt.dev_id;
	point_id=pt.pt_id;
	type = pt.data_type;
	return_t=time(&tt);
	ttime = localtime( &tt);
	lan_head.dest_id=0;
	lan_head.dp_type=BJ_DB_TIME;
	lan_head.length=sizeof(OBJ_MSG);
	lan_head.dest_stn=stnid;
	GetSrcId((UCHAR	*)&lan_head.src_id);

	lan_msg.type_id=DPS_DEV_FB;
	lan_msg.stn_id = stnid;
	lan_msg.dev_id = dev_id;
	lan_msg.data_type=type;
	lan_msg.number=1;
	lan_msg.point_id=point_id;
	fb_status1 = rs.var.devobj.fb_status1;
	if(status == 0)
	{
		fb_status1.p0 =1;
		fb_status1.p1 =0;
	}
	else
	{
		fb_status1.p0 =0;
		fb_status1.p1 =1;
	}
	memcpy(&lan_msg.status1, &fb_status1, 2);
	memcpy(&lan_msg.status2, &rs.var.devobj.fb_status2, 2);
	memcpy(&lan_msg.param , &rs.var.devobj.param, 2);
	lan_msg.ms=0;
	GetSrcId((UCHAR	*)&lan_msg.host_id);
	lan_msg.sec=ttime->tm_sec;
	lan_msg.min=ttime->tm_min;
	lan_msg.hour=ttime->tm_hour;
	lan_msg.day=ttime->tm_mday;
	lan_msg.month=ttime->tm_mon+1;
	lan_msg.year=ttime->tm_year+1900;
#ifdef _BIG_ENDIAN_
	Swap2Byte(&lan_head.length);
	Swap2Byte(&lan_msg.point_id);
	Swap2Byte(&lan_msg.number);
	Swap2Byte(&lan_msg.status1);
	Swap2Byte(&lan_msg.status2);
	Swap2Byte(&lan_msg.year);
#endif		
	lan_out ((char *)&lan_msg,lan_head,0);
		
	sprintf(log_str,"BdOBJ:: pt %d.%d.%d.%d status=%d\n",stnid,dev_id,type,point_id,status); 
	log_write(H9000_LOG_INFO,2,log_str);
	return(TRUE);
	
	
}

int BdINDByID(POINTER pt,short status)
{
	LAN_HEAD        lan_head;
	unsigned char	stnid,dev_id;
	short		state;
	char 		type;
	short 		point_id;
	SYS_MSG		lan_msg;
	time_t          tt,return_t;
	struct          tm *ttime;
	char	log_str[256];
	int			result;

	if( pt.data_type == DEVOBJ_TYPE)
	{
		result = BdObjByID(pt,status);
		return result;
	}

	if(GetIndPtStateById(&pt,(unsigned short *)(&state))==-1) return(FALSE);

	if( status == state )
	{
		return TRUE;
	}

	stnid=pt.stn_id;
	dev_id=pt.dev_id;
	point_id=pt.pt_id;
	type = pt.data_type;
	return_t=time(&tt);
	ttime = localtime( &tt);
	lan_head.dest_id=0;
	lan_head.dp_type=BJ_DB_TIME;
	lan_head.length=sizeof(SYS_MSG);
	lan_head.dest_stn=stnid;
	GetSrcId((UCHAR	*)&lan_head.src_id);

	lan_msg.type_id=DPS_IND_ALM;
	lan_msg.stn_id = stnid;
	lan_msg.dev_id = dev_id;
	lan_msg.data_type=type;
	lan_msg.number=1;
	lan_msg.point_id=point_id;
	status = status +256; /* added by yyp 06.11.10 */	
	memcpy(&lan_msg.status, &status, 2);
	lan_msg.ms=0;
	GetSrcId((UCHAR	*)&lan_msg.host_id);
	lan_msg.sec=ttime->tm_sec;
	lan_msg.min=ttime->tm_min;
	lan_msg.hour=ttime->tm_hour;
	lan_msg.day=ttime->tm_mday;
	lan_msg.month=ttime->tm_mon+1;
	lan_msg.year=ttime->tm_year+1900;
#ifdef _BIG_ENDIAN_
	Swap2Byte(&lan_head.length);
	Swap2Byte(&lan_msg.point_id);
	Swap2Byte(&lan_msg.number);
	Swap2Byte(&lan_msg.status);
	Swap2Byte(&lan_msg.year);
#endif		
	lan_out ((char *)&lan_msg,lan_head,0);
		
	sprintf(log_str,"BdIND:: pt %d.%d.%d.%d status=%d\n",stnid,dev_id,type,point_id,status); 
	log_write(H9000_LOG_INFO,2,log_str);
	return(TRUE);
}     

int BdObjForceByID(POINTER pt,short status)
{
	DMS_COMMON      rs;
	FB_STATUS1	fb_status1;
	LAN_HEAD        lan_head;
	unsigned char	stnid,dev_id;
	char 		type;
	short 		point_id;
	OBJ_MSG		lan_msg;
	time_t          tt,return_t;
	struct          tm *ttime;
	char	log_str[256];

	if(ReadEntryById( &pt, &rs) ==-1) 
		return FALSE;

	stnid=pt.stn_id;
	dev_id=pt.dev_id;
	point_id=pt.pt_id;
	type = pt.data_type;
	return_t=time(&tt);
	ttime = localtime( &tt);
	lan_head.dest_id=0;
	lan_head.dp_type=BJ_DB_TIME;
	lan_head.length=sizeof(OBJ_MSG);
	lan_head.dest_stn=stnid;
	GetSrcId((UCHAR	*)&lan_head.src_id);

	lan_msg.type_id=DPS_DEV_FB;
	lan_msg.stn_id = stnid;
	lan_msg.dev_id = dev_id;
	lan_msg.data_type=type;
	lan_msg.number=1;
	lan_msg.point_id=point_id;
	fb_status1 = rs.var.devobj.fb_status1;
	if(status == 0)
	{
		fb_status1.p0 =1;
		fb_status1.p1 =0;
	}
	else
	{
		fb_status1.p0 =0;
		fb_status1.p1 =1;
	}
	fb_status1.swgf_n = 0;
	memcpy(&lan_msg.status1, &fb_status1, 2);
	memcpy(&lan_msg.status2, &rs.var.devobj.fb_status2, 2);
	memcpy(&lan_msg.param , &rs.var.devobj.param, 2);
	lan_msg.ms=0;
	GetSrcId((UCHAR	*)&lan_msg.host_id);
	lan_msg.sec=ttime->tm_sec;
	lan_msg.min=ttime->tm_min;
	lan_msg.hour=ttime->tm_hour;
	lan_msg.day=ttime->tm_mday;
	lan_msg.month=ttime->tm_mon+1;
	lan_msg.year=ttime->tm_year+1900;
#ifdef _BIG_ENDIAN_
	Swap2Byte(&lan_head.length);
	Swap2Byte(&lan_msg.point_id);
	Swap2Byte(&lan_msg.number);
	Swap2Byte(&lan_msg.status1);
	Swap2Byte(&lan_msg.status2);
	Swap2Byte(&lan_msg.year);
#endif		
	lan_out ((char *)&lan_msg,lan_head,0);
		
	sprintf(log_str,"BdOBJ:: pt %d.%d.%d.%d status=%d\n",stnid,dev_id,type,point_id,status); 
	log_write(H9000_LOG_INFO,2,log_str);
	return(TRUE);
	
	
}



int WrANA ( char *db_name, float value, short status )
{
	POINTER point;
	float	oldvalue;
	DATE_TIME curtime;
	char	log_str[256];

	GetSysTime(&curtime);
	
	if(GetPtIdByNameStr( db_name, &point )==-1)
	{
		sprintf ( log_str, "\nin WrAna: %s is not found in database!\n", db_name );
		printf(log_str);
		return FALSE;
	}
	if( GetAnaPtfValueById( &point, &oldvalue )==-1 ) return(FALSE);

	if( (value-oldvalue)<0.001 && (value-oldvalue)>-0.001 )
	{
		return TRUE;
	}
	printf("WrANA %s = %.3f\n",db_name,value);
	SetAnaPtfValueById( &point,(float)value, curtime );
	return TRUE;
}

int	WrIND ( char *name, short status )
{
	DMS_COMMON      rs;	
	WR_VAR_UNION	new_value;
	ENTRY_DEVOBJ_VAL obj_value;

	IOST		iost;
	DATE_TIME	updatetime;
	POINTER point;
	short state;

	char	log_str[256];

	if(GetPtIdByNameStr( name, &point )==-1)
	{
		sprintf ( log_str, "\nin WrInd: %s is not found in database!\n", name );
		printf(log_str);
		return FALSE;
	}
	if(ReadEntryById( &point, &rs) ==-1) return FALSE;
	switch	( point.data_type )
	{
	case	SOE_TYPE:
	case	POL_TYPE:
	case	OUTS_TYPE:
	case	SYSMSG_TYPE:
		memcpy(&state,&rs.var.ind.status,2);
		if(status == state)
		{
			return TRUE;
		}
		memcpy(&new_value.ind,&status,2);
		
		SetPtValueIostById( &point, new_value, iost, updatetime, RW_RT_VALUE);											
		break;
	case     DEVOBJ_TYPE:/* removed by yyp on 2007.7.31*/
		memcpy(&state,&rs.var.devobj.fb_status1,2);
		if(status == state)
		{
			return TRUE;
		}
		obj_value = rs.var.devobj;
		memcpy ( &obj_value.fb_status1 ,  &status,sizeof(FB_STATUS1)) ;   
		SetDevObjPtVarById( &point, obj_value );
		
		break;
	}

	return TRUE;
}

int BdANAByID(POINTER pt,float value)
{
	LAN_HEAD        lan_head;
	unsigned char	stnid,dev_id;
	char 		type;
	int		data;
	short 		point_id;
	ANA_MSG		lan_msg;
	time_t          tt,return_t;
	struct          tm *ttime;
	float	oldvalue;
	char	log_str[256];
/****************************ZWJ ADD FOR ANA ALARM***********************/
	DMS_COMMON	dbcommpt;//
	ANLG_DATA	anadata;
	int     ana_alm_stat;

	if( GetAnaPtfValueById( &pt, &oldvalue )==-1 ) return(FALSE);
	
	if( (value-oldvalue)<0.001 && (value-oldvalue)>-0.001 )
	{
		return TRUE;
	}

	if ( ReadEntryById( &pt, &dbcommpt ) == -1 ) return(FALSE);
	lan_msg.type_id=DPS_ANALOG;
	lan_msg.status = 256;
	memset(&anadata.status,0,sizeof(ANLG_STATUS));
	anadata.status.alm_type=ANA_ALM_TYPE_LMT;
	
					if(value>dbcommpt.var.anlg.lmt_para.HI_ALARM)
					{
						ana_alm_stat=ANA_ALM_HI_ALARM;
					}
					if((value<=dbcommpt.var.anlg.lmt_para.HI_ALARM)&&(value>dbcommpt.var.anlg.lmt_para.HI_WARN))
					{
						ana_alm_stat=ANA_ALM_HI_WARN;
					}
					if((value>=dbcommpt.var.anlg.lmt_para.LO_ALARM)&&(value<dbcommpt.var.anlg.lmt_para.LO_WARN))
					{
						ana_alm_stat=ANA_ALM_LO_WARN;
					}
					if(value<dbcommpt.var.anlg.lmt_para.LO_ALARM)
					{
						ana_alm_stat=ANA_ALM_LO_ALARM;
					}
					if((value<=dbcommpt.var.anlg.lmt_para.HI_WARN)&&(value>=dbcommpt.var.anlg.lmt_para.LO_WARN))
					{
						ana_alm_stat=ANA_ALM_NORMAL;
					}
				
					if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_NORMAL)  /*原值正常*/
					{
						if(ana_alm_stat==ANA_ALM_HI_ALARM)                        /*新值越上上限*/
						{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;            /*越上上限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
						{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
						{	anadata.status.lmt_state=ANA_ALM_LO_WARN;			  /*越下限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if(ana_alm_stat==ANA_ALM_LO_ALARM)					  /*新值越下下限*/
						{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;			  /*越下下限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
					}
					else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_HI_ALARM)    /*原值越上上限*/
					{
						if(ana_alm_stat==ANA_ALM_HI_WARN)   /*新值越上限*/
						{	anadata.status.lmt_state=ANA_ALM_HI_WARN;          /*越上限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if((ana_alm_stat==ANA_ALM_NORMAL)&&(value>=(dbcommpt.var.anlg.lmt_para.HI_WARN-dbcommpt.var.anlg.lmt_para.HI_RECOVER))) /*新值正常但没有越过上复限死区*/
						{	anadata.status.lmt_state=ANA_ALM_HI_WARN;          /*越上限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if((ana_alm_stat==ANA_ALM_NORMAL)&&(value<(dbcommpt.var.anlg.lmt_para.HI_WARN-dbcommpt.var.anlg.lmt_para.HI_RECOVER))) /*新值正常且越上复限死区*/
						{	anadata.status.lmt_state=ANA_ALM_HI_RECOVER;          /*越上限复归报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
						{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*越下限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
						{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
					}
					else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_HI_WARN)    /*原值越上限*/
					{
						if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
						{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if((ana_alm_stat==ANA_ALM_NORMAL)&&(value<(dbcommpt.var.anlg.lmt_para.HI_WARN-dbcommpt.var.anlg.lmt_para.HI_RECOVER))) /*新值正常且越上复限死区*/
						{	anadata.status.lmt_state=ANA_ALM_HI_RECOVER;          /*越上限复归报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
						{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*越下限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
						{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
					}
					else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_LO_WARN)    /*原值越下限*/
					{
						if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
						{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
						{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if((ana_alm_stat==ANA_ALM_NORMAL)&&(value>(dbcommpt.var.anlg.lmt_para.LO_WARN+dbcommpt.var.anlg.lmt_para.LO_RECOVER))) /*新值正常且越下复限死区*/
						{	anadata.status.lmt_state=ANA_ALM_LO_RECOVER;          /*越下限复归报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
						{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
					}
					else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_LO_ALARM)    /*原值越下下限*/
					{
						if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
						{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
						{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if((ana_alm_stat==ANA_ALM_NORMAL)&&(value>(dbcommpt.var.anlg.lmt_para.LO_WARN+dbcommpt.var.anlg.lmt_para.LO_RECOVER))) /*新值正常且越下复限死区*/
						{	anadata.status.lmt_state=ANA_ALM_LO_RECOVER;          /*越下限复归报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if((ana_alm_stat==ANA_ALM_NORMAL)&&(value<(dbcommpt.var.anlg.lmt_para.LO_WARN+dbcommpt.var.anlg.lmt_para.LO_RECOVER))) /*新值正常但没有越过下复限死区*/
						{	anadata.status.lmt_state=ANA_ALM_LO_WARN;          /*越下限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
						else if(ana_alm_stat==ANA_ALM_LO_WARN)                   /*新值越下限*/
						{	anadata.status.lmt_state=ANA_ALM_LO_WARN;            /*越下限报警*/
							lan_msg.type_id =DPS_ANA_ALM;
							anadata.status.alm_type=ANA_ALM_TYPE_LMT;
						}
					}
/***********************************************************************/
	

	stnid=pt.stn_id;
	dev_id=pt.dev_id;
	point_id=pt.pt_id;
	type = pt.data_type;
	return_t=time(&tt);
	ttime = localtime( &tt);
	lan_head.dest_id=0;
	lan_head.dp_type=BJ_DB_TIME;
	lan_head.length=sizeof(ANA_MSG);
	lan_head.dest_stn=stnid;
	GetSrcId((UCHAR	*)&lan_head.src_id);
	
	//lan_msg.type_id=DPS_ANALOG;//zwj 
	lan_msg.stn_id = stnid;
	lan_msg.dev_id = dev_id;
	lan_msg.data_type=type;
	lan_msg.number=1;
	lan_msg.point_id=point_id;
	//lan_msg.status = 256;//zwj
#ifdef _BIG_ENDIAN_
	CrossCopy(&lan_msg.status, &anadata.status,2);//zwj
#else
	memcpy(&lan_msg.status, &anadata.status,2);//zwj
#endif
	GetSrcId((UCHAR	*)&lan_msg.host_id);
	lan_msg.sec=ttime->tm_sec;
	lan_msg.min=ttime->tm_min;
	lan_msg.hour=ttime->tm_hour;
	lan_msg.day=ttime->tm_mday;
	lan_msg.month=ttime->tm_mon+1;
	lan_msg.year=ttime->tm_year+1900;
	

	data=(int)(value * 1000);
	memcpy(lan_msg.data,&data,4);
#ifdef _BIG_ENDIAN_
	Swap2Byte(&lan_head.length);
	Swap2Byte(&lan_msg.point_id);
	Swap2Byte(&lan_msg.number);
//	Swap2Byte(&lan_msg.status);
	Swap2Byte(&lan_msg.year);
	Swap4Byte(&lan_msg.data);
#endif		
	lan_out ((char *)&lan_msg,lan_head,0);
	//yyp removed 2012.09.05	
	//sprintf(log_str,"BdANA:: pt %d.%d.%d.%d data=%.3f old=%.3f\n",stnid,dev_id,type,point_id,value,oldvalue); 
	//log_write(H9000_LOG_INFO,2,log_str);
	return(TRUE);
}      
         

int BdINDByName(char* logicalname,short status)
{
	POINTER point;
	char	log_str[256];

	if(-1==GetPtIdByNameStr(logicalname, &point))
	{
		sprintf(log_str,"BdINDByName: pt %s not found\n",logicalname); 
		err_write(log_str);	
		return FALSE;
	}
	if(FALSE==BdINDByID(point,status))
	{
		return FALSE;
	}		
	return(TRUE);
}                          

int BdANAByName(char* logicalname,float value)
{
	POINTER point;
	char	log_str[256];

	if(-1==GetPtIdByNameStr(logicalname, &point))
	{
		sprintf(log_str,"BdANAByName: pt %s not found\n",logicalname); 
		err_write(log_str);	
		return FALSE;
	}
	if(FALSE==BdANAByID(point,value))
	{
		return FALSE;
	}		
	return(TRUE);
}   
int NetSwitchCmd(LCUID dev_id)
{      
	COMMAND_MSG     msg;
	LAN_HEAD        head;
	char            namebuf[40];
	char		stn_name[STATION_NAME_SIZE];
	char		dev_name[GROUP_NAME_SIZE];
	DATE_TIME	date_time;
	POINTER		point;
	USHORT		state;
	

	if(-1==GetStnNameById(dev_id.stnid, stn_name))
	{
		return -1;
	}
	if(-1==GetGroupNameById(dev_id.stnid, dev_id.devid, dev_name))
	{
		return -1;
	}	
	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "SYS.NET_WK_STA");
	
	if(-1==GetPtIdByNameStr(namebuf, &point))
	{
		return -1;
	}
	
	if(-1==GetIndPtStateById(&point, &state))
	{
		return -1;
	}
	
	GetSysTime(&date_time);
	
	if(state==0)
		msg.status.opr= 2;
	else
		msg.status.opr= 1;
		
	msg.type_id= C_NET_SWITCH;
	msg.stn_id= point.stn_id;
	msg.dev_id= point.dev_id;
	msg.data_type= point.data_type;
	msg.point_id= point.pt_id;
	msg.status.src= 0;
	msg.data.iValue= 0;
	msg.ctrl_addr= 0;
	GetSrcId((UCHAR	*)&msg.host_id);
	msg.usr_id= -1;
	msg.second= date_time.second;
	msg.minute= date_time.minute;
	msg.hour= date_time.hour;
	msg.day= date_time.day;
	msg.month= date_time.month;
	msg.year= date_time.year;
	strcpy(msg.usr_login_name, "SYSTEM");
	
	head.length= 32;
	head.dp_type= COMM_RECORD_TIME;
	head.dp_type= 0;
	head.dest_id= dev_id.devid;
	GetSrcId((UCHAR	*)&head.src_id);
	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&head.length);
	Swap2Byte(&msg.point_id);
	Swap2Byte(&msg.status);
	Swap4Byte(&msg.data);
	Swap2Byte(&msg.ctrl_addr);
	Swap2Byte(&msg.year);
#endif
	lan_out ((char *)&msg,head,0);  
	return(0);
}       

int BdNetHost(LCUID plcid,short status)
{
	char		devid,stnid;
	char		namebuf[40];
	char		stn_name[STATION_NAME_SIZE];
	char		dev_name[GROUP_NAME_SIZE];
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	POINTER		point;
	USHORT		state;

	devid=plcid.devid;
	stnid=plcid.stnid;
	re_time=time(&newtime);
	devtime=localtime(&newtime);

	if(-1==GetStnNameById(plcid.stnid, stn_name))
	{
		return -1;
	}
	if(-1==GetGroupNameById(plcid.stnid, plcid.devid, dev_name))
	{
		return -1;
	}
	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "SYS.NET_WK_STA");
	
	if(-1==GetPtIdByNameStr(namebuf, &point))
	{
		return -1;
	}
	
	if(-1==GetIndPtStateById(&point, &state))
	{
		return -1;
	}
	
	if( status != state)
	{
		BdcastMsg(stnid,devid,SYSMSG_TYPE,point.pt_id,status);
	}
	return(TRUE);
}

int BdCpuHost(LCUID plcid,short status)
{
	char		devid,stnid;
	char		namebuf[40];
	char		stn_name[STATION_NAME_SIZE];
	char		dev_name[GROUP_NAME_SIZE];
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	POINTER		point;
	USHORT		state;

	devid=plcid.devid;
	stnid=plcid.stnid;
	re_time=time(&newtime);
	devtime=localtime(&newtime);

	if(-1==GetStnNameById(plcid.stnid, stn_name))
	{
		return -1;
	}
	if(-1==GetGroupNameById(plcid.stnid, plcid.devid, dev_name))
	{
		return -1;
	}
	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "SYS.CPU_WK_STA");
	
	if(-1==GetPtIdByNameStr(namebuf, &point))
	{
		return -1;
	}
	
	if(-1==GetIndPtStateById(&point, &state))
	{
		return -1;
	}

	if( status != state)
	{
		BdcastMsg(stnid,devid,SYSMSG_TYPE,point.pt_id,status);
	}
	return(TRUE);
}

int BdCmdSendResult(struct net_cmd_seg netcmdseg, unsigned short result)
{
	netcmdseg.net_cmd_buf.netdata.status.result = result;
#ifdef _BIG_ENDIAN_
	Swap2Byte( &(netcmdseg.lan_head.length));
	Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.point_id));
	Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.status));
	Swap4Byte( &(netcmdseg.net_cmd_buf.netdata.data));
	Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.ctrl_addr));
	Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.year));
#endif	
	netcmdseg.lan_head.dp_type=COMM_RECORD_TIME;
	netcmdseg.lan_head.dest_id=0;
	lan_out ((char *)&(netcmdseg.net_cmd_buf.netdata),netcmdseg.lan_head,0);	
	return TRUE;
}


/********************************************************************************************************
 broadcast current plc ONOFF link state
*********************************************************************************************************/
short BdLCUONOFFStat(LCUID plcid,short state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	POINTER		pt;
	unsigned short	status;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.LCU_ONOFF_STA");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);

	if( status != state)
	{
		BdcastMsg(stnid,devid,SYSMSG_TYPE,pt.pt_id,state);
	}
	
	return(TRUE);
}

short GetLCUONOFFStat(LCUID plcid, short* state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	POINTER		pt;
	unsigned short	status;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.LCU_ONOFF_STA");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);
	
	*state = status;

	return(TRUE);
}

short BdPlcLinkStat(LCUID plcid,short state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	POINTER		pt;
	unsigned short	status;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.LINK_STA");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);

	if( status != state )
	{
		BdcastMsg(stnid,devid,SYSMSG_TYPE,pt.pt_id,state);
	}
	
	return(TRUE);
}

short GetPlcLinkStat(LCUID plcid, short* state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	POINTER		pt;
	unsigned short	status;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.LINK_STA");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);
	*state = status;
	return(TRUE);
}

short BdPlcScanStat(LCUID plcid,short state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	POINTER		pt;
	unsigned short	status;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.SCAN_STA");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);
	if( status != state )
	{
		BdcastMsg(stnid,devid,SYSMSG_TYPE,pt.pt_id,state);
	}
	return(TRUE);
}

short GetPlcScanStat(LCUID plcid, short* state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	POINTER		pt;
	unsigned short	status;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.SCAN_STA");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);
	*state = status;
	return(TRUE);
}

short BdScanHost(LCUID plcid)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	unsigned char srcid;
	POINTER		pt;
	unsigned short	status,state;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.SCAN_HOST");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	GetSrcId(&srcid);
	if(srcid > 15)
	{
		char log_str[256];

		sprintf(log_str,"BdScanHost(): srcid =%d invalid",srcid);
		log_write(H9000_LOG_ERROR,1,log_str);
		return FALSE;		
	}	
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);
	
	state = srcid;
	if( status != state)
	{
		BdcastMsg(stnid,devid,SYSMSG_TYPE,pt.pt_id,state);
	}
	
	return(TRUE);	
}

short GetScanHost(LCUID plcid, short* state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	POINTER		pt;
	unsigned short	status;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.SCAN_HOST");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);
	*state = status;
	return(TRUE);
}

/********************************************************************************************************
 broadcast current plc ONOFF link state
*********************************************************************************************************/
short BdLCUONOFFStatQlt(LCUID plcid,short state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	POINTER		pt;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.LCU_ONOFF_STA");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	BdcastQlt(pt,state);

	return(TRUE);
}

short BdPlcLinkStatQlt(LCUID plcid,short state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	POINTER		pt;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.LINK_STA");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);

	BdcastQlt(pt,state);
	
	return(TRUE);
}

short BdPlcScanStatQlt(LCUID plcid,short state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	POINTER		pt;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.SCAN_STA");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	BdcastQlt(pt,state);
	return(TRUE);
}

short BdScanHostQlt(LCUID plcid,short state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	POINTER		pt;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.SCAN_HOST");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);

	BdcastQlt(pt,state);
	
	return(TRUE);	
}

int BdCpuWkQlt(LCUID plcid,short status)
{
	char		devid,stnid;
	char		namebuf[40];
	char		stn_name[STATION_NAME_SIZE];
	char		dev_name[GROUP_NAME_SIZE];
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	POINTER		point;
	USHORT		state;

	devid=plcid.devid;
	stnid=plcid.stnid;
	re_time=time(&newtime);
	devtime=localtime(&newtime);

	if(-1==GetStnNameById(plcid.stnid, stn_name))
	{
		return -1;
	}
	if(-1==GetGroupNameById(plcid.stnid, plcid.devid, dev_name))
	{
		return -1;
	}
	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "SYS.CPUA_STA");
	
	if(-1==GetPtIdByNameStr(namebuf, &point))
	{
		return -1;
	}
	
	if(-1==GetIndPtStateById(&point, &state))
	{
		return -1;
	}
	
	BdcastQlt(point,status);

	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "SYS.CPUB_STA");
	
	if(-1==GetPtIdByNameStr(namebuf, &point))
	{
		return -1;
	}
	
	if(-1==GetIndPtStateById(&point, &state))
	{
		return -1;
	}
	
	BdcastQlt(point,status);

	return(TRUE);
}

int BdCpuStatQlt(LCUID plcid,short status)
{
	char		devid,stnid;
	char		namebuf[40];
	char		stn_name[STATION_NAME_SIZE];
	char		dev_name[GROUP_NAME_SIZE];
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	POINTER		point;
	USHORT		state;

	devid=plcid.devid;
	stnid=plcid.stnid;
	re_time=time(&newtime);
	devtime=localtime(&newtime);

	if(-1==GetStnNameById(plcid.stnid, stn_name))
	{
		return -1;
	}
	if(-1==GetGroupNameById(plcid.stnid, plcid.devid, dev_name))
	{
		return -1;
	}

	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "SYS.CPU_WK_STA");
	
	if(-1==GetPtIdByNameStr(namebuf, &point))
	{
		return -1;
	}
	
	if(-1==GetIndPtStateById(&point, &state))
	{
		return -1;
	}
	
	BdcastQlt(point,status);

	return(TRUE);
}

int BdNetWkQlt(LCUID plcid,short status)
{
	char		devid,stnid;
	char		namebuf[40];
	char		stn_name[STATION_NAME_SIZE];
	char		dev_name[GROUP_NAME_SIZE];
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	POINTER		point;
	USHORT		state;

	devid=plcid.devid;
	stnid=plcid.stnid;
	re_time=time(&newtime);
	devtime=localtime(&newtime);

	if(-1==GetStnNameById(plcid.stnid, stn_name))
	{
		return -1;
	}
	if(-1==GetGroupNameById(plcid.stnid, plcid.devid, dev_name))
	{
		return -1;
	}
	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "SYS.NET_WK_STA");
	
	if(-1==GetPtIdByNameStr(namebuf, &point))
	{
		return -1;
	}
	
	if(-1==GetIndPtStateById(&point, &state))
	{
		return -1;
	}
	
	BdcastQlt(point,status);

	return(TRUE);
}

short WriteScanHost(LCUID plcid,char *host_name)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	unsigned char hostid;
	POINTER		pt;
	unsigned short	status,state;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.SCAN_HOST");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	if( GetHostId( host_name,&hostid) == -1)  return(FALSE);	
	if(hostid > 15)
	{
		char log_str[256];

		sprintf(log_str,"WriteScanHost(): hostid =%d invalid",hostid);
		log_write(H9000_LOG_ERROR,1,log_str);
		return FALSE;		
	}
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);
	state = hostid;
	if( status != state)
	{
		WriteIndDBByStr(buf, state);
	}
	return(TRUE);	
}

short BdPlcMonHost(unsigned char stnid)
{
	char		tmp[100],buf[100];
	unsigned char srcid;
	POINTER		pt;
	unsigned char idx,devid;
	unsigned short	status,state;
	idx = index_stn[stnid];
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	devid = g_sys_lcu[idx];	
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.LCU_MON_HOST");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	GetSrcId(&srcid);
	if(srcid > 15)
	{
		char log_str[256];

		sprintf(log_str,"BdPlcMonHost(): srcid =%d invalid",srcid);
		log_write(H9000_LOG_ERROR,1,log_str);
		return FALSE;		
	}	
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);
	state = srcid;
	
	if( status != state)
	{
		BdcastMsg(stnid,devid,SYSMSG_TYPE,pt.pt_id,state);
	}
	return(TRUE);	
}

short BdHdbStat(short state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,idx,devid;
	POINTER		pt;
	unsigned short	status;
	stnid = 1;
	idx = index_stn[stnid];
	devid = g_sys_lcu[idx];
 	if(GetStnNameById(stnid,tmp) == -1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(FALSE);
	strcat(buf,tmp);
	strcat(buf,".SYS.HISTA_STA");
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);

	if( status != state )
	{
		BdcastMsg(stnid,devid,SYSMSG_TYPE,pt.pt_id,state);
	}
	
	return(TRUE);
}


int BdSms(char* sms)
{
	LAN_HEAD	head;
	time_t     	re_time;
	char 		smsbuf[SMS_MESSAGE_SIZE+6]; 
	unsigned short 		length;

	head.dest_id=0;
	GetLocalHostId((UCHAR	*)&head.src_id);
	head.dp_type=BJ_DB_TIME;
	head.length=SMS_MESSAGE_SIZE+6;
	
	memset(smsbuf,0,(SMS_MESSAGE_SIZE+6));
	
	time(&re_time);
#ifdef _BIG_ENDIAN_
	Swap4Byte(&re_time);
#endif	

	smsbuf[0]=DPS_SMS;
	smsbuf[1]=head.src_id;
	length =strlen(sms);	
	if(length>SMS_MESSAGE_SIZE)	
	{	
		printf("commonlib.c: sms length =%d too long, trim it \n",length);
		length=SMS_MESSAGE_SIZE;
	}
	else if(length <=0)
	{
		printf("commonlib.c: sms length =%d invalid \n",length);
		return FALSE;
	}
	memcpy(smsbuf+2,sms,length);
	memcpy(smsbuf + SMS_MESSAGE_SIZE+2, &re_time,4);	
#ifdef DEBUG	
	printf("commonlib.c: sms head.length=%d dp_type=%d\n",head.length ,head.dp_type);
	for(j=0;j<head.length;j++)
	{
		printf("%d",(unsigned char) smsbuf[j]);
		printf("=");
		if(j%8==7) printf(" ");
	}	
	printf("\n");
#endif	
	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&head.length);
#endif
	lan_out((char *)smsbuf,head,0);

	return TRUE;
}


int BdLms(short lms_id)
{
	short j;
	LAN_HEAD	head;
	time_t     	re_time;
	struct  tm      devtime;
	char 		lmsbuf[12]; 
	head.dest_id=0;
	GetLocalHostId((UCHAR	*)&head.src_id);
	head.dp_type=BJ_DB_TIME;
	head.length=12;
	
	memset(lmsbuf,0,12);
	
	
	lmsbuf[0]=DPS_LMS;
	lmsbuf[1]=head.src_id;
	memcpy(lmsbuf+2,&lms_id,2);
	lmsbuf[4]=head.src_id;
	
	time(&re_time);
	devtime=*localtime(&re_time);
	  lmsbuf[5]=devtime.tm_sec;
	  lmsbuf[6]=devtime.tm_min;
	  lmsbuf[7]=devtime.tm_hour;
	  lmsbuf[8]=devtime.tm_mday;
 	  lmsbuf[9]=devtime.tm_mon+1;
 	  j= devtime.tm_year+1900;
	memcpy(&lmsbuf[10], &j, 2); 
	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&head.length);
	Swap2Byte(&lmsbuf[2]);
	Swap2Byte(&lmsbuf[10]);
#endif
	lan_out((char *)lmsbuf,head,0);

	return TRUE;
}



void sig_term(int signo)
{
	printf ("signal INT or TRRM or KILL --,exit...\n");
	exit(-1);
}     

void GetTimeString(time_t tt,char* timestring)
{
 	char		tmpbuf[60];
	struct tm	*t;
	t=localtime (&tt);
	sprintf(tmpbuf,"%d-", t->tm_year+1900); /* year */
	strcpy(timestring,tmpbuf);
	sprintf(tmpbuf,"%d-", t->tm_mon+1); /*month */
	strcat(timestring,tmpbuf);
	sprintf(tmpbuf,"%d ", t->tm_mday); /*day */
	strcat(timestring,tmpbuf);
	sprintf(tmpbuf,"%d:", t->tm_hour); /*Hour  */
	strcat(timestring,tmpbuf);
	sprintf(tmpbuf,"%d:", t->tm_min); /*Minute  */
	strcat(timestring,tmpbuf);
	sprintf(tmpbuf,"%d", t->tm_sec); /*Sec */
	strcat(timestring,tmpbuf);
	/*printf("timestring=%s\n",timestring); */

}

int GetIPCConf()
{
	char FileName[100]="/home/ems/h9000/def/ipc_config.def";
	char	str[MAX_CHAR_NUM_IN_LINE];	
	char StrTmp[100];
	char hostname[40];
	char stnname[10];
	char devname[20];
	char mbpaddr_p[40];	
	char mbpaddr_s[40];
	unsigned char stnid;
	unsigned char devid;
	FILE *fp;
	unsigned char ipcid=0;
	if (( fp = fopen ( FileName,"r"))==NULL)
	{
		printf ("==== can't open (w) the file %s! \n",FileName );
		return FALSE;
	}
	else
	{
		/* printf ( "\n==== open (w) %s  success! \n",FileName  ); */
	}
	while ( fscanf(fp,"%s",StrTmp)!=EOF )
	{
		if(strcmp(StrTmp,"BEGIN")==0) break;
	}        

	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if( strlen(str)<10 )	
			continue;

		sscanf(str,"%s %s %s %s %s",
				hostname,  
				stnname ,
				devname ,
				mbpaddr_p,
				mbpaddr_s );
		if(GetStnIdByName( stnname, &stnid ) == -1 )
		{
			continue;
		}

		if(GetGroupIdByName( stnid, devname, &devid ) == -1 )
		{
			continue;
		}
		ipcid++;
		strcpy(ipcconf.seg[ipcid].hostname ,hostname); 
		strcpy(ipcconf.seg[ipcid].stnname  ,stnname );
		strcpy(ipcconf.seg[ipcid].devname  ,devname);
		strcpy(ipcconf.seg[ipcid].mbpaddr_p  ,mbpaddr_p);
		strcpy(ipcconf.seg[ipcid].mbpaddr_s  ,mbpaddr_s);
		ipcconf.seg[ipcid].stnid = stnid;
		ipcconf.seg[ipcid].devid = devid;
	}
	ipcconf.num = ipcid; 
	fclose(fp);
	return TRUE;
}

int GetIdByHostname(char *hname,unsigned char *p_stnid,unsigned char *p_devid)
{
	int i;
	for(i=1;i<=ipcconf.num;i++)
	{
		if(strcmp(ipcconf.seg[i].hostname,hname)==0) 
		{
			*p_stnid = ipcconf.seg[i].stnid;
			*p_devid = ipcconf.seg[i].devid;
			strcpy(g_mbpaddr,ipcconf.seg[i].mbpaddr_p); 
			return TRUE;
		}
	}
	return FALSE;
}

int	SetLogLevel(int argc, char *argv[])
{
	int		i, level;
	level= 0;	
	if(argc>2)
	{
		for(i=1; i<(argc-1); i++)
		{
			if(!strcmp(argv[i], "-errlog")  )
			{
				sscanf(argv[i+1], "%d", &level);
				break;
			}
		}
	}
	if(level>=3)
		level= 3;
	if(level < 1)
	{
		//SetErrLogMode(argc, argv);
	}
	g_loglevel = level;
	return level;
}
