/************************************************************
*  FILE NAME:           iec104_com.c   				        *
*  FUNCTION:            Controlling Station Main Process	*
*			Link Layer Communication Program	            *
*  NAME :  					        	                    *
*  Version :            Win3.0a-->Win4.0a  by  zwj      	*	     
*  DATE:                2008.11                			    *
************************************************************/
#include "../inc/gis_iec104_com.h"

#ifdef		SUN_OS
#include 	<sys/ipc.h>
#include 	<sys/sem.h>
#include 	<sys/shm.h>
#include 	<sys/socket.h>  /* for AF_INET        */
#include 	<sys/filio.h>
#include 	<netinet/in.h>  /* for struct in_addr */
#include 	<arpa/inet.h>   /* for inet_ntoa()    */
#include 	<sys/types.h>
#include	<fcntl.h>
#include 	<netdb.h>       /* for struct hostent */
#include	<errno.h>
#include	<wait.h>
#endif



int			ProcNum;
PROC_REC	sProcRec[MAX_PROC_NUM+1];

#ifdef WINDOWS_OS
HANDLE	hMutex;
#define errno WSAGetLastError()
#endif


int			stn_no=-1;
struct	net_cmd		*shm_net_cmd_addr;
SHM_RECORD *shm_rec_addr;

int Dualhost=1;
int secondHostFlag=0;
int mainChanAbnormalFlag;
char local_hostname[36], pair_hostname[36];
char chan9698CommPtName[ALL_CHANNEL_NUM][256];

BOOL GetCosBroadcastPrvg(char *stn_name, int ip_no)
{
	USHORT		comm_state, host_state;
	
	if(!Dualhost)
		return TRUE;
	
	if(-1==GetLocalHostState(&host_state))
		return TRUE;
	//if(host_state==1) //main host
	if(host_state==1 || host_state==4) //main host or controlnet break
		return TRUE;
	
	//vice host: check main host comm state
	GetHostCommState(pair_hostname, stn_name, ip_no, &comm_state);
	if(comm_state) //main host comm state OK
		return FALSE;
	else //main host comm state FAIL
		return TRUE;
}

BOOL CheckLocalHostWorkState(void)
{
	USHORT	host_state;

	if(-1==GetLocalHostState(&host_state))
		return TRUE;
	//if(host_state==1 || host_state==2) //main or vice
	if(host_state==1 || host_state==2 || host_state==4) //main or vice or controlnet break
		return TRUE;

	return FALSE;
}

BOOL InitHostName(void)
{
	gethostname(local_hostname, sizeof(local_hostname));
	if(Dualhost)
	{
		if(-1==GetDhswHostName(pair_hostname))
			return FALSE;
		if(strcmp(pair_hostname, local_hostname)>0)
			secondHostFlag= 0;
		else
			secondHostFlag= 1;
		return TRUE;
	}
	return TRUE;
}


#ifdef WINDOWS_OS

void MainExitRoutine ( DWORD signal_type )
{
	int		i;
	UINT	uExitCode=0;
	
	for(i=0;i<ProcNum;i++)
	{
		OutputDspInfo(0, "IEC104 Master stop station %s", sProcRec[i].NameID);
		if ( TerminateProcess ( sProcRec[i].pi.hProcess, uExitCode ) == 0 )
		{
			OutputDspInfo(0, "ERROR in TerminateProcess %d ( num=%d ), Err=%d %d\n", sProcRec[i].pi.hProcess,i,uExitCode,GetLastError());
		}
		
		if ( TerminateThread ( sProcRec[i].pi.hThread, uExitCode ) == 0 )
		{
			OutputDspInfo(0, "ERROR in TerminateProcess (hThread ) %d ( num=%d ), Err=%d %d\n",sProcRec[i].pi.hThread,i,uExitCode,GetLastError());
		}
		
	}
	
	OutputDspInfo(0, "IEC104 Master watchdog exit");
	exit (0 );
}
void ChildExitRoutine ( DWORD signal_type )
{
	int	i;
	
	for(i=0; i<sProcRec[stn_no].ip_num; i++)
		UpdateHostCommState(local_hostname, sProcRec[stn_no].NameID, i, 0);
	OutputDspInfo(0, "IEC104 station %s exit", sProcRec[stn_no].NameID);
	
	exit (0 );
}

void OpenNewTermalWindow ( void )
{
	STARTUPINFO				sui;
	int i;
	DWORD	ret=-1;
	char	cmdline[128];
	char	filename[256];
	HANDLE  childProc[MAX_PROC_NUM];
	
	hMutex= CreateMutex( NULL, TRUE, "IEC104_CHILD" );
	if ( GetLastError () == ERROR_ALREADY_EXISTS )
		return;
	
	if ( SetConsoleCtrlHandler ((void *)MainExitRoutine,TRUE ) == 0 )
	{	
		OutputDspInfo(0, "SetConsoleCtrlHandler ERROR %d!!! \n",GetLastError());
		exit ( -1 );
	}
	
	GetStartupInfo(&sui);
	
	SetConsoleTitle ( "H9000通信系统: IEC870-5-104 通信主进程" );
	
	GetModuleFileName(NULL, filename, sizeof(filename));
	
	for(i=0;i<ProcNum;i++)
	{
		sprintf(cmdline,"%s %d",filename,i);
		if( CreateProcess ( NULL,cmdline, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &sui, &sProcRec[i].pi)==FALSE )
		{
			OutputDspInfo(0, "CREATE CHILD WINDOW %s %d ERROR %d!!! \n",filename,i,GetLastError());
			childProc[i]= (HANDLE)-1;
		}
		else
		{
			OutputDspInfo(0, "start CHILD WINDOW %s %d success\n",filename,i);
			childProc[i]= sProcRec[i].pi.hProcess;
		}
	}
	
	for ( ;; )
	{ 
		ret= MsgWaitForMultipleObjects(ProcNum, childProc, FALSE, INFINITE, 0);
		i= ret-WAIT_OBJECT_0;
		if( i>=0 && i<ProcNum  )
		{
			CloseHandle(sProcRec[i].pi.hThread);
			CloseHandle(sProcRec[i].pi.hProcess);
			OutputDspInfo(0, "%s %d CHILD WINDOW exited\n",filename,i);
			
			sprintf(cmdline,"%s %d",filename,i);
			if( CreateProcess ( NULL,cmdline, NULL, NULL, FALSE,
				CREATE_NEW_CONSOLE, NULL, NULL, &sui, &sProcRec[i].pi)==FALSE )
			{
				OutputDspInfo(0, "reboot CHILD WINDOW %s %d ERROR %d\n",filename,i,GetLastError());
				childProc[i]= (HANDLE)-1;
			}
			else
			{
				OutputDspInfo(0, "restart CHILD WINDOW %s %d success\n",filename,i);
				childProc[i]= sProcRec[i].pi.hProcess;
			}
		}
		else
			OutputDspInfo(0, "MsgWaitForMultipleObjects fail, ERROR %d!!!\n",GetLastError());
		
	}
}
#endif //WINDOWS_OS


#ifdef SUN_OS
void MainEndproc(int sig_no)
{
	int i;

	for(i=0;i<ProcNum;i++)
	{
		OutputDspInfo(0, "IEC104 Master stop station %s", sProcRec[i].NameID);
		kill( sProcRec[i].Pid,SIGTERM );
	}
	OutputDspInfo(0, "IEC104 Master watchdog exit");
	exit(0);
} 
                                                                
void ChildEndproc(int sig_no)
{
	int i;
	
	for(i=0; i<sProcRec[stn_no].ip_num; i++)
		UpdateHostCommState(local_hostname, sProcRec[stn_no].NameID, i, 0);
	OutputDspInfo(0, "IEC104 station %s exit", sProcRec[stn_no].NameID);
	exit(0);
} 

#endif /*SUN_OS*/


void InitNetLib(void)
{
#ifdef WINDOWS_OS
	WORD   		wVer;
	WSADATA 	WSAData;
	int 		verst;

	wVer=MAKEWORD(2,2);
	if ((verst = WSAStartup(wVer, &WSAData)) != 0)
	{
		OutputDspInfo(0, "WSAStartup() return %d wrong!\n", verst);
		exit(-1);
	}
#endif
}


int main(int argc,char *argv[])
{ 
	InitOutputFilter("IEC104", -1);

	InitNetLib();

	if(-1==MountDB(NULL))
	{
		OutputDspInfo(0, "Fail to MountDB!");
		exit(-1);
	}
	
	if(lan_out_init() == FALSE)
	{
		OutputDspInfo(0, "Fail to lan_out_init()!");
		exit(-1);
	}

	if(!InitHostName())
	{
		OutputDspInfo(0, "Fail to init hosts name");
		exit(-1);
	}
	
	for(;;)
	{
		if(CheckLocalHostWorkState())
			break;
		OutputDspInfo(OUTPUT_INFO, "wait local host state become main or vice");
		sleep(1);
	}

	CreateCommDspBuf();

	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		OutputDspInfo(0, "Fail to shmat shm_rec_addr: errno=%d\n",errno);
		exit(-1);
	}
	OutputDspInfo(0, "shmat shm_rec_addr: OK!\n");
	
	shm_net_cmd_addr= (struct net_cmd*) LinktoShareMem( SHM_NETCMD_KEY_CHAR );
	if( shm_net_cmd_addr == (struct net_cmd*)-1 )
	{
		OutputDspInfo(0, "Fail to shmat shm_net_cmd: errno=%d\n",errno);
		exit(-1);
	}
	OutputDspInfo(0, "shmat shm_net_cmd: OK!\n");
	
	for(;;)
	{
		if( ReadStnDef(STN_DEF_FILE) == true )
			break;
            	
		OutputDspInfo(0, "Read Station define file %s Error!\n",STN_DEF_FILE);
		sleep(5);
	}

#ifdef WINDOWS_OS
	{
		char	tmp[256]; 
		HANDLE	hMutex;
		
		OpenNewTermalWindow();

		if( SetConsoleCtrlHandler ((void *)MainExitRoutine,TRUE ) == 0 )
		{	
			OutputDspInfo(0, "SetConsoleCtrlHandler ERROR %d!!! \n",GetLastError());
			exit ( -1 );
		}

		if(argc != 2)
		{
			OutputDspInfo(0, "Usage: %s para_id", argv[0]);
			exit(-1);
		}
		stn_no = atoi(argv[1]);
		
		InitOutputFilter("IEC104", stn_no);
		
		sprintf(tmp,"H9000通信系统:  iec104 协议 %s %s %s %s %s",sProcRec[stn_no].NameID,sProcRec[stn_no].ip[0],sProcRec[stn_no].ip[1],
			sProcRec[stn_no].ip[2],sProcRec[stn_no].ip[3]);
		SetConsoleTitle ( tmp );
		
		OutputDspInfo(0, "%s IEC104 Master ( %s, %s, %s, %s) start! ....\n",
			sProcRec[stn_no].NameID,sProcRec[stn_no].ip[0],sProcRec[stn_no].ip[1],sProcRec[stn_no].ip[2],sProcRec[stn_no].ip[3]);
		
		hMutex= OpenMutex(MUTEX_ALL_ACCESS|SYNCHRONIZE, FALSE, "IEC104_CHILD");
		if(hMutex == NULL )
		{
			OutputDspInfo(0, "Failed to open mutex IEC104_CHILD");
			exit(-1);
		}
		
		for(;;)
			MasterStnProc(stn_no);
	}

#else /*SUN_OS*/
	{
		int	pid, wpid, i;

		signal(SIGTERM,MainEndproc);
		signal(SIGINT,MainEndproc);
		
		for(i=0;i<ProcNum;i++)
		{
			pid=fork();
			if( pid==-1 )
			{
				OutputDspInfo(0, "fork() for %s error.\n",sProcRec[i].NameID);
				continue;
			}								
			else if( pid==0 ) //child
			{
				signal(SIGTERM,ChildEndproc);
				signal(SIGINT,ChildEndproc);

				stn_no= i;
				MasterStnProc(stn_no);
				exit(0);
			}		

			sProcRec[i].Pid=pid;
			
			OutputDspInfo(0, "%s IEC104 Master start! pid=%d ....\n",
				sProcRec[i].NameID,sProcRec[i].Pid );
		}/* for(ProcNum)*/

		for(;;) //watchdog
		{
			wpid = wait(NULL);
			if(wpid == -1)
				continue;
			for(i=0;i<ProcNum;i++)
			{
				if(wpid == sProcRec[i].Pid)
				{
					pid=fork();
					if( pid==-1 )
					{
						OutputDspInfo(0, "fork() for %s error.\n",sProcRec[i].NameID);
						continue;
					}								
					if( pid==0 ) //child
					{
						signal(SIGTERM,ChildEndproc);
						signal(SIGINT,ChildEndproc);
						
						stn_no= i;
						MasterStnProc(stn_no);
						exit(0);
					}		
					
					sProcRec[i].Pid=pid;
					
					OutputDspInfo(0, "%s IEC104 Master restart! pid=%d ....\n",
						sProcRec[i].NameID,sProcRec[i].Pid );
					break;
				}
			}/* for(ProcNum)*/
		}
	}

#endif

}



int	SetSocketNoBlock(int sockfd)
{
	int		ret;
	
	ret= 1;
#ifdef	WINDOWS_OS
	if( ioctlsocket(sockfd, FIONBIO, &ret) )
#else
	if( -1==ioctl(sockfd, FIONBIO, &ret) )
#endif
	{ 
		OutputDspInfo(0, "Fail to set socket NONBLOCK");
		return -1;
	}
	return 0;	
}


int	CreateSockNoBlock(void)
{
	int		sockfd;
	
	if(-1==(sockfd=socket(AF_INET, SOCK_STREAM, 0)))
	{ 
		OutputDspInfo(0, "Fail to create stream socket");
		return -1;
	}
	
	if(-1==SetSocketNoBlock(sockfd))
	{ 
		closesocket(sockfd);
		return -1;
	}
	return sockfd;
}


int SocketReadable(int sock, int ms)
{
	fd_set	rset;
	struct timeval tv;
	int		len, error,ret;
	
	len= sizeof(error);
	getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
	FD_ZERO(&rset);
	FD_SET(sock, &rset);
	tv.tv_sec= ms/1000;
	tv.tv_usec= (ms%1000)*1000;
	ret= select(sock+1, &rset, NULL, NULL, &tv);
	return ret;
}

int SetSocket(int sockfd)
{
	int		time_on;
	int		sockopt_on = 1; /* value for turning on boolean socket option */
	
	time_on=1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_KEEPALIVE,(char *)&time_on,sizeof (time_on)))
	{
		OutputDspInfo(OUTPUT_ERROR, "ERROR %d setting SO_KEEPALIVE socket option", errno);
		return -1;
	}
	
#ifdef TCP_NODELAY
	if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (const char *) &sockopt_on, sizeof (int) ) )
	{
		OutputDspInfo(OUTPUT_ERROR, "ERROR %d setting NODELAY socket option", errno);
		return -1;
	}
#endif
	return 0;
}

int ConnectClient(char *STName, char *ipaddr)
{
	int		sock_c;
	int		ret, error, len;
	fd_set	wset;
	struct	timeval tv;
	struct sockaddr_in	cli_addr;

	if((sock_c=CreateSockNoBlock())<0 )
	{ 
		OutputDspInfo(0, "%s: can't open SOCK_STREAM socket!\n",STName);
		return -1;
	}
	
	bzero (( char*)&cli_addr, sizeof ( cli_addr ));
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_addr.s_addr = inet_addr(ipaddr);
	cli_addr.sin_port = htons(2404);
	
	ret= connect(sock_c, (struct sockaddr *)&cli_addr, sizeof(cli_addr));
	if(ret == 0)
		return sock_c;               /* connect completed immediately */
	
	/*or else ret==-1 SOCKET_ERROR*/
#ifdef	WINDOWS_OS
	if(WSAGetLastError() != WSAEWOULDBLOCK)
#else
	if(errno != EINPROGRESS)
#endif
	{
		OutputDspInfo(0,  "fail to connect %s", ipaddr);
		closesocket(sock_c);
		return -1;
	}
	
	len= sizeof(error);
	getsockopt(sock_c, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
	FD_ZERO(&wset);
	FD_SET(sock_c, &wset);
	tv.tv_sec= 0;
	tv.tv_usec= 200*1000;
	ret= select(sock_c+1, NULL, &wset, NULL, &tv);
	if(ret == 0)
	{
		OutputDspInfo(0,  "connect %s timeout", ipaddr);
		closesocket(sock_c);
		return -1;
	}
	else if(ret < 0)
	{
		OutputDspInfo(0,  "select connect %s error", ipaddr);
		closesocket(sock_c);
		return -1;
	}
	else /* ret > 0 */
	{
		len= sizeof(error);
		getsockopt(sock_c, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
		if(error)
		{
			OutputDspInfo(0,  "connect %s refused", ipaddr);
			closesocket(sock_c);
			return -1;
		}				
	}

	if(-1==SetSocket(sock_c))
	{
		closesocket(sock_c);
		return -1;
	}				

	return sock_c;
}


BOOL CheckCtrlPriv9698CommState(char *ptName)
{
	USHORT	state;
	
	if(!ptName[0]) //no init
	{
		OutputDspInfo(OUTPUT_ERROR, "9698 link state point name is null");
		return TRUE;
	}
	if(-1==GetIndPtRtStateByNameStr(ptName, &state))
	{
		OutputDspInfo(OUTPUT_ERROR, "no such 9698 link state point in db: %s", ptName);
		return TRUE;
	}
	return state;
}
BOOL CheckCtrlPrivChanState(int stn_no, unsigned short ctrlPrivChan)
{
	unsigned short chan_norm_state;

	if(ctrlPrivChan<2) //0,1 A
	{
		if(secondHostFlag) //B
			GetHostCommState(pair_hostname, sProcRec[stn_no].NameID, ctrlPrivChan, &chan_norm_state);
		else //A
			GetHostCommState(local_hostname, sProcRec[stn_no].NameID, ctrlPrivChan, &chan_norm_state);
	}
	else //2,3 B
	{
		if(secondHostFlag) //B
			GetHostCommState(local_hostname, sProcRec[stn_no].NameID, ctrlPrivChan-2, &chan_norm_state);
		else //A
			GetHostCommState(pair_hostname, sProcRec[stn_no].NameID, ctrlPrivChan-2, &chan_norm_state);
	}	
	return chan_norm_state;
}

void ClearOutputFilterState(void);
void MasterStnProc(int stn_no)
{
	int ip_no;
	COMM_INFO comm_info[MAX_IEC104_IPADDR_NUM];
	unsigned short	old_ctrlPrivChan;
	int	checkElseHostFlag=0;

	ClearOutputFilterState();
	InitOutputFilter("IEC104", stn_no);

	if(sProcRec[stn_no].ip_num!=2)
	{
		OutputDspInfo(OUTPUT_ERROR, "station %s ipaddr num must be 2: now %d, exit", 
			sProcRec[stn_no].NameID, sProcRec[stn_no].ip_num);
		return;
	}

	for(ip_no=0; ip_no<sProcRec[stn_no].ip_num; ip_no++)
	{
		pthread_t	h_thread;

		memset(&comm_info[ip_no], 0, sizeof(COMM_INFO));
		comm_info[ip_no].ctrl_priv= 0;
		comm_info[ip_no].getCtrlPrivFlag= 0;
		comm_info[ip_no].ip_no= ip_no;
		strcpy(comm_info[ip_no].stn_name, sProcRec[stn_no].NameID);
		strcpy(comm_info[ip_no].ipaddr, sProcRec[stn_no].ip[ip_no]);


#ifdef	WINDOWS_OS
		if(!CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MakeLink, &comm_info[ip_no], 0, NULL))
#else			
		if(pthread_create(&h_thread, NULL, MakeLink, &comm_info[ip_no]))
#endif
		{
			OutputDspInfo(OUTPUT_ERROR, "Fail to create thread Makelink stn %s %s!", comm_info[ip_no].stn_name, comm_info[ip_no].ipaddr);
			return;
		} 
		else
			OutputDspInfo(OUTPUT_INFO, "Success to create thread Makelink stn %s %s!", comm_info[ip_no].stn_name, comm_info[ip_no].ipaddr);
	
		Sleep(500);
	}
	
	Sleep(2000);
	GetCtrlPrivChannel(sProcRec[stn_no].NameID, local_hostname, &old_ctrlPrivChan);
	for(;;)
	{
		unsigned short ctrlPrivChan;
		USHORT	pair_host_state;
		int chanSwitchFlag= 0;

		if(-1==GetCtrlPrivChannel(sProcRec[stn_no].NameID, local_hostname, &ctrlPrivChan))
			ctrlPrivChan= 0;
		ctrlPrivChan= ctrlPrivChan%4; //max 4 comm channel
		OutputDspInfo(0, "ctrl priv channel: %d", ctrlPrivChan);

		for(ip_no=0; ip_no<sProcRec[stn_no].ip_num; ip_no++)
		{
			if(ip_no==(ctrlPrivChan-secondHostFlag*2))
			{
				if(comm_info[ip_no].ctrl_priv==0)
					comm_info[ip_no].getCtrlPrivFlag= 1;
				comm_info[ip_no].ctrl_priv= 1;
			}
			else
				comm_info[ip_no].ctrl_priv= 0;
			OutputDspInfo(0, "chan %d ctrl priv state: %d", ip_no, comm_info[ip_no].ctrl_priv);
		}
		
		if(old_ctrlPrivChan!=ctrlPrivChan)
		{
			old_ctrlPrivChan= ctrlPrivChan;
			Sleep(2000);
			mainChanAbnormalFlag= 0;
			continue;
		}
		old_ctrlPrivChan= ctrlPrivChan;
		
		if(!CheckCtrlPrivChanState(stn_no, ctrlPrivChan) 
			|| !CheckCtrlPriv9698CommState(chan9698CommPtName[ctrlPrivChan]))
			mainChanAbnormalFlag= 1;

		if(mainChanAbnormalFlag)
		{
			unsigned short new_ctrlPrivChan;
			int	i;
			int	findNormChanFlag= 0;

			OutputDspInfo(OUTPUT_ERROR, "need to switch main chan");
			for(i=0; i<4; i++)
			{
				new_ctrlPrivChan= (ctrlPrivChan+i+1)%4;
				if(CheckCtrlPrivChanState(stn_no, new_ctrlPrivChan) 
					&& CheckCtrlPriv9698CommState(chan9698CommPtName[new_ctrlPrivChan]))
				{
					OutputDspInfo(OUTPUT_ERROR, "chan %d state ok: switch to this chan", new_ctrlPrivChan);
					UpdateCtrlPrivChan(sProcRec[stn_no].NameID, local_hostname, new_ctrlPrivChan);
					mainChanAbnormalFlag= 0;
					findNormChanFlag= 1;
					break;
				}
			}

			if(!findNormChanFlag && !CheckCtrlPrivChanState(stn_no, ctrlPrivChan))
			{
				OutputDspInfo(OUTPUT_ERROR, "main chan %d 104 break: find 104 ok chan", ctrlPrivChan);
				for(i=0; i<4; i++)
				{
					new_ctrlPrivChan= (ctrlPrivChan+i+1)%4;
					if(CheckCtrlPrivChanState(stn_no, new_ctrlPrivChan))
					{
						OutputDspInfo(OUTPUT_ERROR, "chan %d 104 ok: switch to this chan", new_ctrlPrivChan);
						UpdateCtrlPrivChan(sProcRec[stn_no].NameID, local_hostname, new_ctrlPrivChan);
						break;
					}
				}
			}
		}
		
		checkElseHostFlag++;
		if(!(checkElseHostFlag%30))
		{
			if(-1==GetHostState(pair_hostname, &pair_host_state) || pair_host_state==0)
			{
				int		i;
				for(i=0; i<sProcRec[stn_no].ip_num; i++)
					UpdateHostCommState(pair_hostname, sProcRec[stn_no].NameID, i, 0);
			}
		}
		
		Sleep(100);		
	}
}


void* MakeLink(void *para)
{
	int		sock;
	IEC104_CMM_INFO *info=NULL;
	COMM_INFO *comm_info= (COMM_INFO*)para;
	
	InitOutputFilter("IEC104", stn_no);

	info= malloc(sizeof(IEC104_CMM_INFO));
	if(!info)
	{
		OutputDspInfo(OUTPUT_ERROR, "Fail to malloc IEC104_CMM_INFO space: %d", sizeof(IEC104_CMM_INFO));
		return (void*)0;
	}
	memset(info, 0, sizeof(IEC104_CMM_INFO));

	InitData(info, comm_info->stn_name, comm_info->ip_no);

	for(;;)
	{
		sock= ConnectClient(comm_info->stn_name, comm_info->ipaddr);
		if( sock==-1 )
		{
			OutputDspInfo(OUTPUT_ERROR, "Fail to connect %s", comm_info->ipaddr);
			UpdateHostCommState(local_hostname, sProcRec[stn_no].NameID, comm_info->ip_no, 0);
		}
		else
		{
			OutputDspInfo(OUTPUT_INFO, "Success to connect %s", comm_info->ipaddr);
			info->sock= sock;
			MakeComm(info, comm_info->stn_name, comm_info);
			closesocket(sock);
		}
		
		Sleep(200);
	}

}



int MakeComm(IEC104_CMM_INFO *info, char *STName, COMM_INFO *comm_info)
{
	int	sframe_flag,unack_num,test_act;
	time_t	interrogation_tim,test_tim_s;
	int		test_time_ms, test_tim_diff_ms;
	time_t	curr_tim_s;
	DATE_TIME	curr_time;
	int	byte_recvd,pkt_len,count;
	int	comm_state=0;
	unsigned short	nRecvASDUId;
	unsigned int	recvbufheadpt;
	int		interrogationflag=0;
	int		dt_enable_flag;
	int		ret;

	/*	IEC104 Network layer communication service */
	recvbufheadpt = 0;
	info->send_data.pnt = 0;
	info->recv_data.pnt = 0;
	info->cmdsavbuf[6] = 0;
	sframe_flag = 0;
	info->nr_recv = info->nr_send = info->nr_ack = info->nr_rs = 0;

	dt_enable_flag= 0;

	GetSysTimeEx(&curr_time, &curr_tim_s);
	info->cmdbuf.sel_tim= test_tim_s= curr_tim_s;
	test_time_ms= curr_time.ms;

	for(;;)
	{
		if(dt_enable_flag==0)
		{
			info->cmd_end = shm_net_cmd_addr->head;
			if(!StartDT(info))
				return(false);
		}

		if(comm_info->getCtrlPrivFlag)
		{
			comm_info->getCtrlPrivFlag= 0;
			BroadcastSoeRec(info, comm_info);
		}
		
		if(dt_enable_flag && !comm_info->ctrl_priv)
		{
			CheckRecvSoeRec(info, comm_info);
		}
		
		GetSysTimeEx(&curr_time, &curr_tim_s);
		if((curr_tim_s - interrogation_tim)>info->scan_period)
		{
			interrogation_tim = curr_tim_s;
			interrogationflag = 1;
		}
		if(interrogationflag == 1 && dt_enable_flag==1)
		{
			if(!GeneralInterrogation(info))
				return(false);
			interrogation_tim = curr_tim_s;
			interrogationflag = 2;
		}
		
		test_tim_diff_ms= (curr_tim_s - test_tim_s)*1000 + curr_time.ms - test_time_ms;
		if((test_act == 0) && test_tim_diff_ms>t3)
		{
			test_tim_s = curr_tim_s;
			test_time_ms= curr_time.ms;
			if(!TestFR(info))
				return(false);
			test_act = 1;
		}
		test_tim_diff_ms= (curr_tim_s - test_tim_s)*1000 + curr_time.ms - test_time_ms;
		if((test_act == 1) && test_tim_diff_ms>t1)
		{
			OutputDspInfo(0, "\nreceive Test ACK Frame timeout! close socket!\n");
			return(false);
		}
		
		if(info->comdflag == 0 && dt_enable_flag==1)
		{
			if(!IEC870_5_Send(info, comm_info))
				return(false);
			else
			{
				if(info->comdflag == 1)
				{
					test_tim_s = curr_tim_s;
					test_time_ms= curr_time.ms;
				}
				info->cmdsavbuf[6] = 0;			/*cmd type clear zero */
			}
		}
		else if((curr_tim_s - info->cmdbuf.sel_tim)>info->cmd_period)
		{
			OutputDspInfo(0, "wait command ack timeout, cancel command: %d %d", curr_tim_s,info->cmdbuf.sel_tim);
			info->comdflag = 0;
		}

		memset( info->recvbuf, 0, MINFRAMELEN );
		ret= SocketReadable(info->sock, 200); //ms
		if(ret == 0)
		{
			OutputDspInfo(0, "recv frame timeout");
			UpdateHostCommState(local_hostname, sProcRec[stn_no].NameID, comm_info->ip_no, 1);

			if(sframe_flag)
			{
				if(!SFormatFR(info))
					return(false);
				test_tim_s = curr_tim_s;
				test_time_ms= curr_time.ms;
				sframe_flag = 0;
			}
			continue;
		}
		else if(ret < 0)
		{
			OutputDspInfo(0, "recv frame error, close socket!\n");
			return false;
		}

		byte_recvd=recv(info->sock,(char *)info->recvbuf,(int)MINFRAMELEN,(int)0);
		if(byte_recvd!=MINFRAMELEN)
		{
			OutputDspInfo(0, "recv frame error: ret=%d, close socket!\n", byte_recvd);
			return(false);
		}
		if(info->recvbuf[0]!=0x68)
		{
			OutputDspInfo(0, "recv frame error: head=0x%x, close socket!\n", info->recvbuf[0]);
			return(false);
		}
		
		test_act = 0;
		test_tim_s = curr_tim_s;
		test_time_ms= curr_time.ms;
		
		if(0==comm_state)
		{
			UpdateHostCommState(local_hostname, sProcRec[stn_no].NameID, comm_info->ip_no, 1);
			comm_state= 1;
		}

		if((info->recvbuf[1]==4) && ((info->recvbuf[2] & 0x3) == 3))	/*U Frame	*/
		{
			OutputDspInfo(0, "received U Frame");
			PrintRecvData(info->recvbuf, byte_recvd);

			if(info->recvbuf[2] == 0x0B)	/*ACK START DT	*/
			{
				OutputDspInfo(0, "received StartDT ACK Frame");
				interrogationflag = 1;
				dt_enable_flag= 1;			
			}
			else if(info->recvbuf[2] == 0x23)	/*ACK STOP DT	*/
			{
				OutputDspInfo(0, "received StopDT ACK Frame");
			}
			else if(info->recvbuf[2] == 0x43)	/*TEST FR	*/
			{
				OutputDspInfo(0, "received Test Frame");
				if(!ACKTestFR(info))
					return(false);
			}
			else if(info->recvbuf[2] == 0x83)	/*ACK TEST FR	*/
			{
				OutputDspInfo(0, "received  Test ACK Frame");
			}
		}
		else if((info->recvbuf[1]==4) && ((info->recvbuf[2] & 0x3) == 1))	/*S Frame	*/
		{
			OutputDspInfo(0, "received S Frame");
			PrintRecvData(info->recvbuf, byte_recvd);

			memcpy(&info->nr_ack,&info->recvbuf[4],2);
			Swap2Byte(&info->nr_ack);
		}
		else if((info->recvbuf[0]==0x68) && ((info->recvbuf[2] & 0x1) == 0))	/*I Frame	*/
		{
			pkt_len = info->recvbuf[1]+2;
			if(pkt_len <=MINFRAMELEN)
			{
				OutputDspInfo(0, "receive I Frame length %d less than 4, shutdown socket!\n",info->recvbuf[1]);
				return(false);
			}
			count = MINFRAMELEN;
			while (count < pkt_len)
			{
				ret= SocketReadable(info->sock, 50);
				if(ret <= 0)
				{
					OutputDspInfo(0, "recv frame error, close socket!\n");
					return(false);
				}
				
				byte_recvd = recv(info->sock, (char *)info->recvbuf+count, pkt_len-count, 0);
				if (byte_recvd <= 0)		/* 0 if link is disconnected, negative if error.*/
				{
					OutputDspInfo(0, "recv frame error, close socket!\n");
					return(false);
				}
				else
					count += byte_recvd;	/* add the bytes read to "count"	*/
			}

			byte_recvd = pkt_len;
			OutputDspInfo(0, "received I Frame");
			PrintRecvData(info->recvbuf, byte_recvd);

			if(dt_enable_flag==0)
			{
				OutputDspInfo(0, "received I Frame but DT is stopped, discard frame");
				continue;				
			}

			memcpy(&info->nr_rs,&info->recvbuf[2],2);
			memcpy(&info->nr_ack,&info->recvbuf[4],2);
			Swap2Byte(&info->nr_rs);
			Swap2Byte(&info->nr_ack);

			if(info->nr_rs != info->nr_recv)
			{
				OutputDspInfo(0, "received Nr %d not equiv V(R) %d client shutdown socket!\n",info->nr_rs,info->nr_recv);
				return(true);
			}
			info->nr_recv +=2;
			if(info->ns_ack > info->nr_recv)
			{
				unack_num = 0x10000 + info->nr_recv - info->ns_ack;
			}
			else
				unack_num = info->nr_recv - info->ns_ack;

			if(unack_num >= W*2)
			{ 
				if(!SFormatFR(info))
					return(false);
				sframe_flag = 0;
			}
			else if(unack_num > 0)
				sframe_flag = 1;

			memcpy(&nRecvASDUId,&info->recvbuf[10],2);
			Swap2Byte(&nRecvASDUId);

			if(nRecvASDUId != info->nASDUId)
			{
				OutputDspInfo(0, "received station id = %d not equal to request %d\n",nRecvASDUId,info->nASDUId);
				continue;
			}
			OutputDspInfo(0, "A Variable Frame is Received! buf_header=%d\n",recvbufheadpt);
			memset(&info->recv_data.data[recvbufheadpt],0,VARFRAMELEN);
			memcpy(&info->recv_data.data[recvbufheadpt],&info->recvbuf[6],pkt_len - 6);
			info->recv_data.length[recvbufheadpt] =(unsigned short) (pkt_len - 6);
			
			IEC870_5_Recv(info, comm_info, STName,recvbufheadpt);
			

			recvbufheadpt ++;
			if(recvbufheadpt >= ASDU_ENTRY_NUMBER)
				recvbufheadpt = 0;
			info->recv_data.pnt = recvbufheadpt;
		}/*if(I S U Frame) */
	}/*for(;;) */
}

