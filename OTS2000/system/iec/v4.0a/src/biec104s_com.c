/***************************************************************
*  FILE NAME:           iec104s_com.c      			*
*  FUNCTION:            Controlled Station Main Process		*
*			Link Layer Communication Program	*
*  NAME :  			              			*
*  DATE:                Apr,2006                		*
*								*
****************************************************************/
#include <thread.h>
#include "../inc/iec104s_com.h"

#define	DEBUG

int		ProcNum;
PROC_REC	sProcRec[MAX_PROC_NUM+1];

extern	ASDUCOMDDATA	recv_data;
extern	ASDUDATA	send_data,send_data2;
extern	SPDEF		spdef;
extern	DPDEF		dpdef;
extern	MEDEF		medef;
extern	CYCDEF		cycdef;
extern	DCDEF		dcdef;
extern	SEDEF		sedef;
extern	ITDEF		itdef;
extern	SCDEF		scdef;
extern	RCDEF		rcdef;

extern	int		NoChangeData;

/*HANDLE	hRecvSemaphore;
*/
unsigned char recvbuf[1024],cmdsendbuf[24];
unsigned char sendbuf[1024];
unsigned short nr_recv,nr_send,nr_ack,nr_rs;
SOEBUF	soebuf;
CMDBUF	cmdbuf;
SEBUF	sebuf;
PMBUF	pmbuf;
ICBUF	icbuf;
CIBUF	cibuf;
RDBUF	rdbuf;
RPBUF	rpbuf;
TESTBUF	testbuf;
CDBUF	cdbuf;
MIRBUF	mirbuf;

/*COMSTAT	cstat; */
int	sigpipe;
int	timeout;
int	ClockSyncflag,comm_flag,comdflag,cmdterm;
int	Readflag,Testflag,Resetflag;
int	Paramflag,MirrorNAckflag;
int	ClkDiffflag,soe_overflow_flag,soe_overflow_Sendflag;
int	interrogationflag,counterflag,icgroupflag,cigroupflag;
int	delayflag;
int	cyclicflag,scanflag;
unsigned short	delayalarm;
unsigned char	cot_interrogation,cot_counter;
unsigned short	m_cRespGenInterroGrpNum,m_cRespCountGrpNum,m_cCyclicGrpNum,m_cScanGrpNum;
unsigned int	repeatnum,timeoutnum;
unsigned short	seladdr;
int		cmd_ack;
short	databand;

unsigned short	nASDUId;
unsigned char	nSrcId;
int		Gps;		/* 0: Without GPS; 1: With GPS */
int		GiType;		/* General interrogation type> 1: M_SP_NA; 20: M_PS_NA */
int		MeASDUType;	/* Me ASDU type> 0: M_ME_NA; 1: M_ME_NB; 3: M_ME_NC */
int		MeType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
int		SEType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
int		TimeTag;	/* Time tag> 0: no; 1: short; 2: long */
int		sp_cos;		/* sp_cos> 0: sp/dp transmit once; 1: twice */
int		Cmd_Sel;	/* 0: cmd without select; 1: cmd with select */

time_t	scan_tim;
time_t	scan_period;
time_t	cmd_period;
/*SYSTEMTIME	localsys_tm; */

#ifdef REC_YF_AGC_CMD
extern	unsigned short	daysaved;
#endif

void Endproc(int sig_no)
{
	int i;

	printf("  ---- iec104s_com: exit signal %d received!  ----\n",sig_no);
	if( getpid()==sProcRec[ProcNum].Pid )
	{
		for(i=0;i<ProcNum;i++)
		{
			printf("  ---- %s iec104s_com: stop! ( pid=%05d ) ----\n",sProcRec[i].NameID,sProcRec[i].Pid);
			kill( sProcRec[i].Pid,SIGKILL );
		}
		UnMountDB();
		Lan_finish(0);
/*		kill(getpid(),SIGKILL); */
	}
	exit(0);
} 
                                                                
void Timer(int sig_no)
{
	signal(SIGALRM,Timer);
	timeout=YES;
}

void sig_pipe(int sig_no)
{
	printf("  ---- iec104s_com: socket broken!!!\n");
	sigpipe=YES;
}

void Waitchild(int nouse)
{
	int status;

/*	printf("  ---- iec104s_com: in Waitchild() !!!\n"); */
	alarm(1);
	signal(SIGALRM,Timer);
	wait(&status);
	alarm(0);
	signal(SIGCHLD,Waitchild); 
}

void main(int argc,char *argv[])
{
	int		hThread_SOE;
	thread_t	ThreadID_SOE;
/*
	WORD   		wVer;
	WSADATA 	WSAData;
	int 		verst;
*/
	unsigned short iec104port;
	int		i;

	struct sockaddr_in	clientaddr;
	int		clientaddrlen;
	int 		iec_sock,iec_newsock;
	char		rmt_ip_str[16];
	char		tmp[100]; 

	for(;;)
	{
		if( ReadStnDef(STN_DEF_FILE) == true )
			break;
            	printf("\n###ALARM!!! IEC104::Read Station define file %s Error!\n",STN_DEF_FILE);
		sleep(5);
		continue;
	}

	i = -1;
	if(argc == 2)
	{
		i = atoi(argv[1]);
#ifdef DEBUG
		printf("iec104s_com:Command Line parameter %s %d\n",argv[1],i);
#endif
	}
	if((i<0)||(i>=ProcNum))
	{
		printf("iec104s_com:Command Line parameter %s %d Wrong!\n",argv[1],i);
		exit(0);
	}

	if(-1==MountDB(DATABASE_NAME))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}
/*
	Lan_init ();
*/
	get_lan_shm(1);

	signal(SIGKILL,Endproc);
	signal(SIGTERM,Endproc);
	signal(SIGINT,Endproc);
	signal(SIGCHLD,Waitchild);
/*	
	strcpy( sProcRec[ProcNum].NameID, "WATCH_DOG" );
	sProcRec[ProcNum].Pid = getpid(); 
*/
	sprintf(tmp,"H9000通信系统: %s iec104协议(iec104s_com) 本地IP：%s",sProcRec[i].NameID,sProcRec[i].Ip);

	comm_flag = 0;
	nr_recv = nr_send = nr_ack = 0;
	InitData(sProcRec[i].NameID);
/*
	hThread_SOE = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)IEC104_SOE,
                                 NULL, CREATE_SUSPENDED, &ThreadID_SOE);
	if (hThread_SOE == NULL)
		printf("iec104s_com:Error in CreateThread ThreadID_SOE:%d\n",GetLastError());
	else
		printf("iec104s_com:hThread_SOE: %d OK!\n",hThread_SOE);

	SetThreadPriority(hThread_SOE,THREAD_PRIORITY_BELOW_NORMAL);	 

	ResumeThread( hThread_SOE);	
*/
	iec_sock = socket(AF_INET, SOCK_STREAM,0);
                
	if( iec_sock<0 )
	{ 
		printf("iec104s_com:: creat socket() failed!\n");
		exit(-1);
	}
	iec104port = (unsigned short)IEC104_PORT;
	bzero (( char*)&clientaddr, sizeof ( clientaddr ));
/*	memset((char *)&clientaddr,0,sizeof(struct sockaddr_in)); */
	clientaddr.sin_family      = AF_INET;
/*
	clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientaddr.sin_addr.s_addr = inet_addr("192.168.222.1");
*/
	clientaddr.sin_addr.s_addr = inet_addr(sProcRec[i].Ip);
	clientaddr.sin_port        = htons(iec104port);  


	if(bind(iec_sock,(struct sockaddr *)&clientaddr,sizeof(clientaddr))<0)
	{
#ifdef DEBUG
		printf("iec104s_com:: bind() error: %d!\n ", errno);
#endif
		close(iec_sock);
		sleep(5);
		exit(-1);
	}

	if(listen(iec_sock,4)<0)
	{
#ifdef DEBUG
		printf("iec104s_com::listen() error: %d!\n ", errno);
#endif
		close(iec_sock);
		exit(-1);
	}

	clientaddrlen=sizeof(clientaddr);

	for(;;)
	{
#ifdef DEBUG
		printf("\niec104s_com:: waiting for connection\n");
#endif   
		UpdateChannelFlag(i,(unsigned short)4);
		iec_newsock=accept(iec_sock,(struct sockaddr *) &clientaddr,(socklen_t *)&clientaddrlen);
		
		if(iec_newsock<0)
		{
#ifdef DEBUG
			printf("\niec104s_com::accept() return %d wrong!\n", errno);
#endif
			continue;

		} 	
		strcpy(rmt_ip_str,inet_ntoa(clientaddr.sin_addr));

#ifdef DEBUG
		printf("\niec104s_com:: new connection! ip=%s socket=%d\n",rmt_ip_str,iec_newsock);
#endif   
		sprintf(tmp,"H9000通信系统: %s iec104协议(iec104s_com) 本地IP：%s 远方IP：%s",sProcRec[i].NameID,sProcRec[i].Ip,rmt_ip_str);
		Net_Com(i,iec_newsock);
	}
}

int Net_Com(int pnum,int sockfd)
{
	int		sock;
	int		byte_recvd,pkt_len,count;
	unsigned int	sendbufheadpt,recvbufheadpt;
	unsigned int	sendbufptr1,sendbufptr2;
	unsigned int	localinitflag;

	int		unack_num;
	int		timeout;
	int		err;
	int		wait_ackflag;
	int	ret;

	time_t	tim,tim_waitack;
	struct tm *timeptr;

	sendbufheadpt = 0;
	sendbufptr1 = 0;
	sendbufptr2 = 0;
	localinitflag = 1;
	comm_flag = 0;
	wait_ackflag = 0;

	sock = sockfd;
#ifdef REC_YF_AGC_CMD
	tim=time(&tim);
	timeptr=localtime(&tim);
	daysaved = timeptr->tm_mday;
#endif

#ifdef DEBUG
	printf("iec104s_com: Thread Net_Com() start!\n");
#endif
	timeout=1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_KEEPALIVE,&timeout,sizeof (timeout))!=0)
	{
		printf("iec104s_com: setsockopt SO_KEEPALIVE Fail ...\n",errno);
	}

	nr_recv = nr_send = nr_ack = nr_rs = 0;
	soebuf.sendpnt = soebuf.bufpnt;

	for(;;)
	{
	/* receive data */

		memset(recvbuf, 0, VARFRAMELEN);
		timeout=NO;
		alarm(2);
		byte_recvd=recv(sockfd,(char *)recvbuf,(int)MINFRAMELEN,(int)0);
		alarm(0);

		tim=time(&tim);
		if( (byte_recvd!=MINFRAMELEN) || timeout==YES )
		{
			if((byte_recvd <= 0)&&( timeout!=YES ))
			{
				printf("iec104s_com: recv() Error %d! do closesocket!\n",errno);
				close(sockfd);
				return(false);
			}
#ifdef DEBUG
			printf("iec104s_com: recv() TIMEOUT! byte_recvd=%d\n",sProcRec[pnum].NameID,byte_recvd);
#endif
			timeoutnum++;
			if(timeoutnum%30==0)
			{
				printf("ReceiveData: timeout! No: %d",timeoutnum);
				UpdateChannelFlag(pnum,(unsigned short)1);
			}
			if(comm_flag)
			{
				if(localinitflag)
				{
					localinitflag = 0;
					ret = EndofInitialization(sock);
					if(ret == false)
						return(false);
					continue;
				}
				if((wait_ackflag)&&((tim - tim_waitack)>t1))
				{
					printf("iec104s_com: MASTER timeout! do closesocket!\n");
					close(sockfd);
					return(false);
				}
				if(nr_send >= nr_ack)
					unack_num = nr_send - nr_ack;
				else
					unack_num = 0x10000 + nr_send - nr_ack;
				if(unack_num >= K*2)
				{
					printf("nr_send=%d, nr_ack=%d, unack_num=%d\n",nr_send, nr_ack, unack_num);
					continue;
				}

				ret = RespondUserDataClass1(sock);
				if(ret == false)
					return(false);
				if((NoChangeData == 0)&&(wait_ackflag == 0))
				{
					wait_ackflag = 1;
					tim_waitack = tim;
				}
			}
			continue;
		}
		else if(byte_recvd == 0)
		{
			printf("iec104s_com: client shutdown socket! do closesocket!\n");
			close(sock);
			return(false);
		}
		else
		{
			timeoutnum =0;

/*
			if((byte_recvd!=MINFRAMELEN)||(byte_recvd != (int)(recvbuf[1]+2)))
			if(byte_recvd!=MINFRAMELEN)
			{
				printf("iec104s_com:Byte number received=%d diff than frame length %d\n",byte_recvd,recvbuf[1]);
				continue;
			}
*/
			UpdateChannelFlag(pnum,(unsigned char)0);
			if((recvbuf[0]==0x68) && (recvbuf[1]==4) && ((recvbuf[2] & 0x3) == 3))	/* U Frame */
			{
				if(recvbuf[2] == 0x07)	/* START DT */
				{
#ifdef DEBUG
					printf("\niec104s_com: received U Frame START DT data byte number %u:",byte_recvd);
					PrintRecvData(byte_recvd);
#endif
					comm_flag = 1;
					nr_recv = nr_send = nr_ack = nr_rs = 0;	/* added 2005.6.17 zy */
					if(!RespondStartDT(sock))
						return(false);
				}
				else if(recvbuf[2] == 0x13)	/* STOP DT */
				{
#ifdef DEBUG
					printf("\niec104s_com: received U Frame STOP DT data byte number %u:",byte_recvd);
					PrintRecvData(byte_recvd);
#endif
					comm_flag = 0;
					if(!RespondStopDT(sock))
						return(false);
				}
				else if(recvbuf[2] == 0x43)	/* TEST FR */
				{
/* 					comm_flag = 0;	 */
#ifdef DEBUG
					printf("\niec104s_com: received U Frame TEST FR data byte number %u:",byte_recvd);
					PrintRecvData(byte_recvd);
#endif
					if(!RespondTestFR(sock))
						return(false);
				}
			}
			else if((recvbuf[0]==0x68) && (recvbuf[1]==4) && ((recvbuf[2] & 0x3) == 1))	/* S Frame */
			{
#ifdef DEBUG
				printf("\niec104s_com: received S Frame data byte number %u:",byte_recvd);
				PrintRecvData(byte_recvd);
#endif
				memcpy(&nr_ack,&recvbuf[4],2);
				wait_ackflag = 0;
			}
			else if((recvbuf[0]==0x68) && ((recvbuf[2] & 0x1) == 0))	/* I Frame */
			{
				pkt_len = recvbuf[1]+2;
				if(pkt_len <=6)
				{
					printf("iec104s_com: Received length %d < 6 wrong;  shutdown socket!\n",pkt_len);
					close(sockfd);
					return(false);
				}
				count = 6;
				while (count < pkt_len)
				{					/* keep reading until "pkt_len" bytes are read	*/
					timeout=NO;
					alarm(2);
					byte_recvd = recv (sockfd, (char *)recvbuf+count, pkt_len-count, 0);
					alarm(0);
					if (byte_recvd <= 0)		/* 0 if link is disconnected, negative if error.*/
					{
						close(sockfd);
						return(false);
					}
					else
						count += byte_recvd;	/* add the bytes read to "count" */
				}	/* end "while" loop */

#ifdef DEBUG
				byte_recvd = pkt_len;
				printf("\niec104s_com: received I Frame data byte number %u:",byte_recvd);
				PrintRecvData(byte_recvd);
#endif
				memcpy(&nr_rs,&recvbuf[2],2);
				memcpy(&nr_ack,&recvbuf[4],2);
				if(nr_rs != nr_recv)
				{
					printf("iec104s_com: Received Nr %d not equiv V(R) %d; shutdown socket!\n",nr_rs,nr_recv);
/*
					close(sockfd);
					return(false);
*/
					nr_recv = nr_rs;	/* for test only */
				}
				nr_recv +=2;
				wait_ackflag = 0;
/* clock synchronization 103 or Delay Acquire command 106 */
/*
				if((recvbuf[6] == C_CS_NA)||(recvbuf[6] == C_CD_NA))
					GetLocalTime(&localsys_tm);
*/
				recvbufheadpt = recv_data.pnt;
				memset(&recv_data.data[recvbufheadpt],0,CMD_DATA_SIZE);
				memcpy(&recv_data.data[recvbufheadpt],&recvbuf[6],pkt_len - 6);
				recv_data.length[recvbufheadpt] =(unsigned short) (pkt_len - 6);
#ifdef DEBUG
				printf("iec104s_com: recv point = %d\n",recvbufheadpt);
#endif
				ret = IEC870_5_Recv(pnum,recvbufheadpt,sock);
				if(ret == false)
					return(false);

				recvbufheadpt ++;
				if(recvbufheadpt >= CMD_ENTRY_NUMBER)
					recvbufheadpt = 0;
				recv_data.pnt = recvbufheadpt;
/* 				ReleaseSemaphore(hRecvSemaphore,1,NULL); */
			}
			else
			{
#ifdef DEBUG
				printf("iec104s_com: Wrong %02X type id %d frame!\n",recvbuf[2],recvbuf[6]);
#endif
			}
		}
	}/* for(;;) */
	return(false);
}

int UpdateChannelFlag(int pnum, unsigned short status)
{
	char	name_buf[36];
	char	host_name[36];
	int	data_type;

	DMS_COMMON	dbentry;
	POINTER		pnt;
	SYS_MSG		lanbcdata;
	LAN_HEAD	lanhd;

	WR_VAR_UNION	new_value;
	enum RW_FLAG	rw_flag=0;
	IOST		iost={0};
	time_t		tim;
	struct tm	tmpt;
	DATE_TIME	update_time;

/*	strcpy(name_buf,SYS_TAB_NAME);
*/
	gethostname(host_name, 10);
	strcat(name_buf,host_name);
	strcat(name_buf,"_");
	strcat(name_buf,sProcRec[pnum].NameID);
/*
	ReadEntryByName(name_buf,&data_type,&dbentry);
*/
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		printf("%s No this com channel status point in the database file!\n",name_buf);
		return(false);
	}
	else if(dbentry.var.ind.status.state != status)
	{
		lanbcdata.type_id =DPS_IND_ALM;
		lanbcdata.stn_id =dbentry.point.stn_id;
		lanbcdata.dev_id =dbentry.point.dev_id;
		lanbcdata.data_type=dbentry.point.data_type;
		lanbcdata.number = 1;
		lanbcdata.point_id = dbentry.point.pt_id;

		lanbcdata.status=status;	/*communication channel status: 0:normal; 1:Fail */
		lanbcdata.ms=0;
		lanbcdata.host_id=GetHost_Cmd();

		tim=time (NULL);
		tmpt=*localtime (&tim);
		lanbcdata.sec=tmpt.tm_sec;
		lanbcdata.min=tmpt.tm_min;
		lanbcdata.hour=tmpt.tm_hour;
		lanbcdata.day=tmpt.tm_mday;
		lanbcdata.month=tmpt.tm_mon+1;
		lanbcdata.year=tmpt.tm_year+1900;

#ifdef SUN
		Swap2Byte(&lanbcdata.number);
		Swap2Byte(&lanbcdata.point_id);
		Swap2Byte(&lanbcdata.status);
		Swap2Byte(&lanbcdata.ms);
		Swap2Byte(&lanbcdata.year);
		Swap2Byte(&lanhd.length);
#endif
		lanhd.length=sizeof(SYS_MSG);
		lanhd.dp_type=BJ_DB_TIME;
/*
		lanhd.src_id =GetHost_Cmd();
*/
		lanhd.dest_id =0;
		lanhd.src_id =lanbcdata.host_id;
		lanhd.dest_stn =lanbcdata.stn_id;
		lanhd.packet_num =0;
		lan_out ((char *)&lanbcdata,lanhd,(unsigned char)0);

		pnt = dbentry.point;
		new_value.ind.state = status;
		update_time.ms		= 0;
		update_time.second	= tmpt.tm_sec;
		update_time.minute	= tmpt.tm_min;
		update_time.hour	= tmpt.tm_hour;
		update_time.day		= tmpt.tm_mday;
		update_time.month	= tmpt.tm_mon+1;
		update_time.year	= tmpt.tm_year+1900;
		rw_flag = RW_UPDATE_TIME | RW_RT_VALUE;

		if(SetPtValueIostById(&pnt, new_value, iost, update_time, rw_flag) == -1)
			printf ( "\nIEC104: %d-%d-%d-%d No this point in the database when Wdb!\n",
				pnt.stn_id, pnt.dev_id, pnt.data_type, pnt.pt_id);

/*
		common_bb.point_id=name_buf;

		common_bb.entry.ind.variable.value.status = status << BitS_STATUS;
		stat = WriteEntryByName(&common_bb, RW_VAR);
		dbentry.point_id=name_buf;
		dbentry.entry.ind.variable.value.status = status;

		if (WriteEntryByID(&dbentry, RW_VAR) == -1)
			printf ( "\nIEC104: %s No this point in the database when Wdb!\n",name_buf);
*/
	}
	return(true);
}

/*
PROCESS_INFORMATION		pi;
*/
/*
void OpenNewTermalWindow ( void )
{
	STARTUPINFO				sui;

	int i;
	char	cmdline[64];

	CreateMutex( NULL, false, "EMS_RUNNING_NOW" );
	if ( GetLastError () != ERROR_ALREADY_EXISTS ) /* return;
	{
		printf("H9000 EMS 系统未启动，iec104 通信进程退出！\n");
		exit(-1);
	}

	CreateMutex( NULL, false, "IEC104S_CHILD" );
	if ( GetLastError () == ERROR_ALREADY_EXISTS )
	{	
		return;
	}

	SetConsoleTitle ( "H9000通信系统: IEC870-5-104 通信主进程(iec104s_com)" );
	if ( SetConsoleCtrlHandler ((void *)ExitRoutine,true ) == 0 )
	{	
		printf("\n !!! iec104s_com: SetConsoleCtrlHandler ERROR %d!!! \n",GetLastError());
	}

	for(i=0;i<ProcNum;i++)
	{
		sui.cb		= sizeof (STARTUPINFO);
		sui.lpReserved	= NULL;
		sui.cbReserved2	= 0;
		sui.lpReserved2	= NULL;
		sui.lpDesktop	= NULL;
		sui.dwFlags          = 0;
		sui.lpDesktop        = NULL;
		sui.lpTitle          = NULL;
		sui.dwX              = 0;
		sui.dwY              = 0;
		sui.dwXSize          = 0;
		sui.dwYSize          = 0;
		sui.dwXCountChars    = 0;
		sui.dwYCountChars    = 0;
		sui.dwFillAttribute  = 0;
		sui.wShowWindow      = 0;

		sprintf(cmdline,"%s %d",EXE_FILE,i);
		if( CreateProcess ( EXE_FILE,cmdline, NULL, NULL, false,
					 CREATE_NEW_CONSOLE, NULL, NULL, &sui, &sProcRec[i].pi)==false )
		{	
			printf("\n !!!CREATE CHILD WINDOW %s %d ERROR %d!!! \n",EXE_FILE,i,GetLastError());
		}
	}
	for ( ;; )
	{ 
		for(i=0;i<ProcNum;i++)
		{
			if ( WaitForSingleObject ( sProcRec[i].pi.hProcess, 0 ) == 0 )	
			{
				CloseHandle(sProcRec[i].pi.hThread);
				CloseHandle(sProcRec[i].pi.hProcess);
				printf("\n !!!Reboot %s %d CHILD WINDOW Now!!! \n",EXE_FILE,i);

				sui.cb       		 = sizeof (STARTUPINFO);
				sui.lpReserved       = NULL;
				sui.cbReserved2      = 0;
				sui.lpReserved2      = NULL;
				sui.lpDesktop 		 = NULL;
				sui.dwFlags          = 0;
				sui.lpDesktop        = NULL;
				sui.lpTitle          = NULL;
				sui.dwX              = 0;
				sui.dwY              = 0;
				sui.dwXSize          = 0;
				sui.dwYSize          = 0;
				sui.dwXCountChars    = 0;
				sui.dwYCountChars    = 0;
				sui.dwFillAttribute  = 0;
				sui.wShowWindow      = 0;

				sprintf(cmdline,"%s %d",EXE_FILE,i);
				if( CreateProcess ( EXE_FILE,cmdline, NULL, NULL, false,
									 CREATE_NEW_CONSOLE, NULL, NULL, &sui, &sProcRec[i].pi)==false )
				{	
					printf("\n !!!Reboot CHILD WINDOW %s %d ERROR %d!!!\n",EXE_FILE,i,GetLastError());
				}
			}	
			sleep ( 10);			
		}
	}
}

void ExitRoutine ( DWORD signal_type )
{
	int		i;
	UINT	uExitCode= 0;

	switch ( signal_type )
	{
	case CTRL_C_EVENT:
		printf ( "\n!!! CTRL_C_EVENT received !!! \n" ); 
		break;

	case CTRL_BREAK_EVENT:
		printf ( "\n!!! CTRL_BREAK_EVENT received !!! \n" ); 
		break;
	
	case CTRL_CLOSE_EVENT:
		printf ( "\n!!! CTRL_CLOSE_EVENT received !!! \n" ); 
		break;
	
	case CTRL_LOGOFF_EVENT:
		printf ( "\n!!! CTRL_LOGOFF_EVENT received !!! \n" ); 
		break;
	
	case CTRL_SHUTDOWN_EVENT:
		printf ( "\n!!! CTRL_SHUTDOWN_EVENT received !!! \n" ); 
		break;
	
	default:
		break;
	}

	for(i=0;i<ProcNum;i++)
	{
		if ( TerminateProcess ( sProcRec[i].pi.hProcess, uExitCode ) == 0 )
		{
			printf ( "ERROR in TerminateProcess %d ( num=%d ), Err=%d %d\n", sProcRec[i].pi.hProcess,i,uExitCode,GetLastError());
		}

		if ( TerminateProcess ( sProcRec[i].pi.hThread, uExitCode ) == 0 )
		{
			printf ( "ERROR in TerminateProcess (hThread ) %d ( num=%d ), Err=%d %d\n",sProcRec[i].pi.hThread,i,uExitCode,GetLastError());
		}
		printf("\n !!!iec104s_com %d Terminate Now!!! \n",i);
	}
	printf("\n !!!iec104s_com 通信主进程 Terminate Now!!! \n");
	exit (0 );
}
*/



  

