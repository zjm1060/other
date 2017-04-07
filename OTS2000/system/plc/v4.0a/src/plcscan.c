/************************************************************************************************************************************************
FILENAME:	plcscan.c
FUNCTION:	
	
	include the main() function of plc 
	communication module

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
2005.11.1	start to modify the code for H9000 V4.0		yyp
************************************************************************************************************************************************/

#include    "../inc/plchead_all.h"

#ifdef MSWIN32
 DWORD		ThreadID[MAX_STN_NUM+1][MAX_LCU_NUM+1][MAX_THR_NUM+1];
 HANDLE		hThreadID[MAX_STN_NUM+1][MAX_LCU_NUM+1][MAX_THR_NUM+1];
 WSADATA	wsaData;	
#endif
#ifndef MSWIN32
 pthread_t	ThreadID[MAX_STN_NUM+1][MAX_LCU_NUM+1][MAX_THR_NUM+1],tid_terminated;
 pid_t		proc_id[MAX_STN_NUM+1][MAX_LCU_NUM+1];
 pid_t		exit_proc_id; 
 int		proc_plc_id[MAX_STN_NUM+1][MAX_LCU_NUM+1]; 
#endif
void sig_termp();
void sig_maperr();
int  childflag=0;
unsigned char hostid;
/************************************************************************************************************************************************
the main() function of "QUANTUM" plc communication module include:
initiate the function;create and maintain the thread
of each LCU	modified by yyp 2005.11.1
************************************************************************************************************************************************/

int main(int argc, char *argv[])
{
	char		devname[12];
	char		typestr[20];
	unsigned char	idx,devid,thrid;
	char	log_str[256];
	wsa_startup();

	printf("\n========================= plcscan.dxe start running ==========================\n");

#ifndef MSWIN32
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
	signal(SIGSEGV,sig_maperr);	
#endif
	CreateLockFile(".plcscan.dxe");
	SetLogLevel(argc, argv);
	if(-1==InitSysLog(H9000_MODULE_PLCSCAN, "PlcScan"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	if(MountDB(DB_NAME) == -1)
	{
		err_write("==== plcscan.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}

	gethostname(hostname,20);
	GetSrcId(&hostid);

	if(lan_init_all() == FALSE)
	{
		err_write("==== plcscan.c: call Lan_init_all() error\n");		
		exit(0);
	}

	if( InitPlcDataShm(1) == FALSE) 
	{
		err_write("==== plcscan.c: call initPlcDataShm(1) error\n");
		exit(0);
	}
	
	if(GetPlcConf(1)==0)	
	{
		err_write("\n==== plcscan.c: GetPlcConf() error!\n");
		exit(0);
	}
	else
	{
		log_write(1,1,"\n==== GetPlcConf() successfully!\n");
	}
	if ( g_cascade != YES )
	{
		//for SIEMENS plc
		if(g_plc_mode == MIXPLC || g_plc_mode == SIEPLC)
		{
			if(ReadPortCfg() ==1)
			{
				log_write(1,1,"\n==== ReadPortCfg() successfully!\n");
     
			}
			else
			{
				err_write("\n==== ReadPortCfg error!\n");		
				exit(0);
			}
		}
		// for AB PLC
		g_plc_mode = ABPLC;
		if(g_plc_mode == MIXPLC || g_plc_mode == ABPLC)
		{	
			InitCIPRequest();
		}
	}
	for(idx=1;idx<MAX_STN_NUM;idx++)
	{
	    for(devid=1;devid<MAX_LCU_NUM;devid++)
	    {
			for(thrid=1;thrid<MAX_THR_NUM;thrid++)
			{
				ThreadID[idx][devid][thrid] = 0;
				/* hThreadID[idx][devid][thrid] = 0; */
			}		
	   }
	}
#ifndef MSWIN32
	for(idx=1;idx<MAX_STN_NUM;idx++)
	{
	    for(devid=1;devid<MAX_LCU_NUM;devid++)
	      {
			proc_id[idx][devid] = 0;
			proc_plc_id[idx][devid] = 0;
	      }
	}
#endif

	/******* begin to loop,create several thread of each LCU *******/ 
	for(idx=1;idx<=g_stn_num;idx++)
	{
		UCHAR stnid;
		GetStnIdByIndex(idx, &stnid);
		for(devid=1;devid<=g_dev_num[idx];devid++)
		{
#ifndef MULTIDEV			
			stnid=1;
			idx=1;
			devid=1;
			//plconf[idx][devid].plctype = ABPLC;
			//plconf[idx][devid].port = 44818;
#endif
		  	if(GetGroupNameById(stnid,devid,devname)==-1) continue;
			if(plconf[idx][devid].valid!=1)	continue;
						
			if(strcmp(devname,"")==0) 	continue;

			GetPlcTypeStr(stnid, devid, typestr);
			if(strcmp(typestr,"SYS") == 0 || strcmp(typestr,"RTP") == 0)
			{
				continue;
			}
			
			if((plconf[idx][devid].plctype == MBPLC)
				|| (plconf[idx][devid].plctype == PREPLC)
				|| (plconf[idx][devid].plctype == SIEPLC)
				|| (plconf[idx][devid].plctype == GEPLC)
				|| (plconf[idx][devid].plctype == ABPLC))
			{
				/****** create new child process for each plc ******/
#ifndef MSWIN32
#ifdef MULTIDEV
				proc_id[idx][devid] = fork();
#endif
				if( proc_id[idx][devid] == 0 )
				{				
					childflag = 1; 
					ChildProc(stnid,devid);
					exit(0); //added by wzg 20111118
				}
				sprintf(log_str,"==== plc %d.%d child process id= %d running...", stnid,devid, proc_id[idx][devid]); 	
				log_printf(H9000_LOG_INFO,1,0,log_str);
#else
				ChildProc(stnid,devid);	
#endif				
			}/* end if( MyStrCmp()==0 )*/
#ifndef MULTIDEV
			break;
#endif			
		}/* end for */
#ifndef MULTIDEV
		break;
#endif					
	}

	/****** loop to maintain the proc of each LCU,recreate it while missing ******/

#ifndef MSWIN32
	for(;;)
	{
		UCHAR stnid;

		exit_proc_id= wait((int*)NULL);
		if(-1==exit_proc_id || exit_proc_id==0 )
		{
			sprintf(log_str,"==== plcscan.c: failed to wait exited process ,id= %d \n",exit_proc_id); 
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			MySleep(2000);
			continue;
		}
		else
		{
			sprintf(log_str,"==== plcscan.c: get exited process id= %d \n",exit_proc_id); 
			log_printf(H9000_LOG_ERROR,1,0,log_str);
		}
		for(idx=1;idx<=g_stn_num;idx++)
		{
			GetStnIdByIndex(idx, &stnid);
			idx = idx;
			for(devid=1;devid<=g_dev_num[idx];devid++)
			{
				if(exit_proc_id == proc_id[idx][devid])
					break;
			}
			if(exit_proc_id == proc_id[idx][devid])
				break;			
		}
		//wzg20111118 if(idx>=g_stn_num && devid >= g_dev_num[idx]) ???
		if(idx>=g_stn_num || devid >= g_dev_num[idx])
		{
			sprintf(log_str,"==== plcscan.c: invalid exited process \n"); 
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			continue;
		}

		proc_id[idx][devid] = -1;
		
		sprintf(log_str,"==== plcscan.c: plc %d.%d scan process exited! \n",stnid,devid); 
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		proc_id[idx][devid]= fork();
		if(proc_id[idx][devid] == 0)
		{
			childflag = 1; 
			ChildProc(stnid,devid);		
			exit(0); //added by wzg 20111118
		}
		sprintf(log_str,"==== plcscan.c: plc %d.%d scan process reboot,procid=%d! \n",stnid,devid,proc_id[idx][devid]); 
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}
#endif	

	for(;;)
	{
		MySleep(2000);
	}
	//wzg20111118 sprintf(log_str,"==== plcscan.c: plc %d.%d scan main process exit! \n",stnid,devid); 
	//log_printf(H9000_LOG_INFO,1,0,log_str);
#ifdef MSWIN32
	WSACleanup();
#endif	
	UnMountDB();
	return TRUE;
}




/************************************************ Child Process function of each PLC ***********************************************************************************/
int ChildProc(unsigned char stnid,unsigned char devid)
{
	THRARG		arg[MAX_THR_NUM];
	unsigned char thrid,idx;
#ifndef MSWIN32
	LCUID		plcid;
	int 		res;
#endif
	char	log_str[256];
	for(thrid=1;thrid<MAX_THR_NUM;thrid++ )
	{
		arg[thrid].stnid=stnid;
		arg[thrid].devid=devid;
	}
	idx = index_stn[stnid];
 	/******* Create Thread for MSWIN32  **********/
#ifdef MSWIN32
	/****** create the data scanning thread ******/
	MySleep(20);
	for(thrid=THR_HEAD;thrid<MAX_THR_NUM;thrid++)
	{
		MySleep(2000);			
		arg[thrid].thrtype = thrid;
		if(thrid == THR_HEAD)
		{
#ifdef MULTITHREAD
			hThreadID[idx][devid][arg[thrid].thrtype]=CreateThread(NULL,
					MAX_STACK_SIZE, 
					(LPTHREAD_START_ROUTINE)ThreadScanH, 
		 			&arg[thrid],
		 			0,
		 			(LPDWORD)&ThreadID[idx][devid][arg[thrid].thrtype]);

#else
			ThreadScanH(&arg[thrid]);
			MySleep(2000);		/* modified by yyp 2008.12.08 */
			return TRUE;
#endif
		}
		else
		{
			if(thrid !=THR_ANA && thrid !=THR_IMP && thrid !=THR_POL && thrid !=THR_SYSMSG
				&& thrid !=THR_INTALM && thrid !=THR_WRITEDATA)	
			{
				continue;
			}
			hThreadID[idx][devid][arg[thrid].thrtype]=CreateThread(NULL,
					MAX_STACK_SIZE, 
					(LPTHREAD_START_ROUTINE)ThreadScan, 
		 			&arg[thrid],
		 			0,
		 			(LPDWORD)&ThreadID[idx][devid][arg[thrid].thrtype]);
		}
		if(hThreadID[idx][devid][arg[thrid].thrtype]!=NULL)
		{
			sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d,thrtype=%d,thr_id=%d OK",
				arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype,(int)ThreadID[idx][devid][arg[thrid].thrtype]);	
			log_printf(H9000_LOG_INFO,1,0,log_str);
		}
		else
		{
			sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d, thrtype=%d failed", arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
		}
	}
			
	/******* Create Thread for SUN  *********/
#else
	MySleep(20);
	proc_plc_id[idx][devid] = getpid();

	/****** create the head  data scanning thread ******/
	thrid = THR_HEAD;
	arg[thrid].thrtype = thrid;
#ifdef MULTITHREAD
 res =   pthread_create(&ThreadID[idx][devid][arg[thrid].thrtype], NULL, ThreadScanH, (void *)&arg[thrid]);

#else
	ThreadScanH(&arg[thrid]);
#endif       				
	if (!res) 
	{	    
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d,thrtype=%d,thr_id=%d OK",
			arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype,(int)ThreadID[idx][devid][arg[thrid].thrtype]);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d, thrtype=%d failed", arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
	}
	MySleep(20);	


	thrid = THR_ANA;
	arg[thrid].thrtype = thrid;				
	res =   pthread_create(&ThreadID[idx][devid][arg[thrid].thrtype], NULL, ThreadScan, (void *)&arg[thrid]);
	if (!res ) 
	{	    
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d,thrtype=%d,thr_id=%d OK",
			arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype,(int)ThreadID[idx][devid][arg[thrid].thrtype]);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d, thrtype=%d failed", arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
	}
	MySleep(20);	

	thrid = THR_IMP;
	arg[thrid].thrtype = thrid;
	res =   pthread_create(&ThreadID[idx][devid][arg[thrid].thrtype], NULL, ThreadScan, (void *)&arg[thrid]);
	if (!res ) 
	{	    
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d,thrtype=%d,thr_id=%d OK",
			arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype,(int)ThreadID[idx][devid][arg[thrid].thrtype]);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d, thrtype=%d failed", arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
	}
	MySleep(20);

	thrid = THR_SYSMSG;
	arg[thrid].thrtype = thrid;
	res =   pthread_create(&ThreadID[idx][devid][arg[thrid].thrtype], NULL, ThreadScan, (void *)&arg[thrid]);
	if (!res ) 
	{	    
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d,thrtype=%d,thr_id=%d OK",
			arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype,(int)ThreadID[idx][devid][arg[thrid].thrtype]);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d, thrtype=%d failed", arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
	}
	
	MySleep(20);

	thrid = THR_INTALM;
	arg[thrid].thrtype = thrid;
	res =   pthread_create(&ThreadID[idx][devid][arg[thrid].thrtype], NULL, ThreadScan, (void *)&arg[thrid]);
	if (!res ) 
	{	    
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d,thrtype=%d,thr_id=%d OK",
		arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype,(int)ThreadID[idx][devid][arg[thrid].thrtype]);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d, thrtype=%d failed", arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
	}
	
	MySleep(20);

	thrid = THR_POL;
	arg[thrid].thrtype = thrid;				
	res =   pthread_create(&ThreadID[idx][devid][arg[thrid].thrtype], NULL, ThreadScan, (void *)&arg[thrid]);
	if (!res) 
	{	    
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d,thrtype=%d,thr_id=%d OK",
			arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype,(int)ThreadID[idx][devid][arg[thrid].thrtype]);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d, thrtype=%d failed", arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
	}

	MySleep(20);
	thrid = THR_WRITEDATA;
	arg[thrid].thrtype = thrid;
	res =   pthread_create(&ThreadID[idx][devid][arg[thrid].thrtype], NULL, ThreadScan, (void *)&arg[thrid]);
	if (!res) 
	{	    
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d,thrtype=%d,thr_id=%d OK",
			arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype,(int)ThreadID[idx][devid][arg[thrid].thrtype]);	
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d, thrtype=%d failed", arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
	}
	plcid.stnid = stnid;
	plcid.devid = devid;
	for(;;)
	{
		MySleep(10000);
	}
	
/*
	while( !thr_join(0, &tid_terminated, NULL) )
	{
		sleep(1);

		for(thrid=1;thrid<MAX_THR_NUM;thrid++)
		{
			if(tid_terminated==ThreadID[idx][devid][thrid])
				break;
		}

		arg[thrid].thrtype = thrid;
		res =   thr_create(NULL, MAX_STACK_SIZE, ThreadScan, (void *)&arg[thrid], 0, &ThreadID[idx][devid][thrid]);
		if (res != -1  ) 
		{	    
			sprintf(log_str,"==== plcscan.c: Create thread plc %d.%d,thrtype=%d,thr_id=%d OK\n",
				arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype,(int)ThreadID[idx][devid][arg[thrid].thrtype]);	
			log_printf(H9000_LOG_INFO,1,0,log_str);
		}		
		else
		{ 
			sprintf(log_str,"==== plcscan.c Create thread plc %d.%d, thrtype=%d failed\n", arg[thrid].stnid,arg[thrid].devid,arg[thrid].thrtype);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
		}
	}	
*/
#endif
	for(;;)
	{
#ifndef MSWIN32
		pause();
#else
		MySleep(1000);
		break;
#endif
	}					
	return TRUE;
}

void sig_termp()
{
#ifndef MSWIN32
	unsigned char idx,devid;
	if(childflag==0)
	{
		for(idx=1; idx <= g_stn_num; idx++)
		{
			UCHAR stnid;
			GetStnIdByIndex(idx, &stnid);
			for(devid=1;devid<=g_dev_num[idx];devid++)
			{
		 		if( proc_id[idx][devid] != 0 )	kill( proc_id[idx][devid], SIGINT );
			}
		}
		printf("plcscan.c: main process exit...\n");	
	}
	else
	{
		printf("plcscan.c: child process %d exit...\n",getpid());
	}
	closesocket(g_sock);
#endif
	exit(0);
}

void sig_maperr()
{
	printf("plcscan.c: get SIGSEGV signal, reason maybe mem addr error!\n");
	printf("plcscan.c: child process %d exit...\n",getpid());
	sig_termp();
	exit(0);
}
