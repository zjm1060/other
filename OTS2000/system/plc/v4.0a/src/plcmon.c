/************************************************************************************************************************************************
FILENAME:	plcmon.c
FUNCTION:	
	
		monitor and test the plc communication state of each cpu, each net, each plc

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
2006.4.19	create the file for H9000 V4.0		yyp
************************************************************************************************************************************************/

#include "../../../lan/v4.0a/inc/lan.h"
#include "../../../cmd/v4.0a/inc/cmd_functions.h"
#include "../inc/plc_com.h"

#ifdef MSWIN32
 DWORD		threadsend,threadrecv,wait_thrid;
 WSADATA	wsaData;	
#endif
#ifndef MSWIN32
 pid_t		proc_id[MAX_STN_NUM+1][MAX_LCU_NUM+1][4];
 pthread_t	threadsend,threadrecv,tid_terminated,wait_thrid;
  pid_t		exit_proc_id;
#endif
int ChildThread(); 
void * ThreadSend( void * arg );
void * ThreadRecv( void * arg );
void sig_termp();
void kill_child();
void InitIpDef();
int  childflag=0;

SOCKET    sockRaw = INVALID_SOCKET;

/************************************************************************************************************************************************
the main() function of "QUANTUM" plc communication module include:
initiate the function;create and maintain the thread
of each LCU	modified by yyp 2005.11.1
************************************************************************************************************************************************/

int main(int argc, char *argv[])
{
	char		hostname[20];		/* hostname of local machine */
	unsigned char	stnid,idx,devid;
	short		OldScanPrvg,CurScanPrvg;
	int			FirstVice=TRUE;
	char		log_str[256];

	CreateLockFile(".plcmon.dxe");
	SetLogLevel(argc, argv);
	wsa_startup();

	printf("\n========================= plcmon.dxe start running ==========================\n");
#ifndef MSWIN32	
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
#endif
	g_stn_num =1;
	IsPlcmonHost = FALSE;
	FirstLoop = YES;
	CurScanPrvg=CUR_WS_IN_VICE;
	gethostname(hostname,20);

	if(-1==InitSysLog(H9000_MODULE_PLCMON, "PlcMon"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}

	if(MountDB(DB_NAME) == -1)
	{
		printf("==== plcmon.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	if(lan_init_all() == FALSE)
	{
		printf("==== plcmon.c: call Lan_init_all() error\n");		
		exit(0);
	}

	if( InitPlcDataShm(1) == FALSE) 
	{
		printf("==== plcmon.c: call initPlcDataShm(1) error\n");
		exit(0);
	}
	/* added by yyp 07.08.07 */	

	if(GetPlcConf(1)==0)	
	{
		printf("\n==== plcmon.c: GetPlcConf() error!\n");
		exit(0);
	}
	else
	{
		printf("\n==== GetPlcConf() successfully!\n");
	}	
	
	InitIpDef();

#ifdef MSWIN32 
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
			/* removed by yyp 2011.01.10
			if(childflag == 0)
			{
				kill_child();
			}
			sprintf(log_str,"plcmon.c: all child process  exit...\n");
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			*/
			MySleep(2000);
			sprintf(log_str,"==== %s is not the station %d primary plc monitor server\n", hostname,stnid);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
		}
		else if(CurScanPrvg == CUR_WS_IN_HOST && CurScanPrvg != OldScanPrvg)
		{

			
			sprintf(log_str,"==== %s is the station %d primary plc monitor server\n", hostname,stnid);
			log_printf(H9000_LOG_ERROR,1,0,log_str);			
			FirstLoop = YES;
			FirstVice = NO;
// 			for(stnid=1;stnid<=g_stn_num;stnid++)
// 			{
// 				if(stnid>1) break;
				BdPlcMonHost( stnid );
				ChildThread();
// 			}/* end for */
			//break;
		}
		else
		{
			if(FirstVice == YES)
			{
				FirstVice = NO;
				sprintf(log_str,"==== %s is not the station %d primary plc monitor server\n", hostname,stnid);
				log_printf(H9000_LOG_ERROR,1,0,log_str);
			}
		}
	}/* end for */

	for(;;)
	{
		MySleep(1000);
	}

	sprintf(log_str,"==== plcmon.c: plc %d.%d mon main process exit! \n",stnid,devid); 
	log_printf(H9000_LOG_INFO,1,0,log_str);
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

#ifndef 	PLCMON_MULTITHREAD	
	ThreadSend(NULL);
#endif

#ifndef MSWIN32
/*
	res =   thr_create(NULL, 0, ThreadSend, NULL, 0, &threadsend);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== plcmon.c: Create send thread thr_id=%d OK\n",
			(int)threadsend);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcmon.c: Create send thread failed\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;
	}
*/
	res =   pthread_create(&threadsend, NULL, ThreadSend, NULL);
	if (!res) 
	{	    
		sprintf(log_str,"==== plcmon.c: Create send thread thr_id=%d OK\n",
			(int)threadsend);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcmon.c: Create send thread failed\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;
	}
	MySleep(1000);
	
/*
	res =   thr_create(NULL, 0, ThreadRecv, NULL, 0, &threadrecv);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== plcmon.c: Create recv thread thr_id=%d OK\n",
			(int)threadrecv);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcmon.c: Create recv thread failed\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;
	}
*/
	res =   pthread_create(&threadrecv, NULL, ThreadRecv, NULL);
	if (!res ) 
	{	    
		sprintf(log_str,"==== plcmon.c: Create recv thread thr_id=%d OK\n",
			(int)threadrecv);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcmon.c: Create recv thread failed\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;
	}
#else
	res=CreateThread(NULL,	0,	(LPTHREAD_START_ROUTINE) ThreadSend, 
		 		NULL,	0,	(LPDWORD)&threadsend);

	if(res!=NULL)
	{
		sprintf(log_str,"==== plcmon.c: Create sned thread thr_id=%d OK\n",
			(int)threadsend);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}
	else
	{
		sprintf(log_str,"==== plcmon.c: Create thread failed\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;	    
	}
	MySleep(1000);
	res=CreateThread(NULL,	0,	(LPTHREAD_START_ROUTINE) &ThreadRecv, 
		 		NULL,	0,	(LPDWORD)&threadrecv);

	if(res!=NULL)
	{
		sprintf(log_str,"==== plcmon.c: Create recv thread thr_id=%d OK\n",
			(int)threadrecv);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}
	else
	{
		sprintf(log_str,"==== plcmon.c: Create thread failed\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;	    
	}
#endif

	MySleep(100);	
	return TRUE;
}




void sig_termp()
{
#ifndef MSWIN32
	char		log_str[256];
	unsigned char idx,devid,i;
	sprintf(log_str,"plcmon.c: process %d get exit signal...\n",getpid());
	log_write(H9000_LOG_ERROR,1,log_str);	
	if(childflag==0)
	{
		for(idx=1; idx <= g_stn_num; idx++)
		{
			UCHAR stnid;
			GetStnIdByIndex(idx, &stnid);
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
	char		log_str[256];
	unsigned char idx,devid,i;
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

