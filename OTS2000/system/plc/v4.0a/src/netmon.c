/************************************************************************************************************************************************
FILENAME:	netmon.c
FUNCTION:	
	
		monitor and test the communication state of each net device

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
2008.7.3	create the file for H9000 V4.0		yyp
************************************************************************************************************************************************/

#include "../../../lan/v4.0a/inc/lan.h"
#include "../../../cmd/v4.0a/inc/cmd_functions.h"
#include "../inc/plc_com.h"

#define MULTITHREAD
#ifdef MSWIN32
 DWORD		threadsend,threadrecv,wait_thrid;
 WSADATA	wsaData;	
#endif
#ifndef MSWIN32
 pid_t		proc_id[MAX_STN_NUM+1][MAX_LCU_NUM+1][4];
 thread_t	threadsend,threadrecv,tid_terminated,wait_thrid;
  pid_t		exit_proc_id;
#endif
int ChildThread(); 
void * ThreadSend( void * arg );
void * ThreadRecv( void * arg );
void sig_termp();
void kill_child();
void InitIpDef();
int GetIpDef();
int CreateWaitThread();
void * ThreadWait( void * arg );
int  childflag=0;
int	 IsPlcmonHost = FALSE;
extern IpDef ip[MAXIPNUM];
extern int ipnum;
SOCKET    sockRaw = INVALID_SOCKET;

/************************************************************************************************************************************************
the main() function of "QUANTUM" plc communication module include:
initiate the function;create and maintain the thread
of each LCU	modified by yyp 2005.11.1
************************************************************************************************************************************************/

int main(int argc, char *argv[])
{
	char		hostname[20];		/* hostname of local machine */
	unsigned char	stnid,devid,idx;
	short		FirstLoop=YES;
	short		OldScanPrvg,CurScanPrvg;
	char		log_str[256];

	wsa_startup();
	/* SetLogLevel(argc, argv); */
	g_loglevel = 2;
	printf("\n========================= plcmon.dxe start running ==========================\n");
#ifndef MSWIN32	
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
#endif
	g_stn_num =1;
	CurScanPrvg=CUR_WS_IN_VICE;
	gethostname(hostname,20);
	CreateLockFile(".netmon.dxe");
	if(-1==InitSysLog(H9000_MODULE_NETMON, "NetMon"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}

	if(MountDB(DB_NAME) == -1)
	{
		printf("==== netmon.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	if(lan_init_all() == FALSE)
	{
		printf("==== plcmon.c: call Lan_init_all() error\n");		
		exit(0);
	}

	/* added by yyp 07.08.07 */	

	if(GetIpDef()==0)	
	{
		printf("\n==== plcmon.c: GetIpDef() error!\n");
		exit(0);
	}
	else
	{
		printf("\n==== GetIpDef() successfully!\n");
	}	
	
#ifdef MSWIN32 
	/*sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED);*/
	sockRaw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(sockRaw == INVALID_SOCKET)
	{
		printf("WSASocket() failed: %d\n", WSAGetLastError());
		return FALSE;
	}
#else
	if ((sockRaw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == INVALID_SOCKET)
	{
		sprintf(log_str,"plcping.c: socket() error\n");
		log_write(H9000_LOG_ERROR,1,log_str);
		sprintf(log_str,"plcping.c: Reason-- oraginal socket must be run by 'root'. \n\ndo this and restart the app:\n");
		log_write(H9000_LOG_ERROR,1,log_str);
		sprintf(log_str,"(1)su root (2)./PLCMONMAKE.csh (3)exit root and run by ems user. \n");
		log_write(H9000_LOG_ERROR,1,log_str);		
		return FALSE;
	}
	else
	{
		sprintf(log_str,"plcping.c: plc socket() success, sock=%d\n");
		log_write(H9000_LOG_INFO,1,log_str);		
	}
		
#endif
	MySleep(2000);
#ifndef MSWIN32
	setuid(getuid());
#endif



	/******** Init *******/
	for(idx=1;idx<MAX_STN_NUM+1;idx++)
	{
		for(devid=1;devid<MAX_LCU_NUM+1;devid++)
		{  
			if(plconf[idx][devid].cpu_net_mode != CPU_D_NET_D)
			{
				if(plconf[idx][devid].ipnum > 2)
				{
					plconf[idx][devid].ipnum=2;
				}
		  }
		}
	}
#ifdef 	MULTITHREAD	
	CreateWaitThread();
#endif
	CurScanPrvg = CUR_WS_IN_VICE;
	//stnid =1;
	GetHostStnId(hostname, &stnid);

	for(;;)
	{
		MySleep(1000);
		OldScanPrvg = CurScanPrvg;
		CurScanPrvg = GetPLCMonPrvg(stnid);
		if(CurScanPrvg==CUR_WS_IN_HOST)
		{
			IsPlcmonHost = TRUE;
		}
		else
		{
			IsPlcmonHost = FALSE;
		}
		if( CurScanPrvg != CUR_WS_IN_HOST && OldScanPrvg == CUR_WS_IN_HOST )
		{
			
			if(childflag == 0)
			{
				kill_child();
			}
			MySleep(2000);
			sprintf(log_str,"plcmon.c: all child process  exit...\n");
			log_printf(H9000_LOG_ERROR,1,1,log_str);
			sprintf(log_str,"==== %s is not the station %d primary plc monitor server\n", hostname,stnid);
			log_printf(H9000_LOG_ERROR,1,1,log_str);
		}
		else if(CurScanPrvg == CUR_WS_IN_HOST && CurScanPrvg != OldScanPrvg)
		{
			
			sprintf(log_str,"==== %s is the station %d primary plc monitor server\n", hostname,stnid);
			log_printf(H9000_LOG_ERROR,1,1,log_str);			
			FirstLoop = NO;
			for(idx=1;idx<=g_stn_num;idx++)
			{
				if(idx>1) break;
				BdPlcMonHost( id_stn[idx] );
				ChildThread();
			}/* end for */
			break;
		}
		else
		{
			if(FirstLoop == YES)
			{
				FirstLoop = NO;
				sprintf(log_str,"==== %s is not the station %d primary plc monitor server\n", hostname,stnid);
				log_printf(H9000_LOG_ERROR,1,1,log_str);
			}
		}
	}/* end for */

	for(;;)
	{
		MySleep(1000);
	}

	sprintf(log_str,"==== plcmon.c: plc %d.%d mon main process exit! \n",stnid,devid); 
	log_printf(H9000_LOG_INFO,1,1,log_str);
	/****** release the resources ******/
#ifdef MSWIN32
	WSACleanup();
#endif	
	UnMountDB();
	return 1;
}




/************************************************ Child Process function of each PLC ***********************************************************************************/
int ChildThread()
{
	int 		res;
	char		log_str[256];
#ifndef 	MULTITHREAD	
	ThreadSend(NULL);
#endif

#ifndef MSWIN32
	res =   thr_create(NULL, 0, ThreadSend, NULL, 0, &threadsend);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== plcmon.c: Create send thread thr_id=%d OK\n",
			(int)threadsend);	
		log_printf(H9000_LOG_INFO,1,1,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcmon.c: Create send thread failed\n");
		log_printf(H9000_LOG_ERROR,1,1,log_str);
		return FALSE;
	}
	MySleep(1000);
	res =   thr_create(NULL, 0, ThreadRecv, NULL, 0, &threadrecv);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== plcmon.c: Create recv thread thr_id=%d OK\n",
			(int)threadrecv);	
		log_printf(H9000_LOG_INFO,1,1,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcmon.c: Create recv thread failed\n");
		log_printf(H9000_LOG_ERROR,1,1,log_str);
		return FALSE;
	}
#else
	res=CreateThread(NULL,	0,	(LPTHREAD_START_ROUTINE) ThreadSend, 
		 		NULL,	0,	(LPDWORD)&threadsend);

	if(res!=NULL)
	{
		sprintf(log_str,"==== plcmon.c: Create sned thread thr_id=%d OK\n",
			(int)threadsend);	
		log_printf(H9000_LOG_INFO,1,1,log_str);
	}
	else
	{
		sprintf(log_str,"==== plcmon.c: Create thread failed\n");
		log_printf(H9000_LOG_ERROR,1,1,log_str);
		return FALSE;	    
	}
	MySleep(1000);
	res=CreateThread(NULL,	0,	(LPTHREAD_START_ROUTINE) &ThreadRecv, 
		 		NULL,	0,	(LPDWORD)&threadrecv);

	if(res!=NULL)
	{
		sprintf(log_str,"==== plcmon.c: Create recv thread thr_id=%d OK\n",
			(int)threadrecv);	
		log_printf(H9000_LOG_INFO,1,1,log_str);
	}
	else
	{
		sprintf(log_str,"==== plcmon.c: Create thread failed\n");
		log_printf(H9000_LOG_ERROR,1,1,log_str);
		return FALSE;	    
	}
#endif

	MySleep(100);	
	return TRUE;
}




void sig_termp()
{
#ifndef MSWIN32
	unsigned char stnid,idx,devid,i;
	sprintf(log_str,"plcmon.c: process %d get exit signal...\n",getpid());
	log_write(H9000_LOG_ERROR,1,log_str);	
	if(childflag==0)
	{
		for(idx=1; idx <= g_stn_num; idx++)
		{
			for(devid=1;devid<=g_dev_num[idx];devid++)
			{
		 		for(i=0; i < plconf[idx][devid].ipnum ; i++)
				{
		 			if( proc_id[idx][devid][i] != 0 )	kill( proc_id[idx][devid][i], SIGINT );
		 		}	
			}
		}
		sprintf(log_str,"plcmon.c: main process exit...\n");
		log_write(H9000_LOG_ERROR,1,log_str);	
	}
	else
	{
		sprintf(log_str,"plcmon.c: child process %d exit...\n",getpid());
		log_write(H9000_LOG_ERROR,1,log_str);
	}
#endif
	exit(-1);
}


void kill_child()
{
/*
#ifndef MSWIN32
	kill( getpid(), SIGINT );
#endif
*/
	
#ifndef MSWIN32
	unsigned char stnid,idx,devid,i;
	for(idx=1; idx <= g_stn_num; idx++)
	{
		for(devid=1;devid<=g_dev_num[idx];devid++)
		{
			for(i=0; i < plconf[idx][devid].ipnum ; i++)
			{
		 		if( proc_id[idx][devid][i] != 0 )	kill( proc_id[idx][devid][i], SIGINT );
				sprintf(log_str,"plcmon.c: SIGNIT %d ...\n",proc_id[idx][devid][i]);
				log_write(H9000_LOG_ERROR,1,log_str);			 		
		 		proc_id[idx][devid][i] = 0;
		 	}
		}
	}
#endif

	return;
}

/************************************************ Child Process function of each PLC ***********************************************************************************/
int CreateWaitThread()
{
	int 		res;
	char		log_str[256];
#ifndef MSWIN32
	res =   thr_create(NULL, 0, ThreadWait, NULL, 0, &wait_thrid);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== plcmon.c: Create wait thread thr_id=%d OK\n",
			(int)wait_thrid);	
		log_printf(H9000_LOG_INFO,1,1,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcmon.c: Create Wait thread failed\n");
		log_printf(H9000_LOG_ERROR,1,1,log_str);
		return FALSE;	    
	}
#else
	res=CreateThread(NULL,	0,	(LPTHREAD_START_ROUTINE) ThreadWait, 
		 		NULL,	0,	&wait_thrid);

	if(res!=NULL)
	{
		sprintf(log_str,"==== plcmon.c: Create Wait thread thr_id=%d OK\n",
			wait_thrid);	
		log_printf(H9000_LOG_INFO,1,1,log_str);
	}
	else
	{
		sprintf(log_str,"==== plcmon.c: Create Wait thread failed\n");
		log_printf(H9000_LOG_ERROR,1,1,log_str);
		return FALSE;	    
	}

#endif

	MySleep(100);	
	return TRUE;
}



void * ThreadWait( void * arg )
{
	char		log_str[256];
	MySleep(10000);	
#ifndef MSWIN32
	for(;;)
	{
		exit_proc_id= wait((int*)NULL);
		if(IsPlcmonHost == TRUE)
		{
			/* removed by yyp 07.05.19
			if(-1==exit_proc_id || exit_proc_id==0 )
			{
				sprintf(log_str,"==== plcmon.c: failed to wait exited process ,id= %d \n",exit_proc_id); 
				log_printf(H9000_LOG_ERROR,1,PLCMON_APP,log_str);
				MySleep(2000);
				continue;
			}
			else
			{
				sprintf(log_str,"==== plcmon.c: get exited process id= %d \n",exit_proc_id); 
				log_printf(H9000_LOG_ERROR,1,PLCMON_APP,log_str);
			}
			for(idx=1;idx<=g_stn_num;idx++)
			{
				for(devid=1;devid<=g_dev_num[idx];devid++)
				{
					for(i=0; i < plconf[idx][devid].ipnum ; i++)
					{
						if(exit_proc_id == proc_id[idx][devid][i])
							break;
					}
					if(exit_proc_id == proc_id[idx][devid][i])
						break;				
				}
				if(exit_proc_id == proc_id[idx][devid][i])
					break;				
			}
			if(idx>=g_stn_num && devid >= g_dev_num[idx] && (i+1)>= plconf[idx][devid].ipnum)
			{
				sprintf(log_str,"==== plcmon.c: invalid exited process \n"); 
				log_printf(H9000_LOG_ERROR,1,PLCMON_APP,log_str);
				continue;
			}

			proc_id[idx][devid][i] = -1;
			
			sprintf(log_str,"==== plcmon.c: plc %d.%d ipnum=%d  process exited! \n",id_stn[idx],devid,i); 
			log_printf(H9000_LOG_ERROR,1,PLCMON_APP,log_str);
			proc_id[idx][devid][i]= fork();
			if(proc_id[idx][devid][i] == 0)
			{
				childflag = 1; 
				ChildThread(id_stn[idx],devid,i);		
			}
			sprintf(log_str,"==== plcmon.c: plc %d.%d ipnum=%d process reboot,procid=%d! \n",id_stn[idx],devid,i,proc_id[idx][devid][i]); 
			log_printf(H9000_LOG_INFO,1,PLCMON_APP,log_str);
			*/
		}
		else
		{
			MySleep(100);
		}
	}
#endif	

	for(;;)
	{
		
		MySleep(1000);
	}
	sprintf(log_str,"==== plcmon.c: ThreadWait exit! \n"); 
	log_printf(H9000_LOG_INFO,1,1,log_str);
}


int GetIpDef()
{
	char FileName[100]="/home/ems/h9000/def/netip_conf.def";
	char StrTmp[100];
	FILE *fp;
	int i;
	time_t now;
	time(&now);
	if (( fp = fopen ( FileName,"r"))==NULL)
	{
		printf ("==== can't open (w) the file %s! \n",FileName );
		return 0;
	}
	else
	{
		/* printf ( "\n==== open (w) %s  success! \n",FileName  ); */
	}
	while ( fscanf(fp,"%s",StrTmp)!=EOF )
	{
		if(strcmp(StrTmp,"BEGIN")==0) break;
	}
	i = 0;
	while ( fscanf(fp,"%s",StrTmp)!=EOF )
	{
		if(strcmp(StrTmp,"END")==0)
		{
			break;
		}
		fscanf(fp,"%s",ip[i].ip);
		fscanf(fp,"%s",ip[i].logicalname);
		ip[i].status = -1;
		ip[i].lasttime  = now;
		i++;
	}
	ipnum = i;
	fclose(fp);	
	return TRUE;
}