/*******************************************************************************************************
FILENAME:	mbxapp.c
FUNCTION:	
	
	the source code file of MBX communication module,include:
	(1) int main();

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
 2005.11.1	start to modify the code for H9000 V4.0		yyp
********************************************************************************************************/

#include "../inc/mbx.h"

unsigned short	dbit_size[MAX_STN_NUM][MAX_LCU_NUM]; /* bitmap size*/
short	cmdptr[MAX_STN_NUM][MAX_LCU_NUM];
struct	plc_data_seg_head   plc_data_head;		/* head struct of plc data */
unsigned short	soeptr[MAX_STN_NUM][MAX_LCU_NUM];	/* soe pointer array */	
short		soeloopcount;				/* read soe loop times */
short		polloopcount;				/* read pol loop times */
short		pol_ptr_end;				/* pointer of pol package */
short		soealm_ptr_end; 			/* pointer of int Alarm */
short		FirstIntAlm;
short		FirstCmd;
short		firstrun;					/* first run of read data function  */
short		plctype;					/* plc type,GE or QUANTUM */
 

int			timeout;					/* alarm timeout : YES or NO */
struct		hostconf	plconf[MAX_STN_NUM+1][MAX_LCU_NUM+1];
struct		plcvar		runvar[MAX_STN_NUM+1][MAX_LCU_NUM+1];
char 		hostname[20];
#ifdef DIALOG_VER
 extern int IsStop ;
#else
 int IsStop =FALSE ;
#endif

 DWORD		ThreadID[MAX_STN_NUM+1][MAX_LCU_NUM+1][MAX_THR_NUM+1];
 HANDLE		hThreadID[MAX_STN_NUM+1][MAX_LCU_NUM+1][MAX_THR_NUM+1];
 WSADATA	wsaData;	

#ifdef DIALOG_VER
extern CString strLine;
extern CMbpscanDlg *pMbpscanDlg;
#endif
char tmpbuf[100];
char buffer[1200];
unsigned char SysReadReq[12] = {
    0, 0,    0, 0, 0, 6, 
    1, 0, 0, 0, 0, 0};

unsigned char SysWriteReq[13] = {
    0, 0,    0, 0, 0, 7,
    1, 0x10, 0, 0, 0, 0,0 };
char ncb_addr[MAX_STN_NUM][MAX_LCU_NUM][20] ;
unsigned int stn_num;
unsigned int dev_num[MAX_STN_NUM];
int	plc_cpu_num[MAX_STN_NUM][MAX_LCU_NUM],hot_cpu[MAX_STN_NUM][MAX_LCU_NUM],hot_net[MAX_STN_NUM][MAX_LCU_NUM];
int	POLTimeCount = 0;
void SetSysTimeByPOL(struct pol_data_seg polseg );
int stime(struct tm newtime);

#ifndef DIALOG_VER
void main()
{
	char		devname[12];
	unsigned char	stnid,devid,thrid;
#ifdef MSWIN32
	WORD   		wvr;
	WSADATA 	WSAData;
	int 		status;
	wvr=MAKEWORD(2,2);
	if ((status = WSAStartup(wvr, &WSAData)) != 0) 
	{
		printf("\nmbxapp.c:: %d is the err\n",status);
	}
#endif
	app_id = MBX_APP;
	printf("\n========================= mbxscan.dxe start running ==========================\n");

	gethostname(hostname,20);
	
	printf("hostname =%s\n",hostname);
	if(MountDB(DB_NAME) == -1)
	{
		printf("==== mbxapp.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	if(lan_init_all() == FALSE)
	{
		printf("==== mbxapp.c: call Lan_init_all() error\n");		
		exit(0);
	}

	if( InitPlcDataShm(1) == FALSE) 
	{
		printf("==== mbxapp.c: call initPlcDataShm(1) error\n");
		exit(0);
	}

	
	if(GetPlcConf(0)==0)	
	{
		printf("\n==== mbxapp.c: GetPlcConf() error!\n");
		exit(0);
	}
	else
	{
		printf("\n==== GetPlcConf() successfully!\n");
	}

	if(GetIPCConf()==0)	
	{
		printf("\n==== mbxapp.c: GetIPCConf() error!\n");
		exit(0);
	}
	else
	{
		printf("\n==== GetIPCConf() successfully!\n");
	}

	if(GetIdByHostname(hostname,&stnid,&devid)==0)	
	{
		printf("\n==== mbxapp.c: GetIdByHostname(%s) error!\n",hostname);
		exit(0);
	}
	else
	{
		printf("\n==== GetIdByHostname successfully, stnid=%d,devid=%d!\n",stnid,devid);
	}

	for(thrid=1;thrid<MAX_THR_NUM;thrid++)
	{
		ThreadID[stnid][devid][thrid] = 0;
	}		
	if(GetGroupNameById(stnid,devid,devname)==-1) return;

	if(plconf[stnid][devid].valid !=1 )	
	{	
		sprintf(log_str,"==== mbxapp.c: plc %d.%d valid =%d error! \n",stnid,devid,plconf[stnid][devid].valid); 
		log_write(ERR,1,log_str);
		return;
	}
	if(strcmp(devname,"")==0) 	return;	
	if( strstr(devname,"SYS")!=NULL ) return;
	runvar[stnid][devid].netcmdptr = shm_net_cmd->head;
	if( plconf[stnid][devid].plctype == MBPLC  || plconf[stnid][devid].plctype == PREPLC)
	{
		ChildProc(stnid,devid);	
	}

	for(;;)
	{
		MySleep(1000);
	}
	sprintf(log_str,"==== mbxapp.c: plc %d.%d scan main process exit! \n",stnid,devid); 
	log_printf(INF,1,MBX_APP,log_str);
#ifdef MSWIN32
	WSACleanup();
#endif	
	UnMountDB();
	return ;
}

#endif


/************************************************ Child Process function of each PLC ***********************************************************************************/
int ChildProc(unsigned char stnid,unsigned char devid)
{
	THRARG		arg[MAX_THR_NUM];
	unsigned char thrid;
	for(thrid=1;thrid<MAX_THR_NUM;thrid++ )
	{
		arg[thrid].stnid=stnid;
		arg[thrid].devid=devid;
	}
	/****** create the data scanning thread ******/
	MySleep(20);
	for(thrid=THR_HEAD;thrid<MAX_THR_NUM;thrid++)
	{
		MySleep(1000);

		arg[thrid].thrtype = THR_HEAD;
		/* MBXScanH(&arg[thrid]); */
		arg[thrid].thrtype = thrid;
		if(thrid == THR_HEAD)
		{
#ifdef MULTITHREAD
			hThreadID[stnid][devid][arg[thrid].thrtype]=CreateThread(NULL,
					MAX_STACK_SIZE, 
					(LPTHREAD_START_ROUTINE)MBXScanH, 
		 			&arg[thrid],
		 			0,
		 			(LPDWORD)&ThreadID[stnid][devid][arg[thrid].thrtype]);
#else
			MBXScanH(&arg[thrid]);
#endif 
		}
		else
		{
				if((thrid != THR_ANA) && (thrid != THR_INTALM))
				{
						continue;
				}
				hThreadID[stnid][devid][arg[thrid].thrtype]=CreateThread(NULL,
					MAX_STACK_SIZE, 
					(LPTHREAD_START_ROUTINE)MBXScan, 
		 			&arg[thrid],
		 			0,
		 			(LPDWORD)&ThreadID[stnid][devid][arg[thrid].thrtype]);
		}
		if(hThreadID[stnid][devid][arg[thrid].thrtype]!=NULL)
		{
			sprintf(log_str,"==== mbxapp.c: Create thread plc %d.%d,thrtype=%d,thr_id=%d OK",
				arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype,(int)ThreadID[stnid][devid][arg[thrid].thrtype]);	
			log_printf(INF,1,MBX_APP,log_str);
		}
		else
		{
			sprintf(log_str,"==== mbxapp.c: Create thread plc %d.%d, thrtype=%d failed", arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype);
			log_printf(ERR,1,MBX_APP,log_str);
		}
	}
			

	for(;;)
	{
#ifdef SUN
		pause();
#else
		MySleep(1000);
#endif
	}					
	return TRUE;
}

/********************************************************************************************************
 Read original data from plc and send data to plc by MBX，
 main function of MBXSCAN  
*********************************************************************************************************/
void * MBXScanH( void * arg )
{
	unsigned char	stnid=1,devid=1,tmphotnet,tmphotcpu;
	char		hostname[20];		/* hostname of local machine */
	char		plctypename[12];
	NCB *nd = (NCB *)NULL;
	LCUID		plcid;
	short		err_count=0;			/* error count of reading data function */
	short		loop_count=0;			/* for loop count */
	unsigned short hotnet;
	BOOL		ret[MAX_THR_NUM];				/* return value of  */
	int             FirstScanPrvgErr=1,FirstLinkStatErr=1,FirstHeadStatErr=1;
	short		OldScanPrvg,CurScanPrvg,OldLinkStat,CurLinkStat, OldHeadStat,CurHeadStat;	/* PLCScanPrvg of last loop,	current loop */
	sprintf(buffer,"\n================== MBXScanH start running ===================\n");
	printf(buffer);
	Sleep(50);

	plcid.stnid = ((THRARG *) arg)->stnid ;
	plcid.devid = ((THRARG *) arg)->devid ;	
	stnid=plcid.stnid;
	devid=plcid.devid; 
	/****** get plc type ******/
	GetPlcTypeStr(plcid.stnid,plcid.devid,plctypename);

	runvar[stnid][devid].hotcpu=rdb_cpu(stnid,devid);
	runvar[stnid][devid].hotnet=rdb_net(stnid,devid);
	runvar[stnid][devid].cpua_state = CPUERR;
	runvar[stnid][devid].cpub_state = CPUERR;
	runvar[stnid][devid].FirstHead = YES;
	runvar[stnid][devid].FirstSend = YES;
	runvar[stnid][devid].FirstIntAlm = YES;
	runvar[stnid][devid].FirstCmd = TRUE;
	runvar[stnid][devid].cpu_flag_count =0;
	runvar[stnid][devid].send_update_flag =0;
	runvar[stnid][devid].cpu_flag =MAX_PLC_FLAG;
	shm_plc_data->seg[ segid[stnid][devid] ].endpp=0;
	shm_plc_data->seg[ segid[stnid][devid] ].ana_update_flag=0;	
	shm_plc_data->seg[ segid[stnid][devid] ].rtd_update_flag=0;
	shm_plc_data->seg[ segid[stnid][devid] ].obj_update_flag=0;	
	shm_plc_data->seg[ segid[stnid][devid] ].imp_update_flag=0;				
	shm_plc_data->seg[ segid[stnid][devid] ].sysmsg_update_flag=0;
	shm_plc_data->seg[ segid[stnid][devid] ].head_thr_valid=NO; 
	CurScanPrvg=CUR_WS_IN_HOST;
	OldLinkStat=LCU_ON;
	CurHeadStat=NO;
	/******* loop always *******/
	for(;;)
	{
		if(IsStop == TRUE)
		{
			break;
		}
		MySleep(100);
		OldScanPrvg=CurScanPrvg;
		/*
		CurScanPrvg=GetPLCScanPrvg(plcid);
		*/
		CurScanPrvg = CUR_WS_IN_HOST;
		if( CurScanPrvg!=CUR_WS_IN_HOST )
		{
			if( OldScanPrvg==CUR_WS_IN_HOST )
			{
				shm_plc_data->seg[ segid[stnid][devid] ].endpp=0;
				shm_plc_data->seg[ segid[stnid][devid] ].head_thr_valid = NO; 
				BdPlcScanStat(plcid,SCANSTOP);
				sprintf(log_str,"==== %s is not the plc %d.%d primary scan server, Prvg = %d,oldprvg=%d\n", hostname,stnid,devid,CurScanPrvg,OldScanPrvg);
				log_printf(ERR,1,MBX_APP,log_str);
			}
			MySleep(500);
			continue;	
		}
		else
		{
			/* BdScanHost(plcid); */
		}	
		/******* read the plc link stat ,if unlink, then next loop *******/
		CurLinkStat=GetDevONOFFStat(plcid);
		if(CurLinkStat == LCU_OFF)
		{
			if( OldLinkStat != LCU_OFF )
			{				
				/* shm_plc_data->seg[ segid[stnid][devid] ].endpp=0; */
				shm_plc_data->seg[ segid[stnid][devid] ].head_thr_valid = NO; 
				BdPlcScanStat(plcid,SCANSTOP);
				sprintf(log_str,"==== Current Link state of plc %d.%d is offline \n",stnid,devid);
				log_printf(ERR,1,MBX_APP,log_str);	
				
			}
			OldLinkStat = LCU_OFF; 	
			MySleep(200);
			continue ;
		}
		tmphotnet=rdb_net(stnid,devid);
		tmphotcpu=rdb_cpu(stnid,devid);
		/******* if current net/cpu is not equal to last hotnet /hotcpu *******/
		if( runvar[stnid][devid].hotcpu !=tmphotcpu )
		{
			runvar[stnid][devid].hotnet = tmphotnet;
			runvar[stnid][devid].hotcpu = tmphotcpu;				
			/* shm_plc_data->seg[ segid[stnid][devid] ].endpp=0; */
			shm_plc_data->seg[ segid[stnid][devid] ].head_thr_valid = NO; 
			BdPlcScanStat(plcid,SCANSTOP);
			MySleep(200);
		}		
		/******* connect plc while not connected *******/
		if(nd == NULL)
		{
			nd=ConnectPLC(plcid);
			if(nd==NULL)
			{
				MySleep(2000);
				BdPlcLinkStat(plcid,LCU_ERR);
				continue;			
			}
		}
	
		ret[THR_HEAD]=GetHead(nd,plcid);
	
		if( ret[THR_HEAD]==YES )
		{
			/***** broadcast link state and set firstscan,only in GetHead thread *****/
			runvar[stnid][devid].FirstHead=NO;
			OldHeadStat=CurHeadStat;
			CurHeadStat=GetHeadStat(shm_plc_data->seg[ segid[stnid][devid] ].head,stnid,devid);
			if( CurHeadStat != YES )
			{
				/* hm_plc_data->seg[ segid[stnid][devid] ].endpp=0; */
				shm_plc_data->seg[ segid[stnid][devid] ].head_thr_valid = NO; 
				BdPlcScanStat(plcid,SCANSTOP);
				sprintf(log_str,"==== scanapp.c: plc %d.%d HeadStat invalid\n",stnid,devid);	
				log_printf(ERR,1,MBX_APP,log_str);
				MySleep(200);
			}
			BdPlcLinkStat(plcid,LCU_ON);
			err_count=0;
			loop_count++;
			if(loop_count>=200)
			{
				loop_count=0;
			}
			if(runvar[stnid][devid].cpu_flag == shm_plc_data->seg[ segid[stnid][devid] ].head.cpuflag)
			{
				runvar[stnid][devid].cpu_flag_count++;
				continue;
			}
			else
			{
				runvar[stnid][devid].cpu_flag = shm_plc_data->seg[ segid[stnid][devid] ].head.cpuflag;
				runvar[stnid][devid].cpu_flag_count = 0;
				BdPlcCpuStat(plcid,runvar[stnid][devid].hotcpu,CPURUN);
			}
			BdPlcScanStat(plcid,SCANRUN);		
			if(OldScanPrvg!=CUR_WS_IN_HOST || OldLinkStat !=LCU_ON || OldHeadStat != YES 
				|| shm_plc_data->seg[ segid[stnid][devid] ].head_thr_valid !=YES )
			{

				if(GetHeadStat(shm_plc_data->seg[ segid[stnid][devid] ].head, stnid,devid) == YES )
				{
					shm_plc_data->seg[ segid[stnid][devid] ].head_thr_valid=YES; 
					shm_plc_data->seg[ segid[stnid][devid] ].firstscan=1;
					shm_plc_data->seg[ segid[stnid][devid] ].endpp=1;
					
			  		sprintf(log_str,"==== scanapp: plc %d.%d set firstscan ,firstscan=%d,endpp=%d,head_valid=%d\n",
			  			stnid,devid, shm_plc_data->seg[ segid[stnid][devid] ].firstscan,
			  			shm_plc_data->seg[ segid[stnid][devid] ].endpp,
			  			shm_plc_data->seg[ segid[stnid][devid] ].head_thr_valid);
					log_printf(INF,1,MBX_APP,log_str);
				}
				else
				{
					sprintf(log_str,"==== scanapp.c: plc %d.%d GetHeadStat error\n",stnid,devid);	
					log_printf(ERR,1,MBX_APP,log_str);					
				}
				OldLinkStat =LCU_ON; 
			}
		}
		else if(ret[THR_HEAD]==NO)
		{
			err_count++;
			runvar[stnid][devid].cpu_flag_count = 0;
			DisConnectPLC(nd,plcid);
			nd = (NCB *)NULL;
			if(err_count>=MAXRETRY)
			{
				shm_plc_data->seg[ segid[stnid][devid] ].head_thr_valid=NO; 
				/* shm_plc_data->seg[ segid[stnid][devid] ].endpp=0; */
				BdPlcScanStat(plcid,SCANSTOP);
				OldLinkStat =LCU_ERR; 
			  	sprintf(log_str,"==== scanapp.c: stnid=%d,devid=%d set endpp=0 ret = NO \n",stnid,devid);	
				log_printf(INF,1,MBX_APP,log_str);	
				BdPlcLinkStat(plcid,LCU_ERR);
				if(runvar[stnid][devid].hotnet == 0)
				{
					hotnet = 1;
				}
				else
				{	
					hotnet = 0;
				}
				BdNetHost(plcid, hotnet );
						
				MySleep(2000);
				err_count=0;
			}
			continue;
		}
		else
		{
			continue;
		}
		/*
		GetPOL(nd,plcid);
		*/
	}/******* end or loop *******/
	/* release the resources */
#ifdef MSWIN32
	WSACleanup();
#endif	
	UnMountDB();
	sprintf(buffer,"\n================== MBXScan stop running ===================\n");
	myprintf(buffer);
	return FALSE;
}


void * MBXScan( void * arg )
{
	NCB *nd = (NCB *)NULL;			
	LCUID		plcid;
	short		err_count=0;			/* error count of reading data function */
	short		loop_count=0;			/* for loop count */
	BOOL		ret[MAX_THR_NUM];
	unsigned char	stnid,devid;
	unsigned char	threadtype;
	int		waitms;					
	short		CurScanPrvg,CurLinkStat, OldHeadStat,CurHeadStat;	/* PLCScanPrvg of last loop,	current loop */
	plcid.stnid = ((THRARG *) arg)->stnid ;
	plcid.devid = ((THRARG *) arg)->devid ;	
	stnid=plcid.stnid;
	devid=plcid.devid; 
	threadtype=((THRARG *) arg)->thrtype;
	MySleep(5000);
	sprintf(log_str,"==== scanapp.c: plc %d.%d Start ThreadScan Function!\n",stnid,devid);	
	log_printf(INF,1,MBX_APP,log_str);	
	CurScanPrvg=CUR_WS_IN_VICE;	
	CurHeadStat=YES;
	/******* loop always *******/
	sprintf(log_str,"==== scanapp.c stn=%d,dev=%d thr =%d: loop to read from plc ...\n",stnid,devid,threadtype);
	log_printf(INF,2,MBX_APP,log_str);	
	for(;;)
	{
		waitms=20;
		MySleep(waitms);
		if(threadtype == THR_ANA)
		{
			ret[threadtype]=MBXCmd(nd,plcid);
		}

		OldHeadStat=CurHeadStat;		
		CurHeadStat = shm_plc_data->seg[ segid[stnid][devid] ].head_thr_valid;
		if( CurHeadStat != YES)
		{	
			if(OldHeadStat == YES)
			{
				sprintf(log_str,"==== stn=%d,dev=%d thr =%d: head_thr is %d invalid ...",
					stnid,devid,threadtype,
					shm_plc_data->seg[ segid[stnid][devid] ].head_thr_valid);
				log_printf(ERR,2,MBX_APP,log_str);
			}	
			MySleep(500);
			continue;
		}

		/******* read the plc link stat ,if unlink, then next loop *******/
		CurLinkStat=GetDevONOFFStat(plcid);
		if(CurLinkStat == LCU_OFF)
		{
			MySleep(200);
			continue;
		}
		
		/******* connect plc while not connected *******/
		if(nd==NULL)
		{
			nd=ConnectPLC(plcid);
			if(nd==NULL)
			{
				MySleep(200);
				BdPlcLinkStat(plcid,LCU_ERR);
				continue;			
			}
		}
		/******* call different functions according to the threadtype *******/
		switch(threadtype)
		{
		case THR_ANA:
			ret[threadtype]= GetANA(nd,plcid);
			ret[threadtype]= ret[threadtype] && GetSysMsg(nd,plcid);
			ret[threadtype]= ret[threadtype] && GetPOL(nd,plcid);
			break;
		case THR_IMP:
			ret[threadtype]=GetIMP(nd,plcid);
			break;
		case THR_SYSMSG:
			ret[threadtype]=GetSysMsg(nd,plcid);
			break;
		case THR_POL:
			ret[threadtype]=GetPOL(nd,plcid);
			break;
		case THR_INTALM:
			ret[threadtype]=GetINTAlm(nd,plcid);
			ret[threadtype]= ret[threadtype] && WriteData(nd,plcid); 
			ret[threadtype]= ret[threadtype] && GetIMP(nd,plcid);
			break;
		case THR_WRITEDATA:
			/* ret[threadtype]=WriteData(nd,plcid); */
			break;
		case THR_CMD:
			ret[threadtype]=MBXCmd(nd,plcid);
			break;
		default:
			continue;
			break;
		}
		if(ret[threadtype]==TRUE)
		{
			if(threadtype==THR_INTALM)
			{
				runvar[stnid][devid].FirstIntAlm = 0;
			}
			err_count=0;
			loop_count++;
			if(loop_count>=200)
			{
				loop_count=0;
			}
		}
		else if(ret[threadtype]==FALSE)
		{
			err_count++;
			DisConnectPLC(nd,plcid);
			nd = (NCB *)NULL;
			if(err_count>=MAXRETRY)
			{
				err_count=0;
			}
			MySleep(2000);			
			continue;
		}
		else
		{
			MySleep(2000);
			continue;
		}
	}/******* end or loop *******/
	return NULL;
}

/********************************************************************************************************
 Write Command to plc
 main function of WriteCmd thread 
*********************************************************************************************************/
int MBXCmd(NCB * nd, LCUID plcid)
{
	short		loop_count=0;			/* for loop count */
	short		ret;				/* return value of  */
	unsigned char	stnid,devid,cmd_stnid,cmd_devid;
	short		CurCmdPrvg=CUR_CMD_VICE,OldCmdPrvg=CUR_CMD_VICE;	/* PLC Cmd Send Prvg of last loop,current loop */
	short		OldLinkStat= LCU_OFF,CurLinkStat= LCU_OFF;	
	int		waitms=0;	
	stnid = plcid.stnid;
	devid = plcid.devid;
	MySleep(100);
	/******* if current net/cpu is not equal to last hotnet /hotcpu *******/
	if( runvar[stnid][devid].hotcpu !=rdb_cpu(stnid,devid) )
	{
		runvar[stnid][devid].hotnet = rdb_net(stnid,devid);
		runvar[stnid][devid].hotcpu = rdb_cpu(stnid,devid);
		MySleep(200);
	}		
	if( runvar[stnid][devid].netcmdptr == shm_net_cmd->head )
	{
		MySleep(200);
		return TRUE;
	}

	/*******  if cur cmd  ptr doesn't belong to cur stnid and devid , then next cmd *******/
	cmd_stnid=shm_net_cmd->seg[runvar[stnid][devid].netcmdptr].net_cmd_buf.netdata.stn_id;
	cmd_devid=shm_net_cmd->seg[runvar[stnid][devid].netcmdptr].net_cmd_buf.netdata.dev_id;
	if(  cmd_stnid != stnid ||  cmd_devid  != devid )	
	{
		if(stnid==1 && devid==1)
		{
			if(cmd_stnid<1 && cmd_stnid > g_stn_num)
			{	
				sprintf(log_str,"==== mbxapp.c: net_cmd_buf[%d].netdata.stn_id =%d invalid",runvar[stnid][devid].netcmdptr, cmd_stnid);
				log_printf(ERR,1,MBXCMD_APP,log_str);				
			}
			else if(cmd_devid<1 && cmd_devid > g_dev_num[cmd_stnid])
			{
				sprintf(log_str,"==== mbxapp.c: net_cmd_buf[%d].netdata.dev_id =%d invalid",runvar[stnid][devid].netcmdptr, cmd_devid);
				log_printf(ERR,1,MBXCMD_APP,log_str);				
			}
		}
		runvar[stnid][devid].netcmdptr++;
		if(runvar[stnid][devid].netcmdptr>=MAX_NETCMD_SEG_NUM) 
		{
			runvar[stnid][devid].netcmdptr=0;
		}
						
		MySleep(200);
		return TRUE;
	}
	
	/******* if cur cmd  ptr belong to cur stnid and devid  then go ahead	*******************************/
	
	/******* read the plc link stat ,if linkoff and  not LCU_ONOFF cmd, then BdCmdErr and next loop *******/
	
	CurLinkStat=GetDevONOFFStat(plcid);
	if(CurLinkStat==LCU_OFF && shm_net_cmd->seg[runvar[stnid][devid].netcmdptr].net_cmd_buf.netdata.type_id != C_LCU_ONOFF)
	{
		OldLinkStat =LCU_ERR; 
		sprintf(log_str,"==== mbxapp.c: CurLinstat =%d stnid=%d,devid=%d next loop\n",CurLinkStat,stnid,devid);	
		log_printf(INF,1,MBXCMD_APP,log_str);
		sprintf(log_str,"LCU %d.%d 离线, 禁止操作!",stnid,devid);
		BdSms(log_str);
		
		/****** Bd cmd error and next cmd *****/
		BdCurCmdErr(plcid);						
		MySleep(2000);
		return TRUE;
	}

	if( nd == NULL && IsCmdSendPlc(plcid,shm_net_cmd->seg[runvar[stnid][devid].netcmdptr].net_cmd_buf.netdata.type_id) == TRUE )
	{
		BdPlcLinkStat(plcid,LCU_ERR);
		BdAllCmdErr(0,plcid);
		runvar[stnid][devid].netcmdptr=shm_net_cmd->head;
		MySleep(2000);
		return FALSE;
	}

	/************* call WriteCmd()  *****************/				
	ret=WriteCmd(nd,plcid);
	if(ret!=TRUE)
	{
		printf("mbxapp::  call WriteCmd error ret=%d, stnid=%d,devid=%d \n",ret,stnid,devid);
		BdAllCmdErr(1,plcid);
		runvar[stnid][devid].netcmdptr=shm_net_cmd->head;
		BdPlcLinkStat(plcid,LCU_ERR);
		return FALSE;		
	}

	return TRUE;
}


/********************************************************************************************************
 write cmd to plc
*********************************************************************************************************/

int WriteCmd(NCB * nd,LCUID plcid)
{
	struct plc_data_seg_head	head;
	short		err_count;				/* error count of reading data function */
	int 		seg;
	int             cmdret;
	int 		ref;
	int 		result;
	int  		scanlen;
	char		host;
	int j;
	unsigned char * pt;
	char		host_firstrun[2];
	unsigned	char databuf[MAX_SCAN_BUF*2]; 	
	unsigned char	stnid,devid,srcid,cmdbufid;
	unsigned short cmd_start_addr,cmd_size;
	unsigned short cmd_base_def,cmd_size_def;
	unsigned short		curptr,cmdprt;
	struct net_cmd_seg netcmdseg;
	struct plc_cmd_seg plccmdseg;
	stnid=plcid.stnid;
	devid=plcid.devid;
	err_count=0;
	
	if (nd==NULL && IsCmdSendPlc(plcid,
		shm_net_cmd->seg[runvar[stnid][devid].netcmdptr].net_cmd_buf.netdata.type_id) == TRUE )   
	{	
		printf("writecmd err: maybe nd=-1 ");
		return FALSE;
	}

	if(runvar[stnid][devid].netcmdptr == shm_net_cmd->head)
	{
		return TRUE;
	}

	if(plconf[stnid][devid].plctype==SIEPLC)
	{	
		cmd_start_addr = CMDBASE_SIE;
		cmd_size = CMDSIZE_SIE;
		cmd_base_def = CMDBASE_SIE;
		cmd_size_def = CMDSIZE_SIE;
	}		
	else if(plconf[stnid][devid].plctype==MBPLC
		|| plconf[stnid][devid].plctype==PREPLC)
	{
		cmd_start_addr = CMDBASE_MB;
		cmd_size = CMDSIZE_MB;
		cmd_base_def = CMDBASE_MB;
		cmd_size_def = CMDSIZE_MB;
	}
	else if(plconf[stnid][devid].plctype==GEPLC)
	{
		cmd_start_addr = CMDBASE_GE;
		cmd_size = CMDSIZE_GE;
		cmd_base_def = CMDBASE_GE;
		cmd_size_def = CMDSIZE_GE;
	}
	else
	{
		sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,plcid.devid,plconf[stnid][devid].plctype);
		log_printf(ERR,1,PLC_APP,log_str);
		return(FALSE);
	}
	
	/***************** Read Head Data From PLC **************************************************/
	if(runvar[stnid][devid].FirstCmd==1 && IsCmdSendPlc(plcid,
		shm_net_cmd->seg[runvar[stnid][devid].netcmdptr].net_cmd_buf.netdata.type_id) == TRUE )
	{
		memset(databuf,0,MAX_SCAN_BUF*2);
			if(plconf[stnid][devid].plctype==SIEPLC)
			{	
				seg = HEADDB_NO;
				ref = HEADBASE_SIE/2;
			}		
			else if(plconf[stnid][devid].plctype==MBPLC
				|| plconf[stnid][devid].plctype==PREPLC)
			{
				seg=REGSEG;
				ref=HEADBASE_MB;
			}
			else if(plconf[stnid][devid].plctype==GEPLC)
			{
				seg=REGSEG;
				ref=HEADBASE_GE;
			}
			else
			{
				sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,plcid.devid,plconf[stnid][devid].plctype);
				log_printf(ERR,1,PLC_APP,log_str);
				return(FALSE);
			}			

		scanlen=(PLCHEADLEN+1)/2;
		result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf,plcid,READ_4X);
		if(result==FALSE)
		{
			sprintf(log_str,"==== mbxapp.c: stnid=%d,devid=%d WriteCmd(): ReadPLC error \n",stnid,devid);	
			log_printf(ERR,1,MBXCMD_APP,log_str);
			return(FALSE);
		}
		memcpy(&head,databuf,PLCHEADLEN);
#ifdef _BIG_ENDIAN_
		Swap2Head( &head );
#endif
		memcpy(&host_firstrun,&head.host_firstrun,2);
#ifdef _BIG_ENDIAN_
		host = host_firstrun[0];
#else
		host = host_firstrun[1];
#endif
		if((host == 1) || (host == 3) ) 
		{
			host =1;
		}
		else
		{
			host=0;
		}
		cmd_start_addr=head.cmd_start_addr;
		cmd_size = head.cmd_size;
		if( cmd_start_addr != cmd_base_def || cmd_size !=cmd_size_def)
		{
			sprintf(log_str,"==== mbxapp.c: stnid=%d devid=%d cmd_start_addr =%d or cmd_size =%d invalid\n",
				stnid,devid, head.cmd_start_addr, head.cmd_size);
			log_printf(ERR,1,MBXCMD_APP,log_str);
			return(FALSE);
		}
		if(runvar[stnid][devid].hotcpu!=host)
		{
			sprintf(log_str,"==== mbxapp.c: stnid=%d,devid=%d error, WriteCmd():hotcpu =%d differ from host =%d in plc!\n",
				stnid,devid,runvar[stnid][devid].hotcpu,host);
			log_printf(ERR,1,MBXCMD_APP,log_str);
			runvar[stnid][devid].hotcpu=host;
			BdCpuHost(plcid, runvar[stnid][devid].hotcpu);
			return(FALSE);
		}
		runvar[stnid][devid].FirstCmd=0;
	}

	/************ write  command ******************************************/
	curptr =  runvar[stnid][devid].netcmdptr;

	while( curptr != shm_net_cmd->head )
	{	
		netcmdseg = shm_net_cmd->seg[curptr];
		if(netcmdseg.net_cmd_buf.netdata.stn_id==stnid 
			&& netcmdseg.net_cmd_buf.netdata.dev_id==devid)
		{

#ifdef _BIG_ENDIAN_
			Swap2Byte( &(netcmdseg.lan_head.length));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.point_id));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.status));
			Swap4Byte( &(netcmdseg.net_cmd_buf.netdata.data));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.ctrl_addr));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.year));
#endif
			/*************** analyze one net cmd ******************/
			memset(&plccmdseg,0,sizeof(plccmdseg));

			if((cmdret=CmdProcess(&netcmdseg,&plccmdseg)) == 1 )
			{
#ifdef _BIG_ENDIAN_			
				for(i=0;i<7;i++)
				{
					Swap2Byte( &(plccmdseg.cmd_buf.cmd.cmd_w[i]) );
				}	

#endif				
				/*************** send one cmd to plc ******************/
				seg=REGSEG;
				if(plconf[stnid][devid].plctype==SIEPLC)
				{	
					seg=CMDDB_NO;
				}
				srcid = plccmdseg.cmd_buf.cmd.src_id;
				/*
				cmdbufid = GetCmdBufId(srcid);
				*/
				cmdbufid =(unsigned char) cmd_size_def;
				if(cmdbufid < 1 || cmdbufid > cmd_size_def )
				{
					sprintf(log_str,"==== mbxapp.c: stnid=%d devid=%d cmdbufid =%d invalid\n",stnid,devid,cmdbufid);
					log_printf(ERR,1,MBXCMD_APP,log_str);
					BdCmdSendResult(netcmdseg, CMD_SENDERR);
					return(FALSE);
				}
				if(plconf[stnid][devid].plctype==SIEPLC)
				{					
					ref = cmd_base_def/2 + (cmdbufid-1)  * PLCCMD_BUF_SIZE;	
				}
				else
				{
					ref = cmd_base_def + (cmdbufid-1)  * PLCCMD_BUF_SIZE;					
				}
				scanlen = PLCCMD_BUF_SIZE;
				if( nd ==NULL ) 
				{
					sprintf(log_str,"==== mbxapp.c stnid=%d,devid=%d,bufnum=%d  write cmd error ====\n",
						stnid,devid,curptr);
					log_printf(ERR,1,MBXCMD_APP,log_str);
					BdCmdSendResult(netcmdseg, CMD_SENDERR);
					curptr++;
					if(curptr>=MAX_NETCMD_SEG_NUM) 
					{
						curptr=0;
					}
					runvar[stnid][devid].netcmdptr = curptr;					
					return(FALSE);					
				}
				result = WritePLC(nd, (unsigned char)seg, ref, PLCCMD_BUF_SIZE,
						(unsigned char *)&(plccmdseg.cmd_buf.cmd), plcid);
				if(result==FALSE)
				{
					sprintf(log_str,"==== mbxapp.c stnid=%d,devid=%d,bufnum=%d  write cmd error ====\n",stnid,devid,curptr);
					log_printf(ERR,1,MBXCMD_APP,log_str);
					BdCmdSendResult(netcmdseg, CMD_SENDERR);
					curptr++;
					if(curptr>=MAX_NETCMD_SEG_NUM) 
					{
						curptr=0;
					}
					runvar[stnid][devid].netcmdptr = curptr;					
					return(FALSE);
				}
				else
				{
					sprintf(log_str,"====  mbxapp.c stnid=%d,devid=%d,bufnum=%d write cmd ok ====\n ",
						stnid,devid,runvar[stnid][devid].netcmdptr);
					log_printf(INF,1,MBXCMD_APP,log_str);
					BdCmdSendResult(netcmdseg, CMD_SUCCESS);
					BdPlcLinkStat(plcid,LCU_ON);
					pt = (unsigned char *) &(plccmdseg.cmd_buf.cmd);
					printf("cmd buf:  ");
					for(j=0;j<PLCCMD_BUF_SIZE*2;j++)
					{
						printf("%02x=", (unsigned char )*(pt +j));
	  
						  if(j==5)
						  {
						   printf("   ");
						  }
						  
						  if(j==11)
						  {
						   printf("   ");
						  }
						  
					}
					printf("\n\n");
					curptr++;
					if(curptr>=MAX_NETCMD_SEG_NUM) 
					{
						curptr=0;
					}
					runvar[stnid][devid].netcmdptr = curptr;
					
					cmdprt = shm_plc_cmd->head;
					if(cmdprt>=MAX_PLCCMD_SEG_NUM) 
					{
						cmdprt=0;
					}	
					shm_plc_cmd->seg[cmdprt] = plccmdseg;
					cmdprt++;
					shm_plc_cmd->head =  cmdprt ;					
				}
			}
			else if(cmdret == 0)
			{
				curptr++;
				if(curptr>=MAX_NETCMD_SEG_NUM) 
				{
					curptr=0;
				}
				runvar[stnid][devid].netcmdptr = curptr;
				sprintf(log_str,"====  mbxapp.c plc%d.%d CmdProcess return %d OK\n",stnid,devid,cmdret);
				log_printf(INF,1,MBXCMD_APP,log_str);
				return(TRUE);
			}
			else
			{
				curptr++;
				if(curptr>=MAX_NETCMD_SEG_NUM) 
				{
					curptr=0;
				}
				runvar[stnid][devid].netcmdptr = curptr;
				sprintf(log_str,"====  mbxapp.c plc%d.%d CmdProcess return %d Error\n",stnid,devid,cmdret);
				log_printf(ERR,1,MBXCMD_APP,log_str);
				return(TRUE);				
			}
			/**** end of if ****/
		}
		else
		{
			curptr++;
			if(curptr>=MAX_NETCMD_SEG_NUM) 
			{
				curptr=0;
			}
			runvar[stnid][devid].netcmdptr = curptr;
		}/**** end of if ****/
		
	} /*** end of while ***/
	return(TRUE);
}



/********************************************************************************************************
 read plc head data
*********************************************************************************************************/

int GetHead(NCB * nd,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	struct plc_data_seg_head head;
	unsigned	char databuf[MAX_SCAN_BUF*2];  
	unsigned char	stnid,devid;
	stnid=plcid.stnid;
	devid=plcid.devid;
	if (nd==NULL)   return FALSE;


	if(plconf[stnid][devid].plctype==SIEPLC)
	{	
		seg = HEADDB_NO;
		ref = HEADBASE_SIE/2;
	}		
	else if(plconf[stnid][devid].plctype==MBPLC
		|| plconf[stnid][devid].plctype==PREPLC)
	{
		seg=REGSEG;
		ref=HEADBASE_MB;
	}
	else if(plconf[stnid][devid].plctype==GEPLC)
	{
		seg=REGSEG;
		ref=HEADBASE_GE;
	}
	else
	{
		sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,plcid.devid,plconf[stnid][devid].plctype);
		log_printf(ERR,1,PLC_APP,log_str);
		return(FALSE);
	}


	/***************** Read Head Data From PLC **************************************************/
	memset(databuf,0,MAX_SCAN_BUF*2);
	scanlen=(PLCHEADLEN+1)/2;
	result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf,plcid,READ_4X); 
	if(result==FALSE)
	{
		sprintf(log_str,"==== scanapp.c: plc %d.%d GetHead(): ReadPLC error \n",stnid,devid);
		log_printf(ERR,1,MBX_APP,log_str);	
		return(FALSE);
	}
	/*****************decode original ana data **************************************************/
	memcpy(&head,databuf,2*scanlen);
#ifdef _BIG_ENDIAN_
	Swap2Head( &head );
#endif
	memcpy(&(shm_plc_data->seg[ segid[stnid][devid] ].head),&head,2*scanlen);
	return TRUE;
}

/********************************************************************************************************
 read plc analog data
*********************************************************************************************************/

int GetANA(NCB * nd,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	int 		offlen=0;
	struct plc_data_seg_head head;
	unsigned	char databuf[MAX_SCAN_BUF*2];  
	unsigned char	stnid,devid;
	stnid=plcid.stnid;
	devid=plcid.devid;

	if (nd==NULL)   return FALSE;

	if(plconf[stnid][devid].plctype==SIEPLC)
	{	
		seg = ANADB_NO;
		ref = ANABASE_SIE/2;
	}		
	else if(plconf[stnid][devid].plctype==MBPLC
		|| plconf[stnid][devid].plctype==PREPLC)
	{
		seg=REGSEG;
		ref=ANABASE_MB;
	}
	else if(plconf[stnid][devid].plctype==GEPLC)
	{
		seg=REGSEG;
		ref=ANABASE_GE;
	}
	else
	{
		sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,plcid.devid,
			plconf[stnid][devid].plctype);
		log_printf(ERR,1,PLC_APP,log_str);
		return(FALSE);
	}

	/***************** get shm data head **************************************************/
	memcpy(&head,&(shm_plc_data->seg[ segid[stnid][devid] ].head),PLCHEADLEN);
	/***************** Read Ana Data From PLC **************************************************/
	memset(databuf,0,MAX_SCAN_BUF*2);
	scanlen=0;
	scanlen+=head.ai_size;
	scanlen+=2*Mod16(head.ai_size);	
	scanlen+=head.ao_size;
	scanlen+=2*Mod16(head.ao_size);	
	scanlen+=head.aic_size;
	scanlen+=head.aicomm_size;
	scanlen+=2*Mod16(head.aicomm_size);				
	result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid,READ_4X);
	if(result==FALSE)
	{
		sprintf(log_str,"==== scanapp.c: plc %d.%d GetANA(): ReadPLC error \n",stnid,devid);
		log_printf(ERR,1,MBX_APP,log_str);	
		return(FALSE);
	}
	
	/*****************decode original ana data **************************************************/
	offlen=0;
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].ai_data,databuf+offlen,2*head.ai_size);
	offlen+=2*head.ai_size;
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].ai_qlt,databuf+offlen,2*Mod16(head.ai_size));
	offlen+=2*Mod16(head.ai_size);
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].ai_en,databuf+offlen,2*Mod16(head.ai_size));	
	offlen+=2*Mod16(head.ai_size);		

	/*****************decode original aout data **************************************************/
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].ao_data,databuf+offlen,2*head.ao_size);
	offlen+=2*head.ao_size;
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].ao_qlt,databuf+offlen,2*Mod16(head.ao_size));
	offlen+=2*Mod16(head.ao_size);
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].ao_en,databuf+offlen,2*Mod16(head.ao_size));	
	offlen+=2*Mod16(head.ao_size);	

	/*****************decode original ana calculate data ****************************************/
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].aic_data,databuf+offlen,2*head.aic_size);
	offlen+=2*head.aic_size;
	
	/*****************decode original ana communication data ************************************/
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].aicomm_data,databuf+offlen,2*head.aicomm_size);
	offlen+=2*head.aicomm_size;
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].aicomm_qlt,databuf+offlen,2*Mod16(head.aicomm_size));
	offlen+=2*Mod16(head.aicomm_size);
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].aicomm_en,databuf+offlen,2*Mod16(head.aicomm_size));	
	offlen+=2*Mod16(head.aicomm_size);	
	
	shm_plc_data->seg[ segid[stnid][devid] ].ana_update_flag++;
	if(shm_plc_data->seg[ segid[stnid][devid] ].ana_update_flag>=MAX_FLAG_NUM)
	{
		shm_plc_data->seg[ segid[stnid][devid] ].ana_update_flag=0;
	}	
	return TRUE;
}

/********************************************************************************************************
 read plc IMP RTD PARAM data
*********************************************************************************************************/

int GetIMP(NCB * nd,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	int 		offlen=0;
	struct plc_data_seg_head head;
	unsigned	char databuf[MAX_SCAN_BUF*2];  
	unsigned char	stnid,devid;
	stnid=plcid.stnid;
	devid=plcid.devid;
	if (nd==NULL)   return FALSE;

	/***************** get shm data head **************************************************/
	memcpy(&head,&(shm_plc_data->seg[ segid[stnid][devid] ].head),PLCHEADLEN);
	

	/***************** Read Obj Data From PLC **************************************************/
	if( head.obj_size>0 )
	{	
		memset(databuf,0,MAX_SCAN_BUF*2);
		if(plconf[stnid][devid].plctype==SIEPLC)
		{	
			seg = OBJDB_NO;
			ref = OBJBASE_SIE/2;
		}		
		else if(plconf[stnid][devid].plctype==MBPLC
			|| plconf[stnid][devid].plctype==PREPLC)
		{
			seg=REGSEG;
			ref=OBJBASE_MB;
		}
		else if(plconf[stnid][devid].plctype==GEPLC)
		{
			seg=REGSEG;
			ref=OBJBASE_GE;
		}
		else
		{
			sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,plcid.devid,plconf[stnid][devid].plctype);
			log_printf(ERR,1,PLC_APP,log_str);
			return(FALSE);
		}
		scanlen=0;
		scanlen+=3*head.obj_size;
		result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid,READ_4X);
		if(result==FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d GetIMP(): ReadPLC OBJ error......\n",stnid,devid);
			log_printf(ERR,1,MBX_APP,log_str);	
			return(FALSE);
		}
		/*****************decode obj data **************************************************/
		offlen=0;
		memcpy(shm_plc_data->seg[ segid[stnid][devid] ].obj_data,databuf,2*scanlen);

		shm_plc_data->seg[ segid[stnid][devid] ].obj_update_flag++;
		if(shm_plc_data->seg[ segid[stnid][devid] ].obj_update_flag>=MAX_FLAG_NUM)
		{
			shm_plc_data->seg[ segid[stnid][devid] ].obj_update_flag=0;
		}
	}
	/***************** Read RTD Data From PLC **************************************************/
	if( head.rtd_size>0 )
	{
		memset(databuf,0,MAX_SCAN_BUF*2);
		seg=REGSEG;
		ref=head.rtd_start_addr;
		scanlen=0;
		scanlen+=head.rtd_size;
		scanlen+=2*Mod16(head.rtd_size);	
		result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid,READ_4X);
		if(result==FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d GetIMP(): ReadPLC RTD error......\n",stnid,devid);
			log_printf(ERR,1,MBX_APP,log_str);	
			return(FALSE);
		}

		/*****************decode original RTD data **************************************************/
		offlen=0;
		memcpy(shm_plc_data->seg[ segid[stnid][devid] ].rtd_data,databuf,2*head.rtd_size);
		offlen+=2*head.rtd_size;
		memcpy(shm_plc_data->seg[ segid[stnid][devid] ].rtd_qlt,databuf+offlen,2*Mod16(head.rtd_size));
		offlen+=2*Mod16(head.rtd_size);
		memcpy(shm_plc_data->seg[ segid[stnid][devid] ].rtd_en,databuf+offlen,2*Mod16(head.rtd_size));	
		shm_plc_data->seg[ segid[stnid][devid] ].rtd_update_flag++;
		if(shm_plc_data->seg[ segid[stnid][devid] ].rtd_update_flag>=MAX_FLAG_NUM)
		{
			shm_plc_data->seg[ segid[stnid][devid] ].rtd_update_flag=0;
		}
	}

	/***************** Read IMP and IMP_COMM  Data From PLC **************************************************/
	if( head.imp_size>0 || head.impcomm_size>0 )
	{
		memset(databuf,0,MAX_SCAN_BUF*2);
		seg=REGSEG;
		ref=head.imp_start_addr;
		scanlen=0;
		scanlen+=head.imp_size *2;
		scanlen+=1*Mod16(head.imp_size);
		scanlen+=head.impcomm_size*2;
		scanlen+=1*Mod16(head.impcomm_size);		
		result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid,READ_4X);
		if(result==FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d GetIMP(): ReadPLC IMP error......\n",stnid,devid);
			log_printf(ERR,1,MBX_APP,log_str);	
			return(FALSE);
		}

		/*****************decode original IMP and IMP_COMM data **************************************************/
		offlen=0;
		memcpy(shm_plc_data->seg[ segid[stnid][devid] ].pi_data,databuf+offlen,4*head.imp_size);
		offlen+=4*head.imp_size;
		memcpy(shm_plc_data->seg[ segid[stnid][devid] ].pi_qlt,databuf+offlen,2*Mod16(head.imp_size));
		offlen+=2*Mod16(head.imp_size);

		memcpy(shm_plc_data->seg[ segid[stnid][devid] ].picomm_data,databuf+offlen,4*head.impcomm_size);
		offlen+=4*head.impcomm_size;
		memcpy(shm_plc_data->seg[ segid[stnid][devid] ].picomm_qlt,databuf+offlen,2*Mod16(head.impcomm_size));

		shm_plc_data->seg[ segid[stnid][devid] ].imp_update_flag++;
		if(shm_plc_data->seg[ segid[stnid][devid] ].imp_update_flag>=MAX_FLAG_NUM)
		{
			shm_plc_data->seg[ segid[stnid][devid] ].imp_update_flag=0;
		}	
	}
	/***************** Read Param Data From PLC and decode when updated *****************************/
	if(head.param_update_flag==PARAMFLAGPLC && head.param_size>0)
	{
		memset(databuf,0,MAX_SCAN_BUF*2);
		if(plconf[stnid][devid].plctype==SIEPLC)
		{	
			seg = PAMDB_NO;
			ref = PARAMBASE_SIE/2;
		}		
		else if(plconf[stnid][devid].plctype==MBPLC
			|| plconf[stnid][devid].plctype==PREPLC)
		{
			seg=REGSEG;
			ref=PARAMBASE_MB;
		}
		else if(plconf[stnid][devid].plctype==GEPLC)
		{
			seg=REGSEG;
			ref=PARAMBASE_GE;
		}
		else
		{
			sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,plcid.devid,plconf[stnid][devid].plctype);
			log_printf(ERR,1,PLC_APP,log_str);
			return(FALSE);
		}
		scanlen=0;
		/* removed by yyp 06.08.09
		scanlen+=3;
		*/
		scanlen+=head.param_size;
		result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid,READ_4X);
		if(result==FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d GetIMP(): ReadPLC Param error......\n",stnid,devid);
			log_printf(ERR,1,MBX_APP,log_str);	
			return(FALSE);
		}
		memcpy(shm_plc_data->seg[ segid[stnid][devid] ].param_data,databuf,2*scanlen);
		/*
		memcpy(&(shm_plc_data->seg[ segid[stnid][devid] ].param_flag),databuf,2*scanlen);
#ifdef _BIG_ENDIAN_
		Swap2Byte(&(shm_plc_data->seg[ segid[stnid][devid] ].param_flag));
		Swap2Byte(&(shm_plc_data->seg[ segid[stnid][devid] ].param_size));
		Swap2Byte(&(shm_plc_data->seg[ segid[stnid][devid] ].param_off));
#endif		
		*/
		
		shm_plc_data->seg[ segid[stnid][devid] ].param_update_flag++;
		if(shm_plc_data->seg[ segid[stnid][devid] ].param_update_flag>=MAX_FLAG_NUM)
		{
			shm_plc_data->seg[ segid[stnid][devid] ].param_update_flag=0;
		}
	}

	return TRUE;
}

/********************************************************************************************************
 read plc SysMsg data
*********************************************************************************************************/

int GetSysMsg(NCB * nd,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	int 		offlen=0;	
	struct plc_data_seg_head head;
	unsigned	char databuf[MAX_SCAN_BUF*2];  
	unsigned char	stnid,devid;
	stnid=plcid.stnid;
	devid=plcid.devid;
	if (nd==NULL)   return FALSE;

	/***************** get shm data head **************************************************/
	memcpy(&head,&(shm_plc_data->seg[ segid[stnid][devid] ].head),PLCHEADLEN);
	/***************** Read Soe  Data From PLC **************************************************/
	if( head.soe_size>0 )
	{
		memset(databuf,0,MAX_SCAN_BUF*2);
		seg=REGSEG;
		ref=head.soe_start_addr;
		scanlen=0;
		scanlen+=3*Mod16(head.soe_size);	
		result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid,READ_4X);
		if(result==FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d GetSysMsg(): ReadPLC error......\n",stnid,devid);
			log_printf(ERR,1,MBX_APP,log_str);	
			return(FALSE);
		}
		/*****************decode original soe  data **************************************************/
		offlen=0;
		memcpy(shm_plc_data->seg[ segid[stnid][devid] ].soe_data,databuf,2*Mod16(head.soe_size));
		offlen+=2*Mod16(head.soe_size);
		memcpy(shm_plc_data->seg[ segid[stnid][devid] ].soe_qlt,databuf+offlen,2*Mod16(head.soe_size));
		offlen+=2*Mod16(head.soe_size);
		memcpy(shm_plc_data->seg[ segid[stnid][devid] ].soe_en,databuf+offlen,2*Mod16(head.soe_size));
	}
	/***************** Read SysMsg  Data From PLC **************************************************/
	if( head.msg_size>0 )
	{	
		memset(databuf,0,MAX_SCAN_BUF*2);
		seg=REGSEG;
		ref=head.msg_start_addr;
		scanlen=0;
		scanlen+=1*Mod16(head.msg_size);	
		result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid,READ_4X);
		if(result==FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d GetSysMsg(): ReadPLC error......\n",stnid,devid);
			log_printf(ERR,1,MBX_APP,log_str);	
			return(FALSE);
		}
		/*****************decode original SysMsg  data **************************************************/
		offlen=0;
		memcpy(shm_plc_data->seg[ segid[stnid][devid] ].msg_data,databuf,2*Mod16(head.msg_size));

		shm_plc_data->seg[ segid[stnid][devid] ].sysmsg_update_flag++;
		if(shm_plc_data->seg[ segid[stnid][devid] ].sysmsg_update_flag>=MAX_FLAG_NUM)
		{
			shm_plc_data->seg[ segid[stnid][devid] ].sysmsg_update_flag=0;
		}
	}
	return TRUE;
}

/********************************************************************************************************
 read plc POL data
*********************************************************************************************************/

int GetPOL(NCB * nd,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	int 		offlen=0;
	short		pol_ptr_end;
	struct plc_data_seg_head head;
	unsigned	char databuf[MAX_SCAN_BUF*2];  
	unsigned char	stnid,devid;
	stnid=plcid.stnid;
	devid=plcid.devid;
	if (nd==NULL)   return FALSE;
	pol_ptr_end = runvar[stnid][devid].pol_ptr_end ;
	/***************** get shm data head and pol pointer ******************************************/
	memcpy(&head,&(shm_plc_data->seg[ segid[stnid][devid] ].head),PLCHEADLEN);
		
	/***************** Read POL Status From PLC and decode ********************************/
	memset(databuf,0,MAX_SCAN_BUF*2);
	if(plconf[stnid][devid].plctype==SIEPLC)
	{	
		seg = POLDB_NO;
		ref = SWBASE_SIE/2;
	}		
	else if(plconf[stnid][devid].plctype==MBPLC
		|| plconf[stnid][devid].plctype==PREPLC)
	{
		seg=REGSEG;
		ref=SWBASE_MB;
	}
	else if(plconf[stnid][devid].plctype==GEPLC)
	{
		seg=REGSEG;
		ref=head.pol_start_addr;
	}
	else
	{
		sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,plcid.devid,plconf[stnid][devid].plctype);
		log_printf(ERR,1,PLC_APP,log_str);
		return(FALSE);
	}
	scanlen=7;
	scanlen+=3*Mod16(head.pol_size);
	scanlen+=3*Mod16(head.dout_size);
	scanlen+=1*Mod16(head.polc_size);
	scanlen+=3*Mod16(head.polcomm_size);
			
	result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid, READ_0X);
	if(result==FALSE)
	{
		sprintf(log_str,"==== scanapp.c: plc %d.%d GetPOL(): ReadPLC error......\n",stnid,devid);
		log_printf(ERR,1,MBX_APP,log_str);	
		return(FALSE);
	}
       
	/*****************decode original POL  data **************************************************/
	offlen=0;
	memcpy(&(shm_plc_data->seg[ segid[stnid][devid] ].pol_data[pol_ptr_end].backup),databuf+offlen,14);
#ifdef _BIG_ENDIAN_
	Swap2Str((unsigned char *) &(shm_plc_data->seg[ segid[stnid][devid] ].pol_data[pol_ptr_end].backup),7);
#endif
	offlen+=14;
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].pol_data[pol_ptr_end].pol_data,databuf+offlen,2*Mod16(head.pol_size));
	offlen+=2*Mod16(head.pol_size);
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].pol_data[pol_ptr_end].dout_data,databuf+offlen,2*Mod16(head.dout_size));
	offlen+=2*Mod16(head.dout_size);	
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].pol_data[pol_ptr_end].pol_qlt,databuf+offlen,2*Mod16(head.pol_size));
	offlen+=2*Mod16(head.pol_size);
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].pol_data[pol_ptr_end].pol_en,databuf+offlen,2*Mod16(head.pol_size));
	offlen+=2*Mod16(head.pol_size);			
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].pol_data[pol_ptr_end].dout_qlt,databuf+offlen,2*Mod16(head.dout_size));
	offlen+=2*Mod16(head.dout_size);
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].pol_data[pol_ptr_end].dout_en,databuf+offlen,2*Mod16(head.dout_size));
	offlen+=2*Mod16(head.dout_size);
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].pol_data[pol_ptr_end].polc_data,databuf+offlen,2*Mod16(head.polc_size));
	offlen+=2*Mod16(head.polc_size);
	
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].pol_data[pol_ptr_end].polcomm_data,databuf+offlen,2*Mod16(head.polcomm_size));
	offlen+=2*Mod16(head.polcomm_size);
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].pol_data[pol_ptr_end].polcomm_qlt,databuf+offlen,2*Mod16(head.polcomm_size));
	offlen+=2*Mod16(head.polcomm_size);
	memcpy(shm_plc_data->seg[ segid[stnid][devid] ].pol_data[pol_ptr_end].polcomm_en,databuf+offlen,2*Mod16(head.polcomm_size));

	/* added by yyp 07.07.02 */
	POLTimeCount ++;
	if(POLTimeCount >= 60 * 60 *24 )
	{
		POLTimeCount = 0;
		SetSysTimeByPOL(shm_plc_data->seg[ segid[stnid][devid] ].pol_data[pol_ptr_end]);
	}
	pol_ptr_end++;

	if(pol_ptr_end>=MAX_POL_BUF)
	{
		pol_ptr_end=0;
	}
	runvar[stnid][devid].pol_ptr_end = pol_ptr_end;	
	shm_plc_data->seg[ segid[stnid][devid] ].pol_seg_ptr = pol_ptr_end;
	


	return TRUE;
}

/********************************************************************************************************
 read plc INT Alarm data
*********************************************************************************************************/

int GetINTAlm(NCB * nd,LCUID plcid)
{
	int	seg;
	int	ref;
	int	result;
	int	scanlen;
	int	offlen=0;
	int	soealm_base,objalm_base,soealm_max,objalm_max;
	struct plc_data_seg_head	head;
	unsigned char	databuf[MAX_SCAN_BUF*2];  
	unsigned char	stnid,devid;
	stnid=plcid.stnid;
	devid=plcid.devid;

	if (nd==NULL)   return FALSE;

	if(plconf[stnid][devid].plctype==SIEPLC)
	{	
		soealm_base = SOEALMBASE_SIE/2;
		objalm_base = OBJALMBASE_SIE/2;	
		soealm_max	= SOEALMMAX_SIE;		
		objalm_max	= OBJALMMAX_SIE;
	}		
	else if(plconf[stnid][devid].plctype==MBPLC
		|| plconf[stnid][devid].plctype==PREPLC)
	{
		soealm_base = SOEALMBASE_MB;
		objalm_base = OBJALMBASE_MB;
		soealm_max	= SOEALMMAX_MB;		
		objalm_max	= OBJALMMAX_MB;
	}
	else if(plconf[stnid][devid].plctype==GEPLC)
	{
		soealm_base = SOEALMBASE_GE;
		objalm_base = OBJALMBASE_GE;
		soealm_max	= SOEALMMAX_GE;
		objalm_max	= OBJALMMAX_GE;
	}
	else
	{
		sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,plcid.devid,plconf[stnid][devid].plctype);
		log_printf(ERR,1,PLC_APP,log_str);
		return(FALSE);
	}
	/***************** get shm data head **************************************************/
	memcpy(&head,&(shm_plc_data->seg[ segid[stnid][devid] ].head),PLCHEADLEN);

	if(runvar[stnid][devid].FirstIntAlm == YES)
	{
		runvar[stnid][devid].soealm_ptr_end = head.soealm_ptr;
		runvar[stnid][devid].objalm_ptr_end = head.objalm_ptr;
		runvar[stnid][devid].FirstIntAlm = NO;
	}
#ifdef PLCDEBUG
	sprintf(log_str,"soealm_ptr_end=%d, head.soealm_ptr=%d\n",runvar[stnid][devid].soealm_ptr_end,head.soealm_ptr);
#endif	
	if(runvar[stnid][devid].soealm_ptr_end==head.soealm_ptr)	
	{
		MySleep(200);
		/* return TRUE; */
	}
	else
	{	
	
		/***************** Read Soe Alarm Data From PLC **************************************************/
		if(runvar[stnid][devid].soealm_ptr_end<head.soealm_ptr)
		{
			/****** read soe alarm from end ptr to head ptr ******/	
			memset(databuf,0,MAX_SCAN_BUF*2);
			seg = REGSEG;
			ref = 1 + soealm_base + runvar[stnid][devid].soealm_ptr_end * SOE_ALM_LEN;
			scanlen = 0;
			scanlen += (head.soealm_ptr - runvar[stnid][devid].soealm_ptr_end) * SOE_ALM_LEN;
			result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid,READ_4X);
			if(result==FALSE)
			{
				sprintf(log_str,"==== scanapp.c: plc %d.%d GetINTAlm(): ReadPLC error......\n",stnid,devid);
				log_printf(ERR,1,MBX_APP,log_str);	
				return(FALSE);
			}
			memcpy(shm_plc_data->seg[ segid[stnid][devid] ].soealm_buf + 2 * runvar[stnid][devid].soealm_ptr_end * SOE_ALM_LEN, databuf, 2 * scanlen);
		}
		else if(runvar[stnid][devid].soealm_ptr_end > head.soealm_ptr)
		{
			/****** read soe alarm from end ptr to soealm_max ******/	
			memset(databuf,0,MAX_SCAN_BUF*2);
			seg = REGSEG;
			ref = 1 + soealm_base + runvar[stnid][devid].soealm_ptr_end * SOE_ALM_LEN;
			scanlen = 0;
			scanlen += (soealm_max - runvar[stnid][devid].soealm_ptr_end) * SOE_ALM_LEN;
			result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid,READ_4X);
			if(result==FALSE)
			{
				sprintf(log_str,"==== scanapp.c: plc %d.%d GetINTAlm(): ReadPLC error......\n",stnid,devid);
				log_printf(ERR,1,MBX_APP,log_str);	

				return(FALSE);
			}
			memcpy(shm_plc_data->seg[ segid[stnid][devid] ].soealm_buf + 2 * runvar[stnid][devid].soealm_ptr_end * SOE_ALM_LEN, databuf, 2 * scanlen);		
			
			/****** read soe alarm from 0 to head ptr ******/
			if(head.soealm_ptr !=0)
			{
				memset(databuf,0,MAX_SCAN_BUF*2);
				seg = REGSEG;
				ref = 1 + soealm_base;
				scanlen = 0;
				scanlen += (head.soealm_ptr - 0 ) * SOE_ALM_LEN;
				result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid,READ_4X);
				if(result==FALSE)
				{
					sprintf(log_str,"GetINTAlm(): ReadPLC error \n");
					return(FALSE);
				}
				memcpy(shm_plc_data->seg[ segid[stnid][devid] ].soealm_buf, databuf, 2 * scanlen);	
			}
		} /* end if */
		runvar[stnid][devid].soealm_ptr_end = head.soealm_ptr;
		shm_plc_data->seg[ segid[stnid][devid] ].soealm_ptr = runvar[stnid][devid].soealm_ptr_end;
		runvar[stnid][devid].FirstIntAlm = 0;
	}

#ifdef PLCDEBUG
	sprintf(log_str,"objalm_ptr_end=%d, head.objalm_ptr=%d\n",runvar[stnid][devid].objalm_ptr_end,head.objalm_ptr);
#endif	
	if(runvar[stnid][devid].objalm_ptr_end==head.objalm_ptr)	
	{
		MySleep(200);
		/* return TRUE; */
	}
	else
	{
		
		/***************** Read obj Alarm Data From PLC **************************************************/
		if(runvar[stnid][devid].objalm_ptr_end<head.objalm_ptr)
		{
			/****** read obj alarm from end ptr to head ptr ******/	
			memset(databuf,0,MAX_SCAN_BUF*2);
			seg = REGSEG;
			ref = 1 + objalm_base + runvar[stnid][devid].objalm_ptr_end * OBJ_ALM_LEN;
			scanlen = 0;
			scanlen += (head.objalm_ptr - runvar[stnid][devid].objalm_ptr_end) * OBJ_ALM_LEN;
			result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid,READ_4X);
			if(result==FALSE)
			{
				sprintf(log_str,"==== scanapp.c: plc %d.%d GetINTAlm(): ReadPLC error......\n",stnid,devid);
				log_printf(ERR,1,PLCSCAN_APP,log_str);	
				return(FALSE);
			}
			memcpy(shm_plc_data->seg[ segid[stnid][devid] ].objalm_buf + 2 * runvar[stnid][devid].objalm_ptr_end * OBJ_ALM_LEN, databuf, 2 * scanlen);
		}
		else if(runvar[stnid][devid].objalm_ptr_end > head.objalm_ptr)
		{
			/****** read obj alarm from end ptr to objalm_max ******/	
			memset(databuf,0,MAX_SCAN_BUF*2);
			seg = REGSEG;
			ref = 1 + objalm_base + runvar[stnid][devid].objalm_ptr_end * OBJ_ALM_LEN;
			scanlen = 0;
			scanlen += (objalm_max - runvar[stnid][devid].objalm_ptr_end) * OBJ_ALM_LEN;
			result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid,READ_4X);
			if(result==FALSE)
			{
				sprintf(log_str,"==== scanapp.c: plc %d.%d GetINTAlm(): ReadPLC error......\n",stnid,devid);
				log_printf(ERR,1,PLCSCAN_APP,log_str);	

				return(FALSE);
			}
			memcpy(shm_plc_data->seg[ segid[stnid][devid] ].objalm_buf + 2 * runvar[stnid][devid].objalm_ptr_end * OBJ_ALM_LEN, databuf, 2 * scanlen);		
			
			/****** read obj alarm from 0 to head ptr ******/
			if(head.objalm_ptr !=0)
			{
				memset(databuf,0,MAX_SCAN_BUF*2);
				seg = REGSEG;
				ref = 1 + objalm_base;
				scanlen = 0;
				scanlen += (head.objalm_ptr - 0 ) * OBJ_ALM_LEN;
				result = ReadPLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid,READ_4X);
				if(result==FALSE)
				{
					sprintf(log_str,"GetINTAlm(): ReadPLC error \n");
					return(FALSE);
				}
				memcpy(shm_plc_data->seg[ segid[stnid][devid] ].objalm_buf, databuf, 2 * scanlen);	
			}
		} /* end if */
		runvar[stnid][devid].objalm_ptr_end = head.objalm_ptr;
		shm_plc_data->seg[ segid[stnid][devid] ].objalm_ptr = runvar[stnid][devid].objalm_ptr_end;
		runvar[stnid][devid].FirstIntAlm = 0;
	}/* end if */

	return TRUE;
}

/********************************************************************************************************
 write date to plc
*********************************************************************************************************/

int WriteData(NCB * nd,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	int		offlen=0;
	struct plc_data_seg_head	head;
	
	unsigned char	stnid,devid;
	stnid = plcid.stnid;
	devid = plcid.devid; 
	if (nd==NULL)   return FALSE;
	runvar[stnid][devid].send_update_flag = 0;
	MySleep(200);
	/***************** get shm data head **************************************************/
	memcpy(&head,&(shm_plc_data->seg[ segid[stnid][devid] ].head),PLCHEADLEN);

	runvar[stnid][devid].timeloopcount ++;
	if( runvar[stnid][devid].timeloopcount >= 5)
	{	
		/******  write run flag to plc ******/
		if(WriteRunFlag(nd,plcid) == FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d write run flag error\n",stnid,devid);
			log_printf(ERR,1,MBX_APP,log_str);	
			return 	FALSE;
		}
		runvar[stnid][devid].timeloopcount = 0;
	}

/* removed by yyp 07.04.21 */
#ifdef YYP	
	/****** write sending data to plc if updated ******/

	if(shm_plc_send->seg[stnid][devid].send_update_flag != runvar[stnid][devid].send_update_flag)
	{
		runvar[stnid][devid].send_update_flag = shm_plc_send->seg[stnid][devid].send_update_flag;
		if(shm_plc_send->seg[stnid][devid].ai_size > SEND_POL_SIZE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d send ai_size =%d error......\n",stnid,devid,shm_plc_send->seg[stnid][devid].ai_size);
			log_printf(ERR,1,MBX_APP,log_str);
			sprintf(smsbuf,"LCU%d.%d 下送模拟量数=%d错误",stnid,devid,shm_plc_send->seg[stnid][devid].ai_size);
			BdSms(smsbuf);	
			return(FALSE);		
		}
		if( shm_plc_send->seg[stnid][devid].pol_size > SEND_POL_SIZE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d send pol_size =%d error......\n",stnid,devid,shm_plc_send->seg[stnid][devid].pol_size);
			log_printf(ERR,1,MBX_APP,log_str);	
			sprintf(smsbuf,"LCU%d.%d 下送开关量数=%d错误",stnid,devid,shm_plc_send->seg[stnid][devid].pol_size);
			BdSms(smsbuf);				
			return(FALSE);			
		}		
		runvar[stnid][devid].send_update_flag = shm_plc_send->seg[stnid][devid].send_update_flag;
		memset(tmpbuf,0,MAX_SCAN_BUF*2);
		memset(databuf,0,MAX_SCAN_BUF*2);
		if(plconf[stnid][devid].plctype==SIEPLC)
		{	
			seg = SENDDB_NO;
			ref = SENDBASE_SIE/2;
			shm_plc_send->seg[stnid][devid].aiqlt_base = SENDANAQLTBASE_SIE;
			shm_plc_send->seg[stnid][devid].pol_base = SENDPOLBASE_SIE;
		}		
		else if(plconf[stnid][devid].plctype==MBPLC
			|| plconf[stnid][devid].plctype==PREPLC)
		{
			seg=REGSEG;
			ref=SENDBASE_MB;
			shm_plc_send->seg[stnid][devid].aiqlt_base = SENDANAQLTBASE_MB;
			shm_plc_send->seg[stnid][devid].pol_base = SENDPOLBASE_MB;
		}
		else if(plconf[stnid][devid].plctype==GEPLC)
		{
			seg=REGSEG;
			ref=SENDBASE_GE;
			shm_plc_send->seg[stnid][devid].aiqlt_base = SENDANAQLTBASE_GE;
			shm_plc_send->seg[stnid][devid].pol_base = SENDPOLBASE_GE;			
		}
		else
		{
			sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,
				plcid.devid,plconf[stnid][devid].plctype);
			log_printf(ERR,1,PLC_APP,log_str);
			return(FALSE);
		}	
		
		scanlen = SEND_DATA_SIZE;
	
		memcpy(databuf,&(shm_plc_send->seg[stnid][devid]),scanlen*2);
#ifdef _BIG_ENDIAN_
		Swap2Str((unsigned char *)databuf,4);
#endif	
		result=WritePLC(nd, (unsigned char)seg, ref, scanlen, databuf, plcid);
		if(result==FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d write plc sending data error......\n",stnid,devid);
			log_printf(ERR,1,MBX_APP,log_str);	
			return(FALSE);
		}
		else
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d  write plc sending data successfully\n",stnid,devid);
			log_printf(INF,2,MBX_APP,log_str);	
		}

	} /* end if */

	/****** if reaching time, write time to plc ******/
	/*
	if(runvar[stnid][devid].FirstSend == YES)
	{	
		if(WriteTimeCmd(nd,plcid) == FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d write time cmd error\n",stnid,devid);
			log_printf(ERR,1,MBX_APP,log_str);	
			return 	FALSE;
		}
		runvar[stnid][devid].FirstSend  = NO;
	}
	*/
#endif

	return TRUE;
}

/***************************************************************************************
Write Run Flag to a WORD in plc data memory . RunFlag switch between 1 and 2 
every loop of Reading plc data.Address is defined as PLCRUNFLAG in "lcu_data.h"?
return TRUE when successfully,FALSE otherwise.
****************************************************************************************/
int WriteRunFlag(NCB * nd,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	unsigned char	stnid,devid;
	unsigned	short runflag;			/*small size buf */
	stnid = plcid.stnid;
	devid = plcid.devid; 
	if(plconf[stnid][devid].plctype==SIEPLC)
	{	
		seg = SENDDB_NO;
		ref = PLCRUNADDR_SIE/2;
	}		
	else if(plconf[stnid][devid].plctype==MBPLC
		|| plconf[stnid][devid].plctype==PREPLC)
	{
		seg=REGSEG;
		ref=PLCRUNADDR_MB;
	}
	else if(plconf[stnid][devid].plctype==GEPLC)
	{
		seg=REGSEG;
		ref=PLCRUNADDR_GE;
	}
	else
	{
		sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,plcid.devid,plconf[stnid][devid].plctype);
		log_printf(ERR,1,PLC_APP,log_str);
		return(FALSE);
	}
	scanlen=1;
	runflag = runvar[stnid][devid].plc_run_flag;
	runvar[stnid][devid].plc_run_flag ++;
	if(runvar[stnid][devid].plc_run_flag >= MAX_PLC_FLAG)
	{
		runvar[stnid][devid].plc_run_flag =1;
	}
#ifdef _BIG_ENDIAN_
	Swap2Byte(&runflag);
#endif
	result=WritePLC(nd, (unsigned char)seg, ref, scanlen, (unsigned char *)&runflag, plcid);
	if(result==FALSE)
	{
		sprintf(log_str,"==== scanapp.c: plc %d.%d Write run flag data error\n",stnid,devid);
		log_printf(ERR,1,MBX_APP,log_str);	
		return(FALSE);
	}
	return TRUE;
}


/***************************************************************************************
 WriteTimeCmd(NCB * nd,LCUID plcid)
****************************************************************************************/
int WriteTimeCmd(NCB * nd,LCUID plcid)
{
	LAN_HEAD        head;
	COMMAND_MSG	msg;
	int		length;
	char		buf[COMMAND_SIZE];
	char            namebuf[40];
	char		stn_name[STATION_NAME_SIZE];
	char		dev_name[GROUP_NAME_SIZE];	
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	POINTER		pt;
	unsigned	char stnid,devid;
	stnid = plcid.stnid;
	devid = plcid.devid;
	
	if(-1==GetStnNameById(stnid, stn_name))
	{
		return FALSE;
	}
	if(-1==GetGroupNameById(stnid, devid, dev_name))
	{
		return FALSE;
	}	
	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "SYS.TIME_SYNC");
	
	if(-1==GetPtIdByNameStr(namebuf, &pt))
	{
		sprintf(log_str,"\nno such point %s in db\n",namebuf);
		log_printf(ERR,1,MBX_APP,log_str);
		return FALSE;
	}
		
	msg.type_id = TIME_SYNC;
	msg.stn_id = pt.stn_id;
	msg.dev_id = pt.dev_id;
	msg.data_type = SYSMSG_TYPE;
	msg.point_id = pt.pt_id;	
	msg.status.opr = 0;
	msg.status.result = 0;
	msg.status.state = 0;
	msg.status.src = 0;
	msg.data.iValue = 0;
	GetSrcId(&msg.host_id);
	msg.usr_id=0;
	strcpy(msg.usr_login_name,"plcscan");
	if(GetPtCtrlAddrById( &pt, &msg.ctrl_addr ) == -1)
	{
		printf("GetPtCtrlAddrById eror\n");
		return FALSE;
	}
	
	re_time=time(&newtime);
	devtime=localtime(&newtime);
	msg.second  =devtime->tm_sec;
	msg.minute  =devtime->tm_min;
	msg.hour  =devtime->tm_hour;
	msg.day  =devtime->tm_mday;
 	msg.month  =devtime->tm_mon+1;
	msg.year  =devtime->tm_year+1900; 
	head.dest_id=msg.dev_id;
	head.src_id = msg.host_id;
	head.dp_type=COMM_TIME;
	head.length = COMMAND_SIZE;
	head.dest_stn = msg.stn_id;
	length = head.length;
	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&head.length);	
	Swap2Byte(&msg.point_id);
	Swap2Byte(&msg.status);
	Swap4Byte(&msg.data);
	Swap2Byte(&msg.ctrl_addr);
	Swap2Byte(&msg.year);
#endif
	memcpy(buf,&msg,COMMAND_SIZE);
	lan_out((char *)&msg,head,0);	
	return TRUE;
}


/***************************************************************************************
Connect to a given plc  using TCP/IP。
return socketid when successfully,INVALID_NCB * otherwise.
****************************************************************************************/

NCB	*ConnectPLC(LCUID plcid)
{
	NCB *nd;
	sprintf(ncb_addr[plcid.stnid][plcid.devid],"%s",g_mbpaddr);	/* mbp addr is define in appconfig.def added by yyp 07.02.28 */
	if ((nd = ncb_open(ncb_addr[plcid.stnid][plcid.devid], 0)) == NULL) 
	{
		sprintf(buffer,"Unable to open DATA MASTER path: %s.\n",ncb_addr[plcid.stnid][plcid.devid]);
		err_write(buffer);
		return ( nd );
	}

	sprintf(buffer,"Path %02X opened\n", nd->NCB_NUM);
	myprintf(buffer);
	sprintf(buffer,"NCB.length = %d \n",nd->NCB_LENGTH);
	myprintf(buffer);
	sprintf(buffer,"NCB.RTO = %d \n",nd->NCB_RTO);
	myprintf(buffer);
	sprintf(buffer,"NCB.STO = %d \n",nd->NCB_STO);
	myprintf(buffer);
	sprintf(buffer,"Routing info: %c%c.%d.%d.%d.%d.%d\n",
	nd->NCB_CALLNAME[0],
	nd->NCB_CALLNAME[1],
	nd->NCB_CALLNAME[2],
	nd->NCB_CALLNAME[3],
	nd->NCB_CALLNAME[4],
	nd->NCB_CALLNAME[5],
	nd->NCB_CALLNAME[6]);
	myprintf(buffer);
 	return (nd);
}

/***************************************************************************************
DisConnect from a given plc  using TCP/IP。
****************************************************************************************/
void DisConnectPLC(NCB *nd,LCUID plcid)
{
	sprintf(buffer,"\nDisConnectPLC %d.%d\n", plcid.stnid,plcid.devid );
	err_write(buffer);	
	if( nd == NULL ) return;
	ncb_close(nd);
	nd = (NCB *)NULL;
	return;
}


/***************************************************************************************
receive data from a given plc through given socket  using TCP/IP。
return TRUE when successfully,FALSE otherwise。
nd:	socket link to plc
count:		size of data to be received
buffer:		pointer of data  to be stored
****************************************************************************************/
int  RecvPLC(NCB *nd, char *buf, int count)
{
	if (ncb_receive_wait(nd,buf,MBX_TIMEOUT) != 0)
	{
		  /*try to receive*/
		  sprintf(buffer,"\nReceive error: %d.\n", nd->NCB_RETCODE);
		  err_write(buffer);
		  return (FALSE);;
	}

#ifdef DEBUG
	sprintf(buffer,"\n==== RecvPLC, recvcount=%d >>\n",count);
	/*myprintf(buffer);*/
	for(i=0;i<count; i++)
	{
		sprintf(tmpbuf,"%02x=",(unsigned char)buf[i]);
		strcat(buffer,tmpbuf);
		if(i==(MBXRECVPDUHEAD-1))	
		{	
			sprintf(tmpbuf,"\n");
			strcat(buffer,tmpbuf);
		}
		else
		{
			if((i+1-MBXRECVPDUHEAD)%20 == 0)
			{
				sprintf(tmpbuf,"\n");
				strcat(buffer,tmpbuf);
			}
		}			
	}
	sprintf(tmpbuf,"\n");	
	strcat(buffer,tmpbuf);
	printf(buffer);
#endif 
	return (count);
}

/***************************************************************************************
read data from a given plc through given socket  using TCP/IP。
return TRUE when successfully,FALSE otherwise。
nd:	socket link to plc
ref:		starting address of	plc shared memory to be read
size:		word size of data to be read
buffer:		pointer of data  to be stored
****************************************************************************************/
int ReadPLC(NCB *nd, unsigned char seg, int ref, int size, unsigned char *buf,
			LCUID plcid,unsigned char type)
{
	int result;
	int i, j, len;
	int count;
	int bsize;
	int offset;
	int lencount=0;
	char dummy[MAX_RECV_SIZE];
	unsigned char tmpbuffer[2048];
	int           base_addr;
	sprintf(buffer,"==== Run ReadPLC, seg=%d, ref=%d, size=%d \n",seg,ref,size);
	log_printf(INF,2,PLC_APP,buffer);	
	ref=ref-1;
	if( ref<0 || size <=0 || size >= MAX_SCAN_BUF )
	{
		sprintf(log_str,"ReadPLC_MB(): ref =%d or size =%d invalid\n",ref,size);
		log_printf(ERR,1,PLC_APP,log_str);		
		return FALSE;
	}
	base_addr = ref;
	bsize = MAX_QUERY_SIZE;
	count =(size-1) / ( MAX_QUERY_SIZE ) + 1;
	if( ref<0 || size <=0 || size >= MAX_SCAN_BUF )
	{
		sprintf(log_str,"ReadPLC(): ref =%d or size =%d invalid\n",ref,size);
		log_printf(ERR,1,MBX_APP,log_str);
		return FALSE;
	}
	offset=0;
	for(j=0;j<count;j++)
	{
		/* ref=base_addr + j*MAX_QUERY_SIZE; */
		ref=base_addr + lencount;
		dummy[0] = 0x01;								/*slave address*/
		if(type == READ_0X)
		{
			dummy[1] = (unsigned char) READ_0X;
		}
		else
		{
			dummy[1] = (unsigned char) READ_4X;
		}
		dummy[2] = (unsigned char)(ref >> 8);           /* Reg. high Addr.(1 word)*/
		dummy[3] = (unsigned char)(ref & 0xff);         /* Low Addr. */   
		if(type != READ_0X)
		{
			dummy[4] = 0;									/* Note:byte number must less than 255(high=0) */
			dummy[5] = (unsigned char)(bsize & 0xff);		/* Word number (low) */
			if(j==count-1)	dummy[5] = size -j* MAX_QUERY_SIZE;
			len=dummy[5];
			if(len==0)	continue;
		}
		else
		{
			len = MAX_QUERY_SIZE * 16;
			if(j==count-1)	len = ( size -j* MAX_QUERY_SIZE ) * 16;
			dummy[4] = (unsigned char)(len >> 8);		/* Note:byte number must less than 255(high=0) */
			dummy[5] = (unsigned char)(len & 0xff);		/* Word number (low) */
			if(len==0)	continue;
		}
		result = 0;
		result = ncb_send(nd, MBX_SCAN_LEN, dummy, 5);
		if (result != 0)
		{
			sprintf(buffer,"ncb_send error: %d.\n", nd->NCB_RETCODE);			
			err_write(buffer);
			return (FALSE);
		}
		lencount += len;
		/* modified by yyp 07.03.20 */
		if(type == READ_0X)
		{
			len = len/16;
		}
#ifdef 	DEBUG
		sprintf(buffer,"\n==== ReadPLC, sendcount=%d ,id = %d/%d >>\n",MBX_SCAN_LEN,j+1,count);
		/*myprintf(buffer);*/
		for(i=0;i<MBX_SCAN_LEN;i++)
		{
			sprintf(tmpbuf,"=%02x",(unsigned char) dummy[i]);
			strcat(buffer,tmpbuf);
			if((i+1)%20 == 0)
			{
				sprintf(tmpbuf,"\n");
				strcat(buffer,tmpbuf);
			}		
		}
		sprintf(tmpbuf,"\n");
		strcat(buffer,tmpbuf);
		printf(buffer);
#endif

		result = RecvPLC(nd, (char *)dummy, len*2+MBXRECVPDUHEAD);
		if (result==FALSE  )
		{
			sprintf(buffer,"ReadPLC: RecvPLC_MB() error\n");
			err_write(buffer);
			return(FALSE);
		}
		
		for(i=0;i<2*len;)
		{
			if(type == READ_0X)
			{
				tmpbuffer[offset+i]= dummy[ i+ MBXRECVPDUHEAD ]; 
				tmpbuffer[offset+i+1]= dummy[ i+ MBXRECVPDUHEAD+1 ]; 
			}
			else
			{
				tmpbuffer[offset+i+1]= dummy[ i+ MBXRECVPDUHEAD ]; 
				tmpbuffer[offset+i]= dummy[ i+ MBXRECVPDUHEAD+1 ]; 
			}
			i=i+2;
		}
		offset=offset+2*len;
	}
	memcpy(buf,tmpbuffer,offset);
	return (TRUE);
}

/***************************************************************************************
write data or cmd to a given plc through given socket  using TCP/IP。
return TRUE when successfully,FALSE otherwise。
nd:	socket link to plc
ref:		starting address of	plc shared memory to be written
size:		word size of data to be written
buffer:		pointer of data to be written
****************************************************************************************/
int WritePLC(NCB *nd, unsigned char seg, int ref, int size, unsigned char *buf, LCUID plcid)
{
	int result;
	int i;
	char dummy[MAX_RECV_SIZE];

	ref=ref-1;
	if( ref<0 || size <=0 || size >= MAX_SCAN_BUF )
	{
		sprintf(log_str,"WritePLC_MB(): ref =%d or size =%d invalid\n",ref,size);
		log_printf(ERR,1,PLC_APP,log_str);
		return FALSE;
	}
	sprintf(buffer,"seg=%d,ref=%d,size=%d\n",seg,ref,size);
	log_printf(INF,2,PLC_APP,buffer);	

	/***** package command  code ************/
	if(size > 100)
	{
		sprintf(buffer,"WritePLC(): error.  size is %d,the MAX is 120\n",size);		
		err_write(buffer);
		return (FALSE);
	}
	
/*	memcpy(dummy,SysWriteReq, PLC_CMD_LEN); */

	dummy[0] = 0x01;								/* slave address*/
	dummy[1] = 0x10;								/* command*/
	dummy[2] = (unsigned char)(ref >> 8);           /* Reg. high Addr.(1 word) */
	dummy[3] = (unsigned char)(ref & 0xff);         /* Low Addr.*/
	dummy[4] = 0;									/* Note:word number must less than 255(high=0) */
	dummy[5] = (unsigned char)(size & 0xff);		/* Word number (low) */
	dummy[6] = dummy[5]*2;							/* byte number (low) */
	for(i=0;i<2*size;)
	{
		dummy[ i+ MBX_CMD_LEN] = buf[i+1];			/*only for modicon*/   
		dummy[ i+ MBX_CMD_LEN+1] = buf[i];
		i = i+2;
	}

	if (ncb_send(nd, MBX_CMD_LEN+ 2*size , dummy, 5) != 0)
	{
	  /*send the command*/
	  sprintf(buffer,"WritePLC(): ncb_send error %d.\n", nd->NCB_RETCODE);
		err_write(buffer);
	  return (FALSE);
	}

#ifdef  DEBUG
	sprintf(buffer,"==== Write data >>\n");
	/* myprintf(buffer); */
	for(i=0;i<MBX_CMD_LEN+ 2*size;i++)
	{
		sprintf(tmpbuf,"=%02x",dummy[i]);
		strcat(buffer,tmpbuf);
		if((i+1)%20 == 0)
		{
			sprintf(tmpbuf,"\n");
			strcat(buffer,tmpbuf);
		}		
	}
	sprintf(tmpbuf,"\n");
	strcat(buffer,tmpbuf);
	myprintf(buffer);
#endif

	result = RecvPLC(nd, (char *)dummy, MBX_CMD_LEN);
	if (result==FALSE )
	{
		sprintf(buffer,"WritePLC(): call RecvPLC_MB() error\n");
		err_write(buffer);
		return (FALSE);
	}

	return (TRUE);
}

int myprintf( char *buf)
{
#ifdef DIALOG_VER
	MyMsgBox((unsigned char *)buf);
#endif
	log_printf(INF,1,MBXCMD_APP,buf);
	MySleep(10);
	return TRUE;
}


void SetSysTimeByPOL(struct pol_data_seg polseg )
{
	
	struct tm poltime;
	poltime.tm_sec = (unsigned char)(polseg.pol_sec_ms/1000);
	poltime.tm_min = * ((unsigned char *)(&polseg.pol_hour_min)) ;
	poltime.tm_hour = * ((unsigned char *)(&polseg.pol_hour_min)+1) ;
	poltime.tm_mday = * ((unsigned char *)(&polseg.pol_month_day)) ;
	poltime.tm_mon = * ((unsigned char *)(&polseg.pol_month_day )+1)  - 1 ;
	poltime.tm_year = polseg.pol_year  - 1900;
		
	stime(poltime);

	return;
}


int stime(struct tm newtime)
{ 
  SYSTEMTIME st;
  GetLocalTime(&st);
  st.wYear       = newtime.tm_year+1900;
  st.wMonth	     = newtime.tm_mon+1;	  
  st.wDay        = newtime.tm_mday;	
  st.wHour       = newtime.tm_hour;
  st.wMinute     = newtime.tm_min;
  st.wSecond     = newtime.tm_sec; 
  st.wDayOfWeek  = newtime.tm_wday;
  if(!SetLocalTime(&st)) 
  { 
	  printf("SetLocalTime() error!\n"); return(-1); 
  }
  else
  { 
	  printf("\nSET LOCAL TIME: %04d-%02d-%02d %02d:%02d:%02d\n",
					st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
  }
  return(TRUE);
} 