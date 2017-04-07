
#include "/home/ems/system/iec/v4.0a/inc/iec104_com.h"
#define	TMPFILE1	"/home/ems/h9000/iec104/tmpfile"

void		Endproc(),	Waitchild();
int		ProcNum;
PROC_REC	sProcRec[MAX_PROC_NUM+1];
unsigned char ProcStnId[MAX_PROC_NUM+1];
unsigned char cur_stn_id;		//added by yyp 2011.03.09


#ifndef SUN_OS
	int			stn_no;
	HANDLE	hMutex;
#endif

extern	ASDUCOMDDATA	send_data;
extern	ASDUDATA	recv_data;
extern  CMDBUF          cmdbuf;


SHM_RECORD *shm_rec_addr;
int shm_id_rec;
int shm_id_buf;
SHM_BUF *shm_buf_addr;

unsigned char recvbuf[1024];
unsigned char sendbuf[1024];
char	infobuf[1024];
unsigned char cmdsavbuf[36];
short	cmd_end;

int	timeout;
int	pollflag;
int	cmd_ack;
int	sigpipe;
int	selectcmd;
int	link_num;
unsigned char	comdflag,localinitfg;
unsigned short	nr_recv,nr_send,nr_ack,ns_ack,nr_rs;

unsigned short	nASDUId;
unsigned char	nSrcId;

int	Gps;		/* 0: Without GPS; 1: With GPS */
int	GiType;		/* General interrogation type> 1: M_SP_NA; 20: M_PS_NA */
int	MeASDUType;	/* Me ASDU type> 0: M_ME_NA; 1: M_ME_ND; 3: M_ME_NC */
int	MeType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
int	SEType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
/*int	TimeTag;		Time tag> 0: no; 1: short; 2: long */
int	sp_cos;		/* sp_cos> 0: sp/dp transmit once; 1: twice */
int	Cmd_Type;	/* 0: single point ctrl C_SC_NA; 1: double point ctrl C_DC_NA */
int	Cmd_Sel;	/* 0: cmd without select; 1: cmd with select */

time_t	scan_period;
time_t	cmd_period;
time_t	test_period_t3;

short	rcv_end;
extern void*	CreateShareMemory(char key_char, int size, char *shm_name);
char logfilename[128];
FILE *fp;
struct tm  *ntime;
int oldday;

char hostname[20];
int ReadDB_IND(char *PointName);

int test_flag=0;
int comm_num;
COMM_CONFIG comm_config[MAX_COMM_NUM];

#ifdef SUN_OS
void Endproc(int sig_no)
{
	int i;

	printf("  ---- Master exit signal %d received!  ----\n",sig_no);
	if( getpid()==sProcRec[ProcNum].Pid )
	{
		for(i=0;i<ProcNum;i++)
		{
			printf("  ---- %s IEC104 Master stop! ( pid=%05d ) ----\n",sProcRec[i].NameID,sProcRec[i].Pid);
			kill( sProcRec[i].Pid,SIGTERM );
		}
		UnMountDB();
		lan_finish(0);
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
	printf("  ---- Master socket broken!!!\n");
	sigpipe=YES;
}
#endif



void main(int argc,char *argv[])
{ 
	int	pid, wpid,i;
	FILE	*fp;
	char	tmp[100]; 
	int	Socket,	Flag=1;
	USHORT	host_state;
	int	stat_loc;
#ifndef SUN_OS
	WORD   		wVer;
	WSADATA 	WSAData;
	int 		verst;
	HANDLE	hMutex;
#endif
#ifdef WINDOWS_OS
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return;
	}
#endif
	gethostname(hostname,20);
	if(argc>1 && !strcmp(argv[1], "-test"))
		test_flag=1;
	else
		test_flag=0;
	ReadCOMMConfig();  
	for(;;)
	{
		if (!test_flag)
		{
			if( ReadStnDef(STN_DEF_FILE) == true )
				break;
			printf("\n###ALARM!!! IEC104::Read Station define file %s Error!\n",STN_DEF_FILE);
		}
		else
		{
			if( ReadStnDef(TEST_STN_DEF_FILE) == true )
				break;
			printf("\n###ALARM!!! IEC104::Read Station define file %s Error!\n",TEST_STN_DEF_FILE);
		}
		sleep(5);
		continue;
	}

#ifndef SUN_OS
	OpenNewTermalWindow();

	wVer=MAKEWORD(2,2);
	if ((verst = WSAStartup(wVer, &WSAData)) != 0)
	{
		printf("iec104s_com: WSAStartup() return %d wrong!\n", verst);
		getchar();
		exit(-1);
	}

	if(argc != 2)
	{
		printf("\nUsage: %s para_id", argv[0]);
		getchar();
		exit(-1);
	}
	
	stn_no = atoi(argv[1]);
#endif
	
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
	
	//added by yyp 2011.03.11  get comm_cmd shm
	if(LinktoSemSet(SEM_WR_COMMCMD)==-1) 
	{
		printf("\nFail to LinktoSemSet(SEM_WR_COMMCMD)!");
		exit(-1);	
	}
	if(get_lan_shm(4)== FALSE)
	{
		printf("\nFail to get comm_cmd shm!");
		exit(-1);
	}	
#ifdef SUN_OS
	signal(SIGTERM,Endproc);
	signal(SIGINT,Endproc);

	strcpy( sProcRec[ProcNum].NameID, "WATCH_DOG" );
	sProcRec[ProcNum].Pid = getpid(); 
#else
	sprintf(tmp,"H9000 system:  iec104 protocol %s %s %s %s %s",sProcRec[stn_no].NameID,sProcRec[stn_no].Ip1,sProcRec[stn_no].Ip2,
		sProcRec[stn_no].Ip3,sProcRec[stn_no].Ip4);
	SetConsoleTitle ( tmp );

	hMutex= OpenMutex(MUTEX_ALL_ACCESS|SYNCHRONIZE, FALSE, "IEC104_CHILD");
	if(hMutex == NULL )
	{
		printf("\nFailed to open mutex IEC104_CHILD");
		exit(-1);
	}
#endif
	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
#ifdef SUN_OS
		printf("\niec104s: Fail to shmat shm_rec_addr: errno=%d\n",errno);
#else
		printf("\niec104s: Fail to shmat shm_rec_addr: errno=%d\n",GetLastError());
#endif
		exit(-1);
	}
	printf("\niec104s:: shmat shm_rec_addr: OK!\n");

	rcv_end = shm_rec_addr->head.header_ptr;

	wpid = -1;
	for(;;)
	{
#ifdef SUN_OS	
		host_state = -1;
		GetLocalHostState(&host_state);
		if( host_state!=1 )
		{
			printf("iec104: GetLocalHostState() is not Host.\n");
			sleep(2);
			continue;
		}
		for(i=0;i<ProcNum;i++)
		{			
			if((wpid == sProcRec[i].Pid)||(-1 == sProcRec[i].Pid))
			{
				host_state = -1;
				GetLocalHostState(&host_state);
				
				if(!test_flag)
				{
					pid=fork();   
				}
				else
				{
					pid=0;
				}
				if( pid==-1 )
				{
					printf("fork() for %s error.\n",sProcRec[i].NameID);
					sleep(5);
					continue;
				}								
				if( pid==0 )
				{
					CreateShareMemory((char)(SHM_BUF_KEY_CHAR+sProcRec[i].id), sizeof(SHM_BUF), "iec104 record");
                                        shm_buf_addr= (SHM_BUF*) LinktoShareMem( (SHM_BUF_KEY_CHAR+sProcRec[i].id) );
					InitData(sProcRec[i].NameID);
					link_num = 0;
					host_state = -1;
					GetLocalHostState(&host_state);
					if( host_state!=1 )
					{
						printf("iec104: GetLocalHostState() %s in STANDBY.\n",sProcRec[i].NameID);
					}
					MasterStnProc(i);
				}		
				if( pid!=0 )
				{
					sProcRec[i].Pid=pid;
				}

				printf("%s IEC104 Master ( %s, %s ,%s, %s ) start! pid=%5d ....\n",
					sProcRec[i].NameID,sProcRec[i].Ip1,sProcRec[i].Ip2,
					sProcRec[i].Ip3,sProcRec[i].Ip4, sProcRec[i].Pid );
			}
		}/* for(i)*/
		for(;;)
		{
			printf("IEC104 Master start wait() ....\n");
			wpid = wait(&stat_loc);
			if(wpid != -1)
				break;
		}
#else
		printf("\n....ProcNum=%d ....\n",stn_no);
		InitData(sProcRec[stn_no].NameID);
		link_num = 0;
		host_state = -1;
		GetLocalHostState(&host_state);
		if( host_state!=1 )
		{
			printf("\nIEC140: %s  This Communication Sever's State Is NOT In Host! Waiting!!!\n",sProcRec[stn_no].NameID);
		}
		MasterStnProc(stn_no);
	
#endif
	}

}

void MasterStnProc(int i)
{
	int Flag=1;
	USHORT	host_state;
	int FileNo;

	for(;;)
	{   
        ReadTime();
		oldday = ntime->tm_mday;
		FileNo=ntime->tm_hour+1;
		sprintf(logfilename,"/home/ems/h9000/his/iec104/IEC104-%s.%02d%02d%02d-%d",
	          sProcRec[i].NameID,ntime->tm_year+1900,ntime->tm_mon+1,ntime->tm_mday,FileNo);
		LogFileChangeDayRmLog();
		host_state = -1;  
		GetLocalHostState(&host_state);
		if( host_state!=1 )
		{
			continue;
		}
		switch( Flag )
		{
			case 1:
				if( MakeLink(sProcRec[i].NameID,(char *)sProcRec[i].Ip1)==false )
				{
					Flag=2;
					link_num ++;
					if(link_num >= 2)
					{
						link_num = 0;
						UpdateChannelFlag(sProcRec[i].NameID, (unsigned short)2);
					}
					sleep(1);
				}
				break;
			case 2:
				if( MakeLink(sProcRec[i].NameID,(char *)sProcRec[i].Ip2)==false )
				{
					Flag=3;
					link_num ++;
					if(link_num >= 2)
					{
						link_num = 0;
						UpdateChannelFlag(sProcRec[i].NameID, (unsigned short)2);
					}
					sleep(1);
				}
				break;
			case 3:
				if( MakeLink(sProcRec[i].NameID,(char *)sProcRec[i].Ip3)==false )
				{
					Flag=4;
					link_num ++;
					if(link_num >= 2)
					{
						link_num = 0;
						UpdateChannelFlag(sProcRec[i].NameID, (unsigned short)2);
					}
					sleep(1);
				}
				break;
			case 4:
				if( MakeLink(sProcRec[i].NameID,(char *)sProcRec[i].Ip4)==false )
				{
					Flag=1;
					link_num ++;
					if(link_num >= 2)
					{
						link_num = 0;
						UpdateChannelFlag(sProcRec[i].NameID, (unsigned short)2);
					}
					sleep(1);
				}
				break;
			default:
				continue;
		}
	}
}

#ifndef SUN_OS
void OpenNewTermalWindow ( void )
{
	STARTUPINFO				sui;
	int i;
	DWORD ret=-1;
	char	cmdline[128];
	char	err_buf[256];
	pid_t	ret_pid;

	hMutex= CreateMutex( NULL, TRUE, "IEC104_CHILD" );
	if ( GetLastError () == ERROR_ALREADY_EXISTS )
		return;

	if ( SetConsoleCtrlHandler ((void *)ExitRoutine,TRUE ) == 0 )
	{	
		printf("\n !!! iec104_com: SetConsoleCtrlHandler ERROR %d!!! \n",GetLastError());
		getchar();
		exit ( -1 );
	}

	GetStartupInfo(&sui);

	SetConsoleTitle ( "H9000 system: IEC870-5-104 communication process" );

	for(i=0;i<ProcNum;i++)
	{
		sprintf(cmdline,"%s %d",EXE_FILE,i);
		if( CreateProcess ( NULL,cmdline, NULL, NULL, FALSE,
							 CREATE_NEW_CONSOLE, NULL, NULL, &sui, &sProcRec[i].pi)==FALSE )

		{	
			printf("\n !!!CREATE CHILD WINDOW %s %d ERROR %d!!! \n",EXE_FILE,i,GetLastError());
		}
	}

	for ( ;; )
	{ 
		for(i=0;i<ProcNum;i++)
		{
			ret= WaitForSingleObject ( sProcRec[i].pi.hProcess, 0 );
			if( ret == WAIT_OBJECT_0 )
			{
				CloseHandle(sProcRec[i].pi.hThread);
				CloseHandle(sProcRec[i].pi.hProcess);
				printf("\n !!!Reboot %s %d CHILD WINDOW Now!!! \n",EXE_FILE,i);

				sprintf(cmdline,"%s %d",EXE_FILE,i);
				if( CreateProcess ( NULL,cmdline, NULL, NULL, FALSE,
									 CREATE_NEW_CONSOLE, NULL, NULL, &sui, &sProcRec[i].pi)==FALSE )
				{	
						printf("\n !!!Reboot CHILD WINDOW %s %d ERROR %d!!!\n",EXE_FILE,i,GetLastError());
				}
			}	
			sleep ( 2 );			
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

		if ( TerminateThread ( sProcRec[i].pi.hThread, uExitCode ) == 0 )
		{
			printf ( "ERROR in TerminateProcess (hThread ) %d ( num=%d ), Err=%d %d\n",sProcRec[i].pi.hThread,i,uExitCode,GetLastError());
		}
		printf("\n !!!iec104_com %d Terminate Now!!! \n",i);
		
	}
 	UnMountDB();
	exit (0 );
}
#endif

int MakeLink(char *STName, char *IpAddr)
{
	struct sockaddr_in	serv_addr, cli_addr;
	int	sock_c, cli_len;
	int	time_on;
	int	sframe_flag,unack_num,test_act;

	int	ret;
	
	fd_set	fset;
	struct timeval	tout;
	
	time_t	scan_tim,test_tim;
	time_t	tim;
	struct	tm	*timeptr;
	int	byte_recvd,pkt_len,count;
	USHORT	host_state;

	unsigned short	nRecvASDUId;
	unsigned int	recvbufheadpt;
	unsigned int	timeoutnum;
	int	interrogationflag,comm_flag;
	struct timeval	timeoval;
	int num=0;
	int FileNo;

	int CommLink;

	if( (sock_c=socket(AF_INET,SOCK_STREAM,0))<0 )
	{ 
		printf("%s: can't open SOCK_STREAM socket!\n",STName);
		return(false);
	}

#ifdef SUN_OS
	bzero (( char*)&cli_addr, sizeof ( cli_addr ));
#else
	memset((char *)&cli_addr,0,sizeof(struct sockaddr_in));
#endif
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_addr.s_addr = inet_addr(IpAddr);
	cli_addr.sin_port = htons(2404);

	timeout=NO;

#ifdef SUN_OS
	alarm(5);
	sigpipe = NO;
	signal(SIGPIPE,sig_pipe);
	signal(SIGALRM,Timer);
#endif
	
	if( (connect(sock_c,(struct sockaddr *)&cli_addr,sizeof(cli_addr))<0) || (timeout == YES) )
	{
#ifdef SUN_OS		
		alarm (0);
#endif
		printf("MASTER CONNECT TO %s %s Fail ...\n",STName,IpAddr);
		sprintf(infobuf,"MASTER CONNECT TO %s %s Fail ...\n",STName,IpAddr);
		WriteDspInfo(infobuf);
	    WriteLogFile(infobuf);
		close(sock_c); 
		sleep(1);
		return(false);		
	}
#ifdef SUN_OS
	alarm (0);
#endif
	printf("MASTER CONNECT TO %s %s OK ...\n",STName,IpAddr);
	sprintf(infobuf,"MASTER CONNECT TO %s %s OK ...\n",STName,IpAddr);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);

	time_on=1;
	if(setsockopt(sock_c,SOL_SOCKET,SO_KEEPALIVE,&time_on,sizeof (time_on))!=0)
	{
		printf("MASTER %s setsockopt SO_KEEPALIVE Fail ...\n",STName);
		sprintf(infobuf,"MASTER %s setsockopt SO_KEEPALIVE Fail ...\n",STName);
		WriteDspInfo(infobuf);
		WriteLogFile(infobuf);
	}

	timeoval.tv_sec=1;
	timeoval.tv_usec=0;
	
	if(setsockopt(sock_c,SOL_SOCKET,SO_SNDTIMEO,(char *)&time_on,sizeof (time_on))!=0)
	{
		printf("MASTER %s setsockopt SO_SNDTIMEO Fail ...\n",STName);
		sprintf(infobuf,"MASTER %s setsockopt SO_SNDTIMEO Fail ...\n",STName);
		WriteDspInfo(infobuf);
		WriteLogFile(infobuf);
	}	
	
	setsockopt(sock_c,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_on,sizeof (time_on));
	setsockopt(sock_c,SOL_SOCKET,SO_REUSEADDR,(char *)&time_on,sizeof (time_on));	

/*	IEC104 Network layer communication service */

	recvbufheadpt = 0;
	send_data.pnt = 0;
	recv_data.pnt = 0;
	localinitfg = 1;
	pollflag = 0;
	cmdsavbuf[6] = 0;
	selectcmd = 1;
	sframe_flag = 0;
	nr_recv = nr_send = nr_ack = nr_rs = 0;

	test_tim = 0;
	test_act = 0;
	

/* Init, Clock Syn, General interrogation */

	if(!StartDT(sock_c))
		return(false);

	memset( recvbuf, 0, MINFRAMELEN );
	timeout=NO;
#ifdef SUN_OS
	alarm(t1);
	signal(SIGALRM,Timer);
	byte_recvd=recv(sock_c,(char *)recvbuf,(int)MINFRAMELEN,(int)0);
	alarm(0);
	if( (byte_recvd!=6) || timeout==YES )
	{
		printf("\n%s %s StartDT timeout ... Close socket!\n",STName,IpAddr);
		sprintf(infobuf,"\n%s %s StartDT timeout ... Close socket!\n",STName,IpAddr);
		WriteDspInfo(infobuf);
		WriteLogFile(infobuf);
		close(sock_c);
		return(false);
	}
#else
	byte_recvd=recv(sock_c,(char *)recvbuf,(int)MINFRAMELEN,(int)0);
	if(byte_recvd == SOCKET_ERROR)
	{
		if(WSAETIMEDOUT != WSAGetLastError())
		{
			printf("iec104_com: %s %s recv() Error %d! do closesocket!\n",STName,IpAddr,WSAGetLastError());
			sprintf(infobuf,"iec104_com: %s %s recv() Error %d! do closesocket!\n",STName,IpAddr,WSAGetLastError());
			WriteDspInfo(infobuf);
		    WriteLogFile(infobuf);
			closesocket(sock_c);
			return(FALSE);
		}
#ifdef DEBUG
		printf("\n%s %s StartDT timeout ... Close socket!\n",STName,IpAddr);
		sprintf(infobuf,"\n%s %s StartDT timeout ... Close socket!\n",STName,IpAddr);
		WriteDspInfo(infobuf);
		WriteLogFile(infobuf);
#endif
		closesocket(sock_c);
		return(FALSE);
	}
	else if(byte_recvd == 0)
	{
		printf("iec104s_com: remote shutdown socket! do closesocket!\n");
		sprintf(infobuf,"iec104s_com: remote shutdown socket! do closesocket!\n");
		WriteDspInfo(infobuf);
		WriteLogFile(infobuf);
		closesocket(sock_c);
		return(FALSE);
	}
#endif

#ifdef DEBUG
	printf("\n%s received ACK StartDT Frame byte number %u:",STName,byte_recvd);
	sprintf(infobuf,"\n%s received ACK StartDT Frame byte number %u:\n",STName,byte_recvd);	
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintRecvData(byte_recvd);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif

	if( recvbuf[0]==0x68 && recvbuf[1]==0x04 && recvbuf[2]==(STARTDT_ACK | U_APCI) )
	{
		pollflag = 1;
		interrogationflag = 1;
	}
	else
	{
		printf("\n%s %s StartDT ACK Wrong ... Close socket!\n",STName,IpAddr);
		close(sock_c);
		return(false);
	}

	timeoutnum = 0;
	test_act = 0;
	link_num = 0;
	tim=time(&tim);
	scan_tim = test_tim = tim;
	//added by yyp 2011.03.09
	cur_stn_id=0;
	if(GetStnIdByName( STName, &cur_stn_id ) == -1)
	{
		printf("\nIEC104: STName %s error\n",STName);
		return(false);
	}
	if(cur_stn_id<1 || cur_stn_id>MAX_STN_NUM)
	{
		printf("\nIEC104: cur_stn_id %d error\n",cur_stn_id);
		return(false);
	}
	//yyp 2011.03.09
	//cmd_end = shm_rec_addr->head.header_ptr;//GCL
	cmd_end = shm_comm_cmd->stn_data[cur_stn_id].head;//yyp
	ClearCommShm(cur_stn_id,0,TYPE_ALL_SEG);
	for(;;)
	{
		
		FileNo=ntime->tm_hour+1;
		sprintf(logfilename,"/home/ems/h9000/his/iec104/IEC104-%s.%02d%02d%02d-%d",
	          STName,ntime->tm_year+1900,ntime->tm_mon+1,ntime->tm_mday,FileNo);
		LogFileChangeDayRmLog();
		host_state = -1;
		GetLocalHostState(&host_state);
		if( host_state!=1 )
		{
			printf("\nIEC104: Switched to Slave! %s close socket!\n",STName);
			close(sock_c);
			return(true);
		}
		if(sigpipe==YES)
		{
			printf("\nIEC140: %s socket %d broken! shutdown socket!\n",STName,sock_c);
			sprintf(infobuf,"\nIEC140: %s socket %d broken! shutdown socket!\n",STName,sock_c);
			WriteDspInfo(infobuf);
			WriteLogFile(infobuf);
			shutdown(sock_c,2);
			return(false);
		}
		tim=time(&tim);
		if((tim - scan_tim)>scan_period)
		{
			scan_tim = tim;
			interrogationflag = 1;
		}
		if(interrogationflag == 1)
		{
			if(!GeneralInterrogation(sock_c))
				return(false);
			scan_tim = tim;
			test_tim = tim;
			interrogationflag = 2;
		}

		if((test_act == 0) && ((tim - test_tim)>test_period_t3))
		{
			test_tim = tim;
			if(!TestFR(sock_c))
				return(false);
			test_act = 1;
		}
		if((test_act == 1) && (tim - test_tim)>t1)
		{
			printf("\nIEC140: %s receive Test ACK Frame timeout! close socket!\n",STName);
			sprintf(infobuf,"\nIEC140: %s receive Test ACK Frame timeout! close socket!\n",STName);
			WriteDspInfo(infobuf);
			WriteLogFile(infobuf);
			close(sock_c);
			return(false);
		}

		if(comdflag == 0)
		{
			if(!IEC870_5_Send(sock_c))
			{
				return(false);
			}
			else
			{
				if(comdflag == 1)
					test_tim = tim;
				cmdsavbuf[6] = 0;			/*cmd type clear zero */
			}
		}
		else if((tim - cmdbuf.sel_tim)>cmd_period)
		{
			comdflag = 0;
		}

		memset(recvbuf, 0, VARFRAMELEN);
		timeout=NO;
#ifdef SUN_OS
		alarm(2);
		byte_recvd=recv(sock_c,(char *)recvbuf,(int)MINFRAMELEN,(int)0);
		alarm(0);
#else
		FD_ZERO(&fset);
		FD_SET(sock_c,&fset);
		tout.tv_sec = 1;
		tout.tv_usec = 0;
		ret = select(sock_c+1, &fset, NULL, NULL, &tout);
		tim=time(&tim);
		if(ret <= 0)
		{
			printf("iec104_com: %s %s select() TIMEOUT! ret=%d\n",STName,IpAddr,ret);
			sprintf(infobuf,"iec104_com: %s %s select() TIMEOUT! ret=%d\n",STName,IpAddr,ret);
			WriteDspInfo(infobuf);
			WriteLogFile(infobuf);
			continue;
		}
	byte_recvd=recv(sock_c,(char *)recvbuf,(int)MINFRAMELEN,(int)0);
	if(byte_recvd == SOCKET_ERROR)
	{
		if(WSAETIMEDOUT != WSAGetLastError())
		{
			printf("iec104_com: %s %s recv() Error %d! do closesocket!\n",STName,IpAddr,WSAGetLastError());
			sprintf(infobuf,"iec104_com: %s %s recv() Error %d! do closesocket!\n",STName,IpAddr,WSAGetLastError());
			WriteDspInfo(infobuf);
			WriteLogFile(infobuf);
			closesocket(sock_c);
			return(FALSE);
		}
#ifdef DEBUG
		printf("\n%s %s StartDT timeout ... Close socket!\n",STName,IpAddr);
		sprintf(infobuf,"\n%s %s StartDT timeout ... Close socket!\n",STName,IpAddr);
		WriteDspInfo(infobuf);
		WriteLogFile(infobuf);
#endif
		closesocket(sock_c);
		return(FALSE);
	}
	else if(byte_recvd == 0)
	{
		printf("iec104s_com: remote shutdown socket! do closesocket!\n");
		sprintf(infobuf,"iec104s_com: remote shutdown socket! do closesocket!\n");
		WriteDspInfo(infobuf);
		WriteLogFile(infobuf);
		closesocket(sock_c);
		return(FALSE);
	}
#endif
		if( (byte_recvd!=MINFRAMELEN) || timeout==YES )
		{
			if((byte_recvd <= 0)&&( timeout!=YES ))
			{
				printf("iec104_com: %s client shutdown socket! do closesocket!\n",STName);
				sprintf(infobuf,"iec104_com: %s client shutdown socket! do closesocket!\n",STName);
				WriteDspInfo(infobuf);
				WriteLogFile(infobuf);
				close(sock_c);
				return(false);
			}
#ifdef DEBUG
			printf("iec104_com: %s recv() TIMEOUT! byte_recvd=%d\n",STName,byte_recvd);
			sprintf(infobuf,"iec104_com: %s recv() TIMEOUT! byte_recvd=%d\n",STName,byte_recvd);
			WriteDspInfo(infobuf);
			WriteLogFile(infobuf);
#endif
			if(sframe_flag)
			{
				if(!SFormatFR(sock_c))
					return(false);
				test_tim = tim;
				sframe_flag = 0;
			}
			timeoutnum++;
			if(timeoutnum%15==0)//GCL
			{
				printf("ReceiveData: %s timeout! No: %d",STName,timeoutnum);
				sprintf(infobuf,"ReceiveData: %s timeout! No: %d",STName,timeoutnum);
				WriteDspInfo(infobuf);
				WriteLogFile(infobuf);
				UpdateChannelFlag(STName, (unsigned short)2);
			}
			continue;
		}
		else
		{
			timeoutnum =0;
			test_tim = tim;

			if(byte_recvd!=MINFRAMELEN)
			{
				printf("%s Byte number received=%d diff than frame length %d\n",STName,byte_recvd,recvbuf[1]);
				sprintf(infobuf,"%s Byte number received=%d diff than frame length %d\n",STName,byte_recvd,recvbuf[1]);
				WriteDspInfo(infobuf);
				WriteLogFile(infobuf);
				continue;
			}
			UpdateChannelFlag(STName, (unsigned short)1);

			if((recvbuf[0]==0x68) && (recvbuf[1]==4) && ((recvbuf[2] & 0x3) == 3))	/*U Frame	*/
			{
#ifdef DEBUG
				printf("\n%s received U Frame data byte number %u:",STName,byte_recvd);
				sprintf(infobuf,"\n%s received U Frame data byte number %u:",STName,byte_recvd);
				WriteDspInfo(infobuf);
				WriteLogFile(infobuf);
				PrintRecvData(byte_recvd);
				WriteDspInfo(infobuf);
				WriteLogFile(infobuf);
#endif
				if(recvbuf[2] == 0x0B)	/*ACK START DT	*/
				{
					printf("\nIEC104: %s ACK START DT received!\n",STName);
					strcpy(infobuf,"\n确认STARTDT");
					WriteDspInfo(infobuf);
					WriteLogFile(infobuf);
					comm_flag = 1;
				}
				else if(recvbuf[2] == 0x07)	/*START DT	*/
				{
					comm_flag = 1;
					if(!ACKStartDT(sock_c))
						return(false);
					strcpy(infobuf,"\nSTARTDT");
					WriteDspInfo(infobuf);
					WriteLogFile(infobuf);
				}
				else if(recvbuf[2] == 0x13)	/*STOP DT	*/
				{
					comm_flag = 0;
					if(!ACKStopDT(sock_c))
						return(false);
					strcpy(infobuf,"\nSTOPDT");
					WriteDspInfo(infobuf);
					WriteLogFile(infobuf);
				}
				else if(recvbuf[2] == 0x43)	/*TEST FR	*/
				{
/*					comm_flag = 0;	*/
					if(!ACKTestFR(sock_c))
						return(false);
					strcpy(infobuf,"\n测试帧");
					WriteDspInfo(infobuf);
					WriteLogFile(infobuf);
				}
				else if(recvbuf[2] == 0x83)	/*ACK TEST FR	*/
				{
					printf("\nIEC104: %s ACK TEST FR received!\n",STName);
					test_act = 0;
					strcpy(infobuf,"\n确认测试帧");
					WriteDspInfo(infobuf);
					WriteLogFile(infobuf);
				}
				else if(recvbuf[2] == 0x23)	/*ACK Stop FR	*/
				{
					printf("\nIEC104: %s ACK Stop FR received!\n",STName);
					strcpy(infobuf,"\n确认停止帧");
					WriteDspInfo(infobuf);
					WriteLogFile(infobuf);
					close(sock_c);
					break;
				}
			}
			else if((recvbuf[0]==0x68) && (recvbuf[1]==4) && ((recvbuf[2] & 0x3) == 1))	/*S Frame	*/
			{	
#ifdef DEBUG
				printf("\n%s received S Frame data byte number %u:",STName,byte_recvd);
				sprintf(infobuf,"\n%s received S Frame data byte number %u:",STName,byte_recvd);
				WriteDspInfo(infobuf);
				WriteLogFile(infobuf);
				PrintRecvData(byte_recvd);
				WriteDspInfo(infobuf);
				WriteLogFile(infobuf);
#endif
#ifdef SUN
				CrossCopy(&nr_ack,&recvbuf[4],2);
#else
				memcpy(&nr_ack,&recvbuf[4],2);
#endif
			}
			else if((recvbuf[0]==0x68) && ((recvbuf[2] & 0x1) == 0))	/*I Frame	*/
			{
				pkt_len = recvbuf[1]+2;
				if(pkt_len <=6)
				{
					printf("iec104_com: %s receive I Frame length %d less than 4, shutdown socket!\n",STName,recvbuf[1]);
					close(sock_c);
					return(false);
				}
				count = 6;
				while (count < pkt_len)
				{			/* keep reading until "pkt_len" bytes are read	*/
					timeout=NO;
#ifdef SUN_OS
					alarm(2);
#endif
					byte_recvd = recv (sock_c, (char *)recvbuf+count, pkt_len-count, 0);
#ifdef SUN_OS
					alarm(0);
#endif
					if (byte_recvd <= 0)		/* 0 if link is disconnected, negative if error.*/
					{
						close(sock_c);
						return(false);
					}
					else
						count += byte_recvd;	/* add the bytes read to "count"	*/
				}	/* end "while" loop	*/

				byte_recvd = pkt_len;	
#ifdef DEBUG
				printf("\n%s received I Frame data byte number %u:",STName,byte_recvd);
				sprintf(infobuf,"\n%s received I Frame data byte number %u:",STName,byte_recvd);
				WriteDspInfo(infobuf);
				WriteLogFile(infobuf);
				PrintRecvData(byte_recvd);
				WriteDspInfo(infobuf);
				WriteLogFile(infobuf);
#endif
#ifdef SUN
				CrossCopy(&nr_rs,&recvbuf[2],2);
				CrossCopy(&nr_ack,&recvbuf[4],2);
#else
				memcpy(&nr_rs,&recvbuf[2],2);
				memcpy(&nr_ack,&recvbuf[4],2);
#endif
				if(nr_rs != nr_recv)
				{
					printf("iec104s_com: %s Received Nr %d not equiv V(R) %d client shutdown socket!\n",STName,nr_rs,nr_recv);
					close(sock_c);
					return(true);
				}
				nr_recv +=2;
				if(ns_ack > nr_recv)
				{
					unack_num = 0x10000 + nr_recv - ns_ack;
				}
				else
					unack_num = nr_recv - ns_ack;
				if(unack_num >= W*2)
				{
					if(!SFormatFR(sock_c))
						return(false);
					sframe_flag = 0;
				}
				else
					sframe_flag = 1;

#ifdef SUN
				CrossCopy(&nRecvASDUId,&recvbuf[10],2);
#else
				memcpy(&nRecvASDUId,&recvbuf[10],2);
#endif
				if(nRecvASDUId != nASDUId)
				{
					printf("%s received station id = %d not equal to request %d\n",STName,nRecvASDUId,nASDUId);
					sprintf(infobuf,"%s received station id = %d not equal to request %d\n",STName,nRecvASDUId,nASDUId);
					WriteDspInfo(infobuf);
					WriteLogFile(infobuf);
					continue;
				}
#ifdef DEBUG
				printf("%s A Variable Frame is Received! pt=%d\n",STName,recvbufheadpt);
				sprintf(infobuf,"%s A Variable Frame is Received! pt=%d\n",STName,recvbufheadpt);
				WriteDspInfo(infobuf);
				WriteLogFile(infobuf);
#endif
				memset(&recv_data.data[recvbufheadpt],0,VARFRAMELEN);
				memcpy(&recv_data.data[recvbufheadpt],&recvbuf[6],pkt_len - 6);
				recv_data.length[recvbufheadpt] =(unsigned short) (pkt_len - 6);
				IEC870_5_Recv(STName,recvbufheadpt,sock_c);

				recvbufheadpt ++;
				if(recvbufheadpt >= ASDU_ENTRY_NUMBER)
					recvbufheadpt = 0;
				recv_data.pnt = recvbufheadpt;
			}
			timeoutnum =0;
		}/*if(I S U Frame) */
	}/*for(;;) */
/*	UnMountDB();
*/
}

int UpdateChannelFlag(char *pName, unsigned short status)  
{
	char	name_buf[64];
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

	strcpy(name_buf,SYS_TAB_NAME);
	strcat(name_buf,pName);
	strcat(name_buf,hostname);
	
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		printf("%s No this com channel status point in the database file!\n",name_buf);
		return(false);
	}
	else if(dbentry.var.ind.status.rt_state != status)
	{
		lanbcdata.type_id =DPS_IND_ALM;
		lanbcdata.stn_id =dbentry.point.stn_id;
		lanbcdata.dev_id =dbentry.point.dev_id;
		lanbcdata.data_type=dbentry.point.data_type;
		lanbcdata.number = 1;
		lanbcdata.point_id = dbentry.point.pt_id;

		lanbcdata.status=status+0x0100;	/*communication channel status: 0:normal; 1:Fail */
		lanbcdata.ms=0;
		GetLocalHostId(&lanbcdata.host_id);

		tim=time (NULL);
		tmpt=*localtime (&tim);
		lanbcdata.sec=tmpt.tm_sec;
		lanbcdata.min=tmpt.tm_min;
		lanbcdata.hour=tmpt.tm_hour;
		lanbcdata.day=tmpt.tm_mday;
		lanbcdata.month=tmpt.tm_mon+1;
		lanbcdata.year=tmpt.tm_year+1900;

		lanhd.length=sizeof(SYS_MSG);
#ifdef SUN
		Swap2Byte(&lanbcdata.number);
		Swap2Byte(&lanbcdata.point_id);
		Swap2Byte(&lanbcdata.status);
		Swap2Byte(&lanbcdata.ms);
		Swap2Byte(&lanbcdata.year);
		Swap2Byte(&lanhd.length);
#endif
		lanhd.dp_type=BJ_DB_TIME;

		lanhd.dest_id =0;
		lanhd.src_id =lanbcdata.host_id;
		lanhd.dest_stn =lanbcdata.stn_id;
		lanhd.packet_num =0;
		lan_out ((char *)&lanbcdata,lanhd,(unsigned char)0);

		pnt = dbentry.point;
		new_value.ind.rt_state = status;
		new_value.ind.fault_tag = status;
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

	}
	return(true);
}


void WriteDspInfo(char *buf)
{
	int	bufptr,len,charlen;
	
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
	}
}
  

void    WriteLogFile (char logtxt[])
{
	char tmp[64];
    fp = fopen(logfilename, "a");
    if ( fp != NULL)
    {
    		GetSysTimeStr(tmp);
		fwrite( tmp, strlen( tmp ),1,fp);
        fwrite( logtxt, strlen( logtxt ),1,fp);
        fclose(fp);
    }
}

void ReadTime()
{
    time_t          shjian;	
    time ( &shjian );
    ntime = localtime ( &shjian );
}

void    LogFileChangeDayRmLog()
{
    char Str[128]="",Str1[128]="";
    int i;
	int Year,Month,Day,tmp_Day,tmp_Month;
    if( oldday != ntime->tm_mday )
    { 
		if(LogSaveDayNum<30)
		{
			if (ntime->tm_mday>LogSaveDayNum)
			{
				Year = ntime->tm_year+1900;
				Month = ntime->tm_mon+1;
				Day = ntime->tm_mday-LogSaveDayNum-1;
			}
			else
			{
				Year = ntime->tm_year+1900;
				if(ntime->tm_mon>=1)
					Month = ntime->tm_mon;
				else if(ntime->tm_mon==0)
					Month = 12;
				if (((ntime->tm_mon+1)%2==1)&&((ntime->tm_mon+1)!=2))
					tmp_Day = 31;
				else if (ntime->tm_mon+1==2)
				{
					if((ntime->tm_year+1900)%4==0)
						tmp_Day = 29;
					else
						tmp_Day = 28;
				}
				else
					tmp_Day = 30;
				Day = ntime->tm_mday+tmp_Day-LogSaveDayNum-1;
			}
		}
		else 
		{
			tmp_Month = LogSaveDayNum/30;
			Year = ntime->tm_year+1900;
			if (ntime->tm_mon>tmp_Month)
				Month = ntime->tm_mon+1-tmp_Month;
			else
				Month = ntime->tm_mon+1+12-tmp_Month;
			Day = ntime->tm_mday;
		}
#ifdef SUN_OS
		strcpy(Str,"rm /home/ems/h9000/his/iec104/");
#else
		strcpy(Str,"del /home/ems/h9000/his/iec104/");
#endif
		if(LogSaveDayNum<30)
		{
			for(i=0;i<Month;i++)
			{
				sprintf(Str1,"IEC104*.%02d%02d*",Year,i+1);
				strcat(Str,Str1);
				system(Str);
			}
			for(i=0;i<Day;i++)
			{
				sprintf(Str1,"IEC104*.%02d%02d%02d*",Year,Month,i);
				strcat(Str,Str1);
				system(Str);
			}
			
		}
		else
		{
			for(i=0;i<Month;i++)
			{
				sprintf(Str1,"IEC104*.%02d%02d*",Year,i+1);
				strcat(Str,Str1);
				system(Str);
			}
		}		
		oldday = ntime->tm_mday;
    }
	if((fp=fopen(logfilename,"r"))==NULL)
	{
		fp=fopen(logfilename, "w");
		strcpy(Str,"**************************************************\n");
		fwrite(Str,strlen(Str),1,fp);
		strcpy(Str,"      IWHR Technology\n");
		fwrite(Str,strlen(Str),1,fp);
		strcpy(Str,"      H9000 V4.0 iec104 Record\n");
		fwrite(Str,strlen(Str),1,fp);
		strcpy(Str,"**************************************************\n");
		fwrite(Str,strlen(Str),1,fp);
	}
	fclose(fp);  
}

void ReadCOMMConfig()
{
	FILE	*fp;
	char filename[100];
	char keyword[30]="";
	char StrTemp[40]="";
	int i;
	char err_str[80]="";

	strcpy(filename,HOMEPATH);
	strcat(filename,COMM_CONIFG_FILE);
	if( (fp=fopen(filename, "r"))==NULL )
	{
		printf("Fail to open %s\n", filename);
		exit(0);
	}
	strcpy(keyword,"COMM_NUM");
	if(-1==LocateKeyword(fp,keyword))
		exit(0);
	fscanf(fp,"%s",StrTemp);
	comm_num = atoi(StrTemp);
	printf("comm_num=%d\n",comm_num);

	strcpy(keyword,"COMM_CONFIG");
	if(-1==LocateKeyword(fp,keyword))
		exit(0);
	for(i=0;i<comm_num;i++)
	{
		fscanf(fp,"%s %s %s %s %s %s %s",comm_config[i].stn,comm_config[i].hostname1,comm_config[i].hostname2,comm_config[i].hostname3,
			comm_config[i].hostname4,comm_config[i].protocol,StrTemp);
		comm_config[i].priority=atoi(StrTemp);
		printf("%s %s %s %s %s %s %s\n",comm_config[i].stn,comm_config[i].hostname1,comm_config[i].hostname2,comm_config[i].hostname3,
			comm_config[i].hostname4,comm_config[i].protocol,StrTemp);
	}

	strcpy(keyword,"LogSaveDayNum");
	if(-1==LocateKeyword(fp,keyword))
		exit(0);
	fscanf(fp,"%s",StrTemp);
	LogSaveDayNum = atoi(StrTemp);
	printf("LogSaveDayNum=%d\n",LogSaveDayNum);

	strcpy(keyword,"SYS_TAB_NAME");
	if(-1==LocateKeyword(fp,keyword))
		exit(0);
	fscanf(fp,"%s",SYS_TAB_NAME);
	printf("SYS_TAB_NAME=%s\n",SYS_TAB_NAME);
}
