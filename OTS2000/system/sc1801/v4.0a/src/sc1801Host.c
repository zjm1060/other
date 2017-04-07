
/************************************************************
*  FILE NAME:           sc1801Host.C      					*
*  FUNCTION:            Controlling Station Main Process	*
*						Link Layer Communication Program	*
*  NAME :  					        						*
*  DATE:                May,2000                			*
************************************************************/


#include	"..\inc\sc1801Host.h"

#define	DEBUG

/*extern 	HANDLE ComInit(char *COM,DWORD Baud_Rate,
					   BYTE ByteSize,BYTE Parity,BYTE StopBits);*/
/*void	Lan_init(void);*/
void	SC1801_Recv(void);
void	SC1801_Send(void);
BOOL	RequestRTUConfig(void);
BOOL	RepeatTransmit(void);
BOOL	CommandSend();
BOOL	ClockSynchronisation(void);
BOOL	RequestFullData(void);
BOOL	RequestExceptData(void);
void	ClearRecvFault(void);
void	PrintRecvData();
BOOL	SimuCommandSend();

BOOL	GetRTUConfig();
void	GetDBNameRangeTable();		

extern	ASDUCOMDDATA	send_data;
extern	ASDUDATA		recv_data;

#ifdef SUN_OS
extern	int	hPort_com;
#else
HANDLE	hPort_com;
HANDLE	hRecvSemaphore;
#endif

extern	char	infobuf[1024];
unsigned char recvbuf[1024];
unsigned char sendbuf[1024];
unsigned char comd_id,send_comd_id,rtu_status,rtu_jumpter;
/*DWORD	dwErrors;
COMSTAT	cstat;*/
unsigned char	rtuconfig[16];
unsigned char	pntname[16][24][36];
float	lorange[16][12],hirange[16][12];
unsigned short	databand,bytecount,retry,sst_ms;

time_t	tim;
SYSTEMTIME sys_tm;
struct tm *timeptr;

BOOL	resetlinkf,initflag,comdflag,pollflag,RequestFullDataflag;
BOOL	counterflag,ClockSyncflag,interrogationflag;
unsigned int	repeatnum,timeoutnum;
int		cmd_ack,localinitfg;

#ifdef SUN_OS
void Endproc(int sig_no)
{
	int i;

	if( getpid()==sProcRec[MAX_PROC_NUM].Pid )
	{
		sprintf(infobuf,"  ---- sc1801host: exit signal %d received!  ----\n",sig_no);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		for(i=0;i<ProcNum;i++)
		{
			if(sProcRec[i].Pid != -1)
			{
				sprintf(infobuf," kill ---- %s sc1801host: ! ( pid=%05d ) ----\n",sProcRec[i].NameID,sProcRec[i].Pid);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				kill( sProcRec[i].Pid, SIGTERM);
				UpdateChannelFlag(i,(unsigned short)1);
			}
		}
	}
	else
	{
		UpdateChannelFlag(0,(unsigned short)1);
		sprintf(infobuf,"  ---- sc1801host: exit signal %d received! Exit ----\n",sig_no);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}
	UnMountDB();
	lan_finish(0);
	exit(0);
} 
                                                                
void Timer(int sig_no)
{
	signal(SIGALRM,Timer);
/*	timeout=YES; */
}

void Waitchild(int nouse)
{
	int status,wpid,i;

	sprintf(infobuf,"  ---- sc1801host Master in Waitchild() !!!\n");
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

	alarm(1);
	signal(SIGALRM,Timer);
	wpid = wait(&status);
	alarm(0);
	if(wpid != -1)
	{
		for(i=0;i<ProcNum;i++)
		{
			if(sProcRec[i].Pid == wpid)
			{
				sprintf(infobuf,"  ---- %s sc1801host: stop! ( pid=%05d ) ----\n",sProcRec[i].NameID,sProcRec[i].Pid);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				sProcRec[i].Pid = -1;
				break;
			}
		}
	}
	signal(SIGCHLD,Waitchild); 
}
#endif



int	main(int argc, char *argv[])
{
#ifdef SUN_OS
	int	hThread_Recv;
	int	hThread_Send;
	thread_t	ThreadID_Recv,ThreadID_Send,hThread_ret;
	ssize_t		byte_recvd;
	struct timeval	tout;
	time_t	tim;
#else
	HANDLE	hThread_Recv,hThread_Send;
	int		ThreadID_Recv,ThreadID_Send;
	DWORD	byte_recvd;
#endif
	BOOL	fSuccess,dataflow;					  	
	//BOOL	dataflag;
	LONG	cMaxCount,cInitCount;
	unsigned short	datacount;
	

	unsigned int	sendbufheadpt,recvbufheadpt;
	unsigned char	chksum,ctrlfield;
	unsigned int	i;

	SetModuleName("SC1801");

	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}

	if(lan_out_init() == FALSE)
	{
		printf("\nFail to lan_out_init()!");
		exit(-1);
	}

#ifdef SUN_OS
		signal(SIGTERM,Endproc);
		signal(SIGINT,Endproc);
#endif

	GetRTUConfig();  //Get modules type of slots
	GetDBNameRangeTable();	//Get database name of each point

	send_data.pnt = 0;
	recv_data.pnt = 0;
	recvbufheadpt = 0;
	sendbufheadpt = 0;
	cmd_ack = 0;
	timeoutnum = 0;
	localinitfg=1;
	//GenDataTable();

	cMaxCount= 10;
	cInitCount = 0;

	for(;;)
	{
		
		
		hPort_com = ComInit(COM_PORT1,Com_Baud_Rate,8,ODD,0);
		if(hPort_com < 0)
		{
			sprintf(infobuf,"\nsc1801host: device %s init failed!!! Exit now!!!\n\n",COM_PORT1);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			sleep(3);
			
			continue;
		}
		sprintf(infobuf,"\nsc1801host: device %s init OK!!!!!!\n\n",COM_PORT1);
 			printf("%s",infobuf);
		break;
	}

#ifdef SUN_OS
	hThread_Recv = thr_create(NULL, 0, SC1801_Recv, NULL, 0, &ThreadID_Recv);
	if (hThread_Recv != 0)
	{
		sprintf(infobuf,"sc1801host:Error in thr_create ThreadID_Recv:%d error: %d!\n ", ThreadID_Recv, errno);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		exit(-1);
	}
	else
	{
		sprintf(infobuf,"sc1801host:thr_create ThreadID_Recv: %d OK!\n",hThread_Recv);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}

	hThread_Send = thr_create(NULL, 0, SC1801_Send, NULL, 0, &ThreadID_Send);
	if (hThread_Send != 0)
	{
		sprintf(infobuf,"sc1801host:Error in thr_create ThreadID_Send:%d error: %d!\n ", ThreadID_Send, errno);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		exit(-1);
	}
	else
	{
		sprintf(infobuf,"sc1801host:thr_create ThreadID_Send: %d OK!\n",hThread_Send);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}
		while( !thr_join(0, &hThread_ret, NULL) )
	{
		if( hThread_ret==ThreadID_Recv )
		{
			sprintf(infobuf,"sc1801host: restart thread ThreadID_Recv %d\n", hThread_ret);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			hThread_Recv = thr_create(NULL, 0, SC1801_Recv, NULL, 0, &ThreadID_Recv);
			if (hThread_Recv != 0)
			{
				sprintf(infobuf,"sc1801host:Error in thr_create ThreadID_SOE:%d error: %d!\n ", ThreadID_Recv, errno);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				exit(-1);
			}
			else
			{
				sprintf(infobuf,"sc1801host:thr_create ThreadID_Recv: %d OK!\n",hThread_Recv);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}
		}
		if( hThread_ret==ThreadID_Send )
		{
			sprintf(infobuf,"sc1801Send: restart thread ThreadID_Send %d\n", hThread_ret);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			hThread_Send = thr_create(NULL, 0, SC1801_Send, NULL, 0, &ThreadID_Send);
			if (hThread_Send != 0)
			{
				sprintf(infobuf,"sc1801Send:Error in thr_create ThreadID_Send:%d error: %d!\n ", ThreadID_Send, errno);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				exit(-1);
			}
			else
			{
				sprintf(infobuf,"sc1801Send:thr_create ThreadID_Send: %d OK!\n",hThread_Send);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}
		}
	}
#else
	/*hPort_com=ComInit(COM_PORT1,Com_Baud_Rate,8,EVENPARITY,ONESTOPBIT);
	printf("\n %s init compelete !!!\n\n",COM_PORT1);*/

	hRecvSemaphore = CreateSemaphore(NULL,cInitCount,cMaxCount,"sem_recv");

	if (hRecvSemaphore == NULL)
	{	sprintf(infobuf,"Error in CreateSemaphore sem_recv:%d\n",GetLastError());
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}
	else
	{	sprintf(infobuf,"sem_recv: OK!\n");
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}

	hThread_Recv = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SC1801_Recv,
                                 NULL, CREATE_SUSPENDED, &ThreadID_Recv);
	if (hThread_Recv == NULL)
	{	sprintf(infobuf,"Error in CreateThread ThreadID_Recv:%d\n",GetLastError());
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}
	else
	{	sprintf(infobuf,"hThread_Recv: %d OK!\n",hThread_Recv);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}

	SetThreadPriority(hThread_Recv,THREAD_PRIORITY_BELOW_NORMAL);	 

 	hThread_Send = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SC1801_Send,
                                 NULL, CREATE_SUSPENDED, &ThreadID_Send);
	if (hThread_Send == NULL)
	{	sprintf(infobuf,"Error in CreateThread ThreadID_Send:%d\n",GetLastError());
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}
	else
	{	sprintf(infobuf,"hThread_Send: %d OK!\n",hThread_Send);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}

	SetThreadPriority(hThread_Send,THREAD_PRIORITY_BELOW_NORMAL);

	ResumeThread( hThread_Recv);	
	ResumeThread( hThread_Send);	
#endif
	// SC 1801 Link layer communication service
	for(;;)
    {
		initflag = 1;
		resetlinkf = 0;
		ClockSyncflag = 0;
		repeatnum = 0;
		timeoutnum = 0;

/*		if(PurgeComm(hPort_com,PURGE_TXABORT|PURGE_RXABORT|
			PURGE_RXCLEAR|PURGE_TXCLEAR)==FALSE)
			printf("INIT: PurgeComm ERRNO= %d \n",GetLastError());*/

		while(initflag)
		{
			if(resetlinkf != 1)
			{
				if(!RequestRTUConfig())
				{
					sleep(1);
					continue;
				}
			}
			else
			{
				if(!ClockSynchronisation())
				{
					sleep(1);
					continue;
				}
			}

#ifdef SUN_OS
		tim=time(&tim);
	
		memset(recvbuf, 0, VARFRAMELEN);
		nOffset = 0;

		FD_ZERO(&fset);
		FD_SET(hPort_com,&fset);
		tout.tv_sec = 2;
		tout.tv_usec = 0;

		ret = select(hPort_com+1, &fset, NULL, NULL, &tout);
		printf("====ret=%d=====",ret);

		if(ret <= 0)
#else
			fSuccess = ReadFile(hPort_com,(LPSTR)recvbuf,RTU_CONFIG_LEN*10,&byte_recvd,NULL);
			if(!fSuccess)
			{
				ClearRecvFault();
				continue;
			}
			if(byte_recvd == 0)
#endif
			{
				timeoutnum++;
				if(timeoutnum >= 10)
				{
					pollflag = 0;
					initflag = 1;
					resetlinkf = 0;
					ClockSyncflag = 0;
					timeoutnum = 0;
					sprintf(infobuf,"timeout >= 10, restart RequestRTUConfig\n");
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					continue;
				}

				if(resetlinkf != 1)
				{	sprintf(infobuf,"RequestRTUConfig: timeout!\n");
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
				}
				else
				{	sprintf(infobuf,"ClockSynchronisation: timeout!\n");
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
				}
				continue;
			}


			sprintf(infobuf,"SC1801 Host received data byte number %u :\n",byte_recvd);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);

			PrintRecvData(byte_recvd);

			datacount = recvbuf[2] << 8;
			datacount = datacount + recvbuf[3];

			if((recvbuf[0]!=STATION_ID)||((unsigned short)byte_recvd!= (datacount+5)))
			{
				sprintf(infobuf,"\nSC1801 Host received Frame ID %d or Count %d Error!\n",recvbuf[0],datacount);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				/*ClearRecvFault();*/
				continue;
			}

			chksum = 0xff;
			for(i=0;i<byte_recvd-1;i++)
				chksum = chksum ^ recvbuf[i];	

			if(chksum != recvbuf[byte_recvd-1])
			{
				sprintf(infobuf,"\nSC1801 Host received Frame XOR Check Error!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
			/*	ClearRecvFault();*/
				continue;
			}

			comd_id = recvbuf[1] & 0x3F;
			if(comd_id != send_comd_id)
			{
				sprintf(infobuf,"\nSC1801 Host received comd id %d != send comd id %d\n",
					comd_id,send_comd_id);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				/*ClearRecvFault();*/
				continue;
			}

			ctrlfield= recvbuf[1] & 0xc0;
			//ctrlfunc = recvbuf[1] & 0x0f;

			switch(comd_id)
			{
			case NAK:
				sprintf(infobuf,"NOT ACKNOWLEDGED frame Command= %02x Err code= %02x\n",
					recvbuf[4],recvbuf[5]);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				break;

			case RRC:
				if((resetlinkf == 1)||(byte_recvd != RTU_CONFIG_LEN))
				{
					sprintf(infobuf,"Report RTU Configuration frame wrong!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
					/*ClearRecvFault();*/
					continue;
				}

				sprintf(infobuf,"Report RTU Configuration frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				resetlinkf = 1;
				timeoutnum =0;
				break;

			case SST:
				if((ClockSyncflag == 1)||(byte_recvd != SST_LEN))
				{
					sprintf(infobuf,"Synchronize System Time frame Wrong!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
					/*ClearRecvFault();*/
					continue;
				}

				sprintf(infobuf,"Synchronize System Time frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				ClockSyncflag = 1;
				initflag = 0;
				timeoutnum =0;
				break;

			default :
				sprintf(infobuf,"Error Command Code %02x\n",comd_id);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				break;
			}

			continue;
		}//while(initflag)

		//General interrogation

		pollflag = 1;
		//dataflag = 0;
		dataflow = 0;
		comdflag = 0;
		interrogationflag = 1;
		repeatnum = 0;
		timeoutnum = 0;
		cmd_ack = 0;	//Simulate the comd count

		sendbufheadpt = send_data.pnt;
		//sendbufheadpt = 0;
		//fcb ++ ;

		while(pollflag)
		{
			if(timeoutnum >= 10)
			{
				pollflag = 0;
				initflag = 1;
				//localinitfg = 2;
				//resetlinkf = 0;
				sprintf(infobuf,"timeout >= 10, restart RequestRTUConfig\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				break;
			}

			if((repeatnum !=0)&&(repeatnum <= 3))
			{
				if(!RepeatTransmit())
					continue;
			}
			else
			{
				repeatnum =0;

				/*
				if(sendbufheadpt != send_data.pnt)		
				{
					comdflag = 1;
					if(!CommandSend(sendbufheadpt))
						continue;
					sendbufheadpt ++;
					if(sendbufheadpt>= CMD_ENTRY_NUMBER)
						sendbufheadpt = 0;
				}
				else
				*/

				{
				comdflag = 0;
				SimuCommandSend(cmd_ack);
				cmd_ack ++;
				if(cmd_ack > 12)
					cmd_ack = 0;

				/*
				if(RequestFullDataflag)
				{
					if(!RequestFullData())
						continue;
				}
				else
				//{	
					//if(dataflag)
					{
						if(!RequestExceptData())
							continue;
					}
				//}
				*/
				}
			}
#ifdef SUN_OS
		memset(recvbuf, 0, VARFRAMELEN);
		nOffset = 0;

		FD_ZERO(&fset);
		FD_SET(hPort_com,&fset);
		tout.tv_sec = 2;
		tout.tv_usec = 0;

		ret = select(hPort_com+1, &fset, NULL, NULL, &tout);
		printf("====ret=%d=====",ret);

		if(ret <= 0)
#else
			fSuccess = ReadFile(hPort_com,(LPSTR)recvbuf,VARFRAMELEN,&byte_recvd,NULL);
			if(!fSuccess)
			{
				if(comdflag)
					printf("CommandSend: ReadFile ERRNO= %d \n",GetLastError());
				else
					printf("Polling Data: ReadFile ERRNO= %d \n",GetLastError());
				ClearRecvFault();
				continue;
			}

			if(byte_recvd == 0)
#endif
			{
				timeoutnum++;
				repeatnum++;
				if(comdflag)
					printf("CommandSend: timeout! No: %d",timeoutnum);
				else
					printf("Polling Data: timeout! No: %d",timeoutnum);
				continue;
			}


			sprintf(infobuf,"\nSC1801 Host received data byte number %u :\n",byte_recvd);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				PrintRecvData(byte_recvd);
			//memcpy(&datacount,&recvbuf[2],2);

			datacount = recvbuf[2] << 8;
			datacount = datacount + recvbuf[3];

			if((recvbuf[0]!=STATION_ID)||((unsigned short)byte_recvd!= datacount+5))
			{
				sprintf(infobuf,"\nSC1801 Host received Frame ID or Count Error!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				repeatnum++;
				continue;
			}

			chksum = 0xff;
			for(i=0;i<byte_recvd-1;i++)
				chksum = chksum ^ recvbuf[i];	

			if(chksum != recvbuf[byte_recvd-1])
			{
				sprintf(infobuf,"\nSC1801 Host received Frame XOR Check Error!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				repeatnum++;
				continue;
			}

			comd_id = recvbuf[1] & 0x3F;
			if((comd_id != send_comd_id)&&(comd_id != NAK))
			{
				sprintf(infobuf,"\nSC1801 Host received comd id %d != send comd id %d\n",
					comd_id,send_comd_id);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				/*ClearRecvFault();*/
				repeatnum++;
				continue;
			}

			switch (comd_id)
			{
			case NAK:
				sprintf(infobuf,"NOT ACKNOWLEDGED frame Command= %02x Err code= %02x\n",
					recvbuf[4],recvbuf[5]);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);

				repeatnum++;
				timeoutnum =0;
				break;

			case RRC:
				if(byte_recvd != RTU_CONFIG_LEN)
				{
					sprintf(infobuf,"Report RTU Configuration frame length wrong!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
					ClearRecvFault();
					repeatnum++;
					continue;
				}

				sprintf(infobuf,"Report RTU Configuration frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				resetlinkf = 1;
				break;

			case DRF:
				sprintf(infobuf,"DATA Request Full frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case XRF:
				sprintf(infobuf,"Exception Request Full frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case SOE:
				sprintf(infobuf,"Sequence of Events frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case PAR:
				sprintf(infobuf,"Pulse Accumulator Report frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case PAZ:
				sprintf(infobuf,"Pulse Accumulator ZERO frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case PAF:
				sprintf(infobuf,"Pulse Accumulator Freeze frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case PAT:
				sprintf(infobuf,"Pulse Accumulator Thaw frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case SDB:
				sprintf(infobuf,"Set Deadband frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case RDB:
				sprintf(infobuf,"Read Deadband frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case COA:
				sprintf(infobuf,"Control Output ARM frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case COD:
				sprintf(infobuf,"Control Output Direct frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case COE:
				sprintf(infobuf,"Control Output Execute frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case COL:
				sprintf(infobuf,"Control Output Latched frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case DRL:
				sprintf(infobuf,"DATA Request Latched frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case SST:
				if(byte_recvd != SST_LEN)
				{
					sprintf(infobuf,"Synchronize System Time frame length Wrong!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				/*	ClearRecvFault();*/
					repeatnum++;
					continue;
				}

				sprintf(infobuf,"Synchronize System Time frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				ClockSyncflag = 1;
				break;

			case RST:
				if(byte_recvd != SST_LEN)
				{
					sprintf(infobuf,"Report System Time frame length Wrong!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
					/*ClearRecvFault();*/
					repeatnum++;
					continue;
				}
#ifdef DEBUG
				printf("Report System Time frame Correctly!\n");

/*				tim = (time_t)(recvbuf[5]<<24);	//High tim 24-31
				tim = tim + (time_t)(recvbuf[6]<<16);	//High tim 16-23
				tim = tim + (time_t)(recvbuf[7]<<8);	//low tim 8-15
				tim = tim + (time_t)recvbuf[8];	//High low 0-7
				tim = tim + DRFT_SEC;	//1970.1.1 -- 2000.1.1 GMT

				sst_ms = recvbuf[9]<<8;
				sst_ms = sst_ms + recvbuf[10];

				timeptr=localtime(&tim);
				GetLocalTime(&sys_tm);

				sys_tm.wYear		= timeptr->tm_year + 1900;
				sys_tm.wMonth		= timeptr->tm_mon+1;	  
				sys_tm.wDay			= timeptr->tm_mday;	
				sys_tm.wHour		= timeptr->tm_hour;
				sys_tm.wMinute		= timeptr->tm_min;
				sys_tm.wSecond		= timeptr->tm_sec; 
				sys_tm.wDayOfWeek	= timeptr->tm_wday;
				sys_tm.wMilliseconds= (unsigned short)(sst_ms/10);

				printf("Report RTU System Time Received!\n");
				printf("%4d/%2d/%2d %2d:%2d:%2d %3dms DayOfWeek %2d\n",sys_tm.wYear,
					sys_tm.wMonth,sys_tm.wDay,sys_tm.wHour,sys_tm.wMinute,
					sys_tm.wSecond,sys_tm.wMilliseconds,sys_tm.wDayOfWeek);
					*/
#endif
				timeoutnum =0;
				repeatnum = 0;
				break;

			case RIM:
				sprintf(infobuf,"Report Interface Mode frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case SIM:
				sprintf(infobuf,"Set Interface Mode frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case VER:
				sprintf(infobuf,"Read RTU And RMF Version Identifiers frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case RES:
				sprintf(infobuf,"Perform RTU Cold Start frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			case ERR:
				sprintf(infobuf,"Report Error DATA frame Correctly!\n");
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				timeoutnum =0;
				repeatnum = 0;
				break;

			default :
				sprintf(infobuf,"SC1801 Host received command code %2x is unavailable!\n",comd_id);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);

				/*ClearRecvFault();*/
				break;			
			}	//switch(recvbuf[0])
		}		//while(pollflag)
	}			//for(;;)

	UnMountDB();
}


void WriteDspInfo(char *buf)
{
/*	int	bufptr,len,charlen;

	if ((long *)shm_buf_addr==(long *)-1)
		return;
	bufptr = shm_buf_addr->header_ptr;
	len = strlen(buf);
	charlen = DSP_INFO_SIZE - bufptr;
	if(charlen < len)
	{
		memcpy(&shm_buf_addr->buf[bufptr],buf,charlen);
		bufptr = len - charlen;
		memcpy(&shm_buf_addr->buf[0],&buf[charlen],bufptr);
		shm_buf_addr->header_ptr = bufptr;
	}
	else
	{
		memcpy(&shm_buf_addr->buf[bufptr],buf,len);
		charlen  = bufptr + len;
		if(charlen >= DSP_INFO_SIZE)
			shm_buf_addr->header_ptr = 0;
		else
			shm_buf_addr->header_ptr = charlen;
	}*/
	Err_log(buf);
}