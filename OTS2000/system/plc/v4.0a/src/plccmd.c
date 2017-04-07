/**************************************************************************************************************************************************
FILENAME:	plccmd.c
FUNCTION:	
	
	include the main() function of plc 
	command module

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
2008.6.27  upgraded for AB								yyp
2005.11.1	start to modify the code for H9000 V4.0		yyp
***************************************************************************************************************************************************/

#include    "../inc/plchead_all.h"

#ifdef MSWIN32
 DWORD		ThreadID[MAX_STN_NUM+1][MAX_LCU_NUM+1];
 HANDLE		hThreadID[MAX_STN_NUM+1][MAX_LCU_NUM+1];
 WSADATA	wsaData;	
#endif
#ifndef MSWIN32
 pid_t		proc_id[MAX_STN_NUM+1][MAX_LCU_NUM+1];
 pid_t		exit_proc_id;
 int		proc_plc_id[MAX_STN_NUM+1][MAX_LCU_NUM+1]; 
#endif
int  childflag=0;
void sig_termp();
void sig_maperr();
/**************************************************************************************************************************************************
the main() function of "QUANTUM" plc communication module£¬include:
initiate the function;create and maintain the thread
of each LCU¡£	modified by yyp 2005.11.1
***************************************************************************************************************************************************/

int main(int argc, char *argv[])
{
	char		hostname[20];		/* hostname of local machine */
	char		devname[12];
	unsigned char	idx,devid;
	THRARG		arg;
	char		typestr[20];	
	char log_str[256];
	wsa_startup();

	CreateLockFile(".plccmd.dxe");
	SetLogLevel(argc, argv);
	if(-1==InitSysLog(H9000_MODULE_PLCCMD, "PlcCmd"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	printf("\n========================= plccmd.dxe start running ==========================\n");

	gethostname(hostname,20);
#ifndef MSWIN32	
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
	signal(SIGSEGV,sig_maperr);			
#endif
	if(MountDB(DB_NAME) == -1)
	{
		printf("==== plccmd.c: call MountDB(DATABASE_NAME) error\n");
		exit(0);
	}
	
	if(lan_init_all() == FALSE)
	{
		printf("==== plccmd.c: call lan_init_all() error\n");		
		exit(0);
	}

	if( InitPlcDataShm(1) == FALSE) 
	{
		sprintf(log_str,"==== plccmd.c: call initPlcDataShm(1) error\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);	
		exit(0);
	}
	
	if(GetPlcConf(1)==0)	
	{
		sprintf(log_str,"\n==== plccmd.c: GetPlcConf() error!\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		exit(0);	
	}
	else
	{
		sprintf(log_str,"\n==== GetPlcConf() successfully!\n");
		log_printf(H9000_LOG_INFO,1,0,log_str);
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
		if(g_plc_mode == MIXPLC || g_plc_mode == ABPLC)
		{	
			InitCIPRequest();
		}
	}
	/* begin to loop,create two thread of each LCU */ 
	for(idx=1;idx<=g_stn_num;idx++)
	{
		UCHAR stnid;
		GetStnIdByIndex(idx, &stnid);
#ifndef MULTIDEV
		if(stnid>1)
		{
			break;
		}
#endif
		for(devid=1;devid<=g_dev_num[idx];devid++)
		{

#ifndef MULTIDEV			
			if(devid>1)
			{
				break;
			}
			idx=1;
			devid=20;
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
				
				MySleep(10);
				arg.stnid=stnid;
				arg.devid=devid;
				
				/******* Create Thread for MSWIN32  *******/
#ifdef MSWIN32

				/****** create the write plc command thread ******/
				MySleep(1000);			
				hThreadID[idx][devid]=CreateThread(NULL,
		 					MAX_STACK_SIZE, 
		 					(LPTHREAD_START_ROUTINE)ThreadCmd, 
		 					&arg,
		 					0,
		 					(LPDWORD)&ThreadID[idx][devid]);
				if(hThreadID[idx][devid]==NULL)
				{	
					sprintf(log_str,"==== plccmd.c: Creat plc cmd thread failed plcid=%d.%d  errorcode= %d\n",stnid,devid,GetLastError());
					log_printf(H9000_LOG_ERROR,1,0,log_str);
				}
				else
				{
					sprintf(log_str,"==== plccmd.c: Create plc cmd thread: plcid=%d.%d ok\n",stnid,devid,ThreadID[idx][devid]);
					log_printf(H9000_LOG_INFO,1,0,log_str);
				}
				
				/******* Create child process for SUN  *******/
#else
#ifdef MULTIDEV 
				proc_id[idx][devid] = fork();
				if( proc_id[idx][devid] == 0 )
				{
#endif					
					childflag = 1; 		
					arg.stnid=stnid;
					arg.devid=devid;
					arg.thrtype = THR_CMD;
					ThreadCmd( (void *)  &arg );
#ifdef MULTIDEV
				}
#endif				
				sprintf(log_str,"==== plccmd.c: plc cmd child process id= %d running,plcid=%d.%d...\n", proc_id[idx][devid],stnid,devid); 
				log_printf(H9000_LOG_ERROR,1,0,log_str);
#endif				

			}/* end if */
		}/* end for */
	}
	
	sprintf(log_str,"==== plccmd.c: start loop to wait...\n");	
	log_printf(H9000_LOG_INFO,1,0,log_str);
	
	/****** loop to maintain the thread of each LCU,recreate it while missing ******/
#ifndef MSWIN32
	for(;;)
	{
		UCHAR stnid;

		exit_proc_id= wait((int*)NULL);
		if(-1==exit_proc_id || exit_proc_id==0 )
		{
			sprintf(log_str,"==== plccmd.c: failed to wait exited process ,id= %d \n",exit_proc_id); 
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			MySleep(2000);
			continue;
		}
		else
		{
			sprintf(log_str,"==== plccmd.c: get exited process id= %d \n",exit_proc_id); 
			log_printf(H9000_LOG_ERROR,1,0,log_str);
		}
		for(idx=1;idx<=g_stn_num;idx++)
		{
			GetStnIdByIndex(idx, &stnid);

			for(devid=1;devid<=g_dev_num[idx];devid++)
			{
				if(exit_proc_id == proc_id[idx][devid])
					break;
			}
			if(exit_proc_id == proc_id[idx][devid])
				break;			
		}
		if(idx>=g_stn_num || devid >= g_dev_num[idx])
		{
			sprintf(log_str,"==== plccmd.c: invalid exited process \n"); 
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			continue;
		}

		proc_id[idx][devid] = -1;
		
		sprintf(log_str,"==== plccmd.c: plc %d.%d cmd process exited! \n",stnid,devid); 
		log_printf(H9000_LOG_ERROR,1,0,log_str);

		proc_id[idx][devid]= fork();
		if(proc_id[idx][devid] == 0)
		{
			childflag = 1; 
			arg.stnid=stnid;
			arg.devid=devid;
			arg.thrtype = THR_CMD;
			ThreadCmd( (void *)  &arg );			
		}
		sprintf(log_str,"==== plccmd.c: plc %d.%d cmd process reboot,procid=%d! \n",stnid,devid,proc_id[idx][devid]); 
		log_printf(H9000_LOG_INFO,1,0,log_str);
	}
#endif

	for(;;)
	{
#ifdef SUN
		pause();
#else
		MySleep(1000);
#endif
	}
	//sprintf(log_str,"==== plccmd.c: plc %d.%d cmd main process exit! \n",stnid,devid); 
	//log_printf(H9000_LOG_INFO,1,0,log_str);

	/****** release the resources ******/

#ifdef MSWIN32
	WSACleanup();
#endif	
	UnMountDB();
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
			for(devid=1;devid<=g_dev_num[idx];devid++)
			{
		 		if( proc_id[idx][devid] != 0 )	kill( proc_id[idx][devid], SIGINT );
			}
		}
		printf("plccmd.c: main process exit...\n");	
	}
	else
	{
		printf("plccmd.c: child process %d exit...\n",getpid());
	}
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
