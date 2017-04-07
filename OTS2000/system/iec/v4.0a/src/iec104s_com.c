/****************************************************************
*  FILE NAME:           iec104s_com.c      			*
*  FUNCTION:            Controlled Station Main Process		*
*			Link Layer Communication Program	*
*  NAME :  			              			*
*  DATE:                Apr,2006                		*
*								*
****************************************************************/
#include	<stdarg.h>

#include "../inc/iec104s_com.h"
#include "../../../dps/v4.0a/inc/dps_functions_def.h"

#define	DEBUG
#define HAVE_MSGHDR_MSG_CONTROL

int		ProcNum;
PROC_REC	sProcRec[MAX_PROC_NUM+1];

extern	ASDUCOMDDATA	recv_data;
extern	ASDUDATA	send_data;
extern	SPDEF	spdef;
extern	DPDEF	dpdef;
extern	STDEF	stdef;
extern	MEDEF	medef;
extern	DCDEF	dcdef;
extern	SEDEF	sedef;
extern	ITDEF	itdef;
extern	SCDEF	scdef;
extern	RCDEF	rcdef;


unsigned char recvbuf[1024],cmdsendbuf[24];
unsigned char sendbuf[1024];

char	infobuf[1024];
IEC104S_STATION_BUF *dsp_buf_addr=(IEC104S_STATION_BUF *)-1;
IEC104S_SHARE_BUF *share_addr=(IEC104S_SHARE_BUF *)-1;

unsigned short nr_recv,nr_send,nr_ack,ns_ack;
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

int	sigpipe;
int	timeout;
int	ClockSyncflag,DT_Enable_Flag,comdflag,cmdterm;
int	localinitflag=1;
int	Readflag,Testflag,ResetProcessflag;
int	Paramflag,MirrorNAckflag;
int	ClkDiffflag,soe_overflow_flag,soe_overflow_Sendflag;
int	interrogationflag,counterflag,icgroupflag,cigroupflag;
int	delayflag;
int	cyclicflag,scanflag;
int	cmd_ack;

unsigned short	delayalarm;
unsigned char	cot_interrogation,cot_counter;
unsigned int	repeatnum,timeoutnum;
unsigned short	seladdr;

unsigned short	nASDUId;
unsigned char	nSrcId;
unsigned char	cmd_src;
int 	listen_sock, link_sock;

int	Gps;		/* 0: Without GPS; 1: With GPS */
int	GiType;		/* General interrogation type> 1: M_SP_NA; 20: M_PS_NA */
int	MeASDUType;	/* Me ASDU type> 0: M_ME_NA; 1: M_ME_NB; 3: M_ME_NC */
int	MeType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
int	SEType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
short	me_code;	/* 0 or -32768 */
short	se_code;	/* 0 or -32768 */
int	TimeTag;	/* Time tag> 0: no; 1: short; 2: long */
int	sp_cos;		/* sp_cos> 0: sp/dp transmit once; 1: twice */
int	Cmd_Sel;	/* 0: cmd without select; 1: cmd with select */

time_t	scan_tim;
time_t	scan_period;
time_t	cmd_period;
time_t	cmd_trans_delay;
time_t	no_recv_tim;
time_t	tim_waitack[65536];

#ifdef REC_YF_AGC_CMD
extern	unsigned short	daysaved;
#endif

int	Create_NewIEC104Console(int stn_id, int sock);
void ResetOrgData(void);
void Endproc(int sig_no);

#ifdef WINDOWS_OS
#define		errno	WSAGetLastError()
#endif

int		ouput_info_filter= OUTPUT_ERROR;
FILE	*dsp_info_file;
char	host_name[36];
                                                                
int local_stn_id;

typedef	struct
{
	UCHAR			type_id;
	UCHAR			stn_id;
	UCHAR			dev_id;
	UCHAR			data_type;
	USHORT			pt_num;
	USHORT			pt_id;
	ANLG_STATUS		ana_status;
	char			data[4];
	UCHAR			host_id;
    UCHAR			second;
    UCHAR			minute;
	UCHAR			hour;
	UCHAR			day;
	UCHAR			month;
	USHORT			year;
}SP_ANA_INFO_PKG1;



void GetSysShortTimeStr1( char *timeStr)
{
	DATE_TIME	date_time;
	
	GetSysTime(&date_time);
	sprintf(timeStr, "%02d:%02d:%02d.%03d", date_time.hour,
		date_time.minute, date_time.second, date_time.ms);
}

int BroadCastAnaPt1(POINTER point, UCHAR net, float value, ANLG_STATUS status, DATE_TIME *date_time)
{
	LAN_HEAD		head;
	SP_ANA_INFO_PKG1	info;
	UCHAR			hostId;
	GULONG			data;
	
	head.length= sizeof(SP_ANA_INFO_PKG1);
	Swap2Byte(&head.length);
	head.dp_type= BJ_DB_TIME;
	head.dest_id= 0;
	if(-1==GetLocalHostId(&head.src_id))
		return -1;
	head.dest_stn= GetModuleId();
	
	if(status.alm_type)
		info.type_id= DPS_ANA_ALM;
	else
		info.type_id= DPS_ANALOG;
	info.stn_id= point.stn_id;
	info.dev_id= point.dev_id;
	info.data_type= point.data_type;
	info.pt_num= 1;
	Swap2Byte(&info.pt_num);
	info.pt_id= point.pt_id;
	Swap2Byte(&info.pt_id);
	memcpy(&info.ana_status, &status, 2);
	Swap2Byte(&info.ana_status);
	data= (GULONG)(value * 1000.);
	memcpy(info.data, &data, 4);
	Swap4Byte(&info.data);
	GetLocalHostId(&hostId);
	info.host_id= (char)hostId;
	info.second= date_time->second;
	info.minute= date_time->minute;
	info.hour= date_time->hour;
	info.day= date_time->day;
	info.month= date_time->month;
	info.year= date_time->year;
	Swap2Byte(&info.year);
	
	lan_out((char*)&info, head, net);
	return 0;
}

USHORT	local_host_normal_state;

void CheckHostState(void)
{
	unsigned short state;
	
	if(-1!=GetLocalHostState(&state))
	{
		if(state==1 || state==2)
			local_host_normal_state= 1;
		else
			local_host_normal_state= 0;
	}
	else
		local_host_normal_state= 1;
}

void InitOutputFilter(void)
{
	char dsp_filename[256];
	
	ouput_info_filter= OUTPUT_ERROR;
	sprintf(dsp_filename, "%sIEC104S_LOG.def", IEC104_HOMEPATH);
	if(dsp_info_file= fopen(dsp_filename, "r"))
	{
		char	buff[256];
		
		while(fgets(buff,256,dsp_info_file))
		{
			if(buff[0]=='!' || buff[0]=='#')
				continue;
			
			if(1!=sscanf(buff, "%d", &ouput_info_filter))
				continue;

			break;
		}
		
		fclose(dsp_info_file);
	}
	
	sprintf(dsp_filename, "%sIEC104S_COM_%s_LOG.txt", IEC104_LOGPATH, sProcRec[local_stn_id].NameID);
	dsp_info_file= fopen(dsp_filename, "w");
}




#ifdef	SUN_OS
void Waitchild(int nouse)
{
	int status,wpid,i;
	
	wpid = wait(&status);
	OutputDspInfo(OUTPUT_ERROR, "recv child exit signal: pid= %d", wpid);
	if(wpid != -1)
	{
		for(i=0;i<ProcNum;i++)
		{
			if(sProcRec[i].Pid == wpid)
			{
				OutputDspInfo(OUTPUT_ERROR,"station %d %s is exited! ( pid=%d )",i,sProcRec[i].NameID, sProcRec[i].Pid);
				sProcRec[i].Pid = -1;
				break;
			}
		}
	}
	signal(SIGCHLD,Waitchild); 
}

void MainExitRoutine ( int signal )
{
	int		i;
	UINT	uExitCode= 0;
	
	shutdown(listen_sock, 2);
	closesocket(listen_sock);
	RemoveShareMemory('a', "IEC104S");
	
	for(i=0;i<ProcNum;i++)
	{
		if(sProcRec[i].Pid != -1)
		{
			kill( sProcRec[i].Pid, SIGTERM );		
			OutputDspInfo(OUTPUT_ERROR, "now start to terminate %s iec104s_com", sProcRec[i].NameID);
		}
	}
	OutputDspInfo(OUTPUT_ERROR, "iec104s_com 通信主进程 exit Now");
	exit (0 );
}

void ChildExitRoutine ( int signal )
{
	UINT	uExitCode= 0;
	
	UpdateChannelFlag(local_stn_id,(unsigned short)1);
	
	shutdown(link_sock, 1);
	closesocket(link_sock);
	
	OutputDspInfo(OUTPUT_ERROR, "%s iec104s_com exit", sProcRec[local_stn_id].NameID);
	if(dsp_info_file)
		fclose(dsp_info_file);
	exit (0 );
}

void*	CreateIEC104ShareMemory(void)
{
	char	err_str[128];
	int		shm_id;
	void	*shm_addr;
	key_t	key;
	char	keyfile[128];
	char	*shm_name= "IEC104S";
	
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	if( (key=ftok(keyfile, 'a'))== -1 )
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to generate %s key: %d!", shm_name, errno);
		return (void*)-1;
	}
	if( (shm_id=shmget(key, sizeof(IEC104S_SHARE_BUF), IPC_CREAT|0660)) == -1 )
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to create %s share memory: %d!", shm_name, errno);
		return (void*)-1;
	}
    
    if( (shm_addr=shmat(shm_id, 0, 0)) == (void*)-1 )
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to shmat %s share memory: %d!", shm_name, errno);
		return (void*)-1;
	}
	OutputDspInfo(0,"\nCreate %s share memory: 0x%x", shm_name, key);
	return shm_addr;
}

void*	LinktoIEC104ShareMem(void)
{
	key_t	key;
	int		shm_id;
	char	err_str[128];
	char	keyfile[128];
	
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	if( (key=ftok(keyfile, 'a'))== -1 )
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to generate key!\n");
		return (void*)-1;
	}
	if( (shm_id=shmget(key, 0, 0660)) == -1 )
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to shmget share memory!");
		return (void*)-1;
	}
	
    return shmat(shm_id, 0, 0);
}

int write_fd(int fd, void *ptr, int nbytes, int sendfd)   
{   
    struct msghdr msg;   
    struct iovec iov[1]; 
	
    // 有些系统使用的是旧的msg_accrights域来传递描述符，Linux下是新的msg_control字段   
#ifdef HAVE_MSGHDR_MSG_CONTROL   
    union{ // 前面说过，保证cmsghdr和msg_control的对齐   
        struct cmsghdr cm;   
        char control[CMSG_SPACE(sizeof(int))];   
    }control_un;   
    struct cmsghdr *cmptr;    
    // 设置辅助缓冲区和长度   
    msg.msg_control = control_un.control;    
    msg.msg_controllen = sizeof(control_un.control);   
    // 只需要一组附属数据就够了，直接通过CMSG_FIRSTHDR取得   
    cmptr = CMSG_FIRSTHDR(&msg);   
    // 设置必要的字段，数据和长度   
    cmptr->cmsg_len = CMSG_LEN(sizeof(int)); // fd类型是int，设置长度   
    cmptr->cmsg_level = SOL_SOCKET;    
    cmptr->cmsg_type = SCM_RIGHTS;  // 指明发送的是描述符   
    *((int*)CMSG_DATA(cmptr)) = sendfd; // 把fd写入辅助数据中   
#else   
    msg.msg_accrights = (caddr_t)&sendfd; // 这个旧的更方便啊   
    msg.msg_accrightslen = sizeof(int);   
#endif   
    // UDP才需要，无视   
    msg.msg_name = NULL;   
    msg.msg_namelen = 0;   
    // 别忘了设置数据缓冲区，实际上1个字节就够了   
    iov[0].iov_base = ptr;   
    iov[0].iov_len = nbytes;   
    msg.msg_iov = iov;   
    msg.msg_iovlen = 1;   
    return sendmsg(fd, &msg, 0);   
}   

int read_fd(int fd, void *ptr, int nbytes, int *recvfd)   
{   
    struct msghdr msg;   
    struct iovec iov[1];   
    int n;   
    int newfd;   
#ifdef HAVE_MSGHDR_MSG_CONTROL   
    union{ // 对齐   
		struct cmsghdr cm;   
		char control[CMSG_SPACE(sizeof(int))];   
    }control_un;   
    struct cmsghdr *cmptr;   
    // 设置辅助数据缓冲区和长度   
    msg.msg_control = control_un.control;   
    msg.msg_controllen = sizeof(control_un.control);   
#else   
    msg.msg_accrights = (caddr_t) &newfd; // 这个简单   
    msg.msg_accrightslen = sizeof(int);   
#endif    
	
    // TCP无视   
    msg.msg_name = NULL;   
    msg.msg_namelen = 0;   
    // 设置数据缓冲区   
    iov[0].iov_base = ptr;   
    iov[0].iov_len = nbytes;   
    msg.msg_iov = iov;   
    msg.msg_iovlen = 1;   
    // 设置结束，准备接收   
    if((n = recvmsg(fd, &msg, 0)) <= 0)   
    {   
        return n;   
    }   
#ifdef HAVE_MSGHDR_MSG_CONTROL   
    // 检查是否收到了辅助数据，以及长度，回忆上一节的CMSG宏   
    cmptr = CMSG_FIRSTHDR(&msg);   
    if((cmptr != NULL) && (cmptr->cmsg_len == CMSG_LEN(sizeof(int))))   
    {   
		// 还是必要的检查   
        if(cmptr->cmsg_level != SOL_SOCKET)   
        {   
            printf("control level != SOL_SOCKET\n");   
            exit(-1);   
        }   
        if(cmptr->cmsg_type != SCM_RIGHTS)   
        {   
            printf("control type != SCM_RIGHTS\n");   
            exit(-1);   
        }   
		// 好了，描述符在这   
        *recvfd = *((int*)CMSG_DATA(cmptr));   
    }   
    else  
    {   
        if(cmptr == NULL) printf("null cmptr, fd not passed.\n");   
        else printf("message len[%d] if incorrect.\n", cmptr->cmsg_len);   
        *recvfd = -1; // descriptor was not passed   
    }   
#else   
    if(msg.msg_accrightslen == sizeof(int)) *recvfd = newfd;    
    else *recvfd = -1;   
#endif   
    return n;   
}  


int InitSocketPair(int socketPair[][2])
{
	int	i;

	for(i=0;i<ProcNum;i++)
	{
		if(socketpair(AF_UNIX, SOCK_STREAM, 0, socketPair[i]))
		{
			OutputDspInfo(OUTPUT_ERROR,"\nFail to create stn %d socketpair: errno=%d", i, errno);
			return -1;
		}
		OutputDspInfo(0,"\nSuccess to create stn %d socketpair: %d %d", i, socketPair[i][0], socketPair[i][1]);
	}
	return 0;
}
int SendNewSocket(int pipe, int new_sock, int stn_id)
{
	char value;

	int ret= write_fd(pipe, &value, 1, new_sock);	
	if(ret!=1)
	{
		OutputDspInfo(OUTPUT_ERROR,"\nFail to send station %d new socket: ret= %d %d", stn_id, ret, pipe);
		return -1;
	}
	return 0;
}

int GetNewSocket(int pipe, int stn_id)
{
	char	value;
	int	new_sock= -1;
	int	n= read_fd(pipe, &value, 1, &new_sock);
	if(n!=1)
	{
		OutputDspInfo(OUTPUT_ERROR,"\nFail to recv station %d new socket", stn_id);
		return -1;
	}
	return new_sock;
}

BOOL	CreateMutex( char *filename )
{
	int		fd, val;
	struct	flock lock;
	char	err_str[128];
	char	name[256];
	char	buf[16];
	
	sprintf(name, "%s%s", SYS_TMP_PATH, filename);
	if( (fd=open(name, O_WRONLY|O_CREAT, 0644)) == -1 )
	{
		OutputDspInfo(OUTPUT_ERROR, "Fail to open %s!\n", name);
		exit(-1);
    }	
	
	lock.l_type= F_WRLCK;
	lock.l_start= 0;
	lock.l_whence= SEEK_SET;
	lock.l_len= 0;
	if( fcntl(fd, F_SETLK, &lock) < 0 )
	{
		if(errno==EACCES || errno==EAGAIN)	
			return FALSE;
		else
        {
			OutputDspInfo(OUTPUT_ERROR, "Fail to write_lock %s!\n", filename);
			exit(-1);
		}
	}
	
    if( ftruncate(fd, 0) < 0 )
    {
		OutputDspInfo(OUTPUT_ERROR, "Fail to truncate %s!\n", filename);
		exit(-1);
    }
	
    sprintf(buf, "%d\n", getpid());
    if( write(fd, buf, strlen(buf)) != strlen(buf) )
    {
		OutputDspInfo(OUTPUT_ERROR, "Fail to write pid to %s!\n", filename);
		exit(-1);
    }
	
	val= fcntl(fd, F_GETFD, 0);
	if(val<0)
    {
		OutputDspInfo(OUTPUT_ERROR, "Fail to fcntl F_GETFD %s!\n", filename);
		exit(-1);
    }
	
	val |= FD_CLOEXEC;
	if(fcntl(fd, F_SETFD, val)<0)
    {
		OutputDspInfo(OUTPUT_ERROR, "Fail to fcntl F_SETFD %s!\n", filename);
		exit(-1);
    }
	return TRUE;
}

int MainCommProc(void)
{
	int		stn_id;
	int 	status;
	int		child_process_flag;
	unsigned short iec104port;
	SOCKADDR_IN    	clientaddr;
	int socketPair[MAX_PROC_NUM][2];
	char			rmt_ip_str[256];
	char			tmp[256];

	OutputDspInfo(0,"iec104 通信主进程启动！\n");
	
	share_addr= (IEC104S_SHARE_BUF*)CreateIEC104ShareMemory();
	if(share_addr == (IEC104S_SHARE_BUF*)-1)
	{
		OutputDspInfo(OUTPUT_ERROR,"\nFail to create share memory");
		exit(-1);
	}
	memset(share_addr, 0, sizeof(IEC104S_SHARE_BUF));
	
	if(-1==InitSocketPair(socketPair))
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to InitSocketPair!\n");
		exit(-1);
	}
	
	signal(SIGTERM, MainExitRoutine);
	signal(SIGINT, MainExitRoutine);
	signal(SIGSEGV, MainExitRoutine);
	signal(SIGCHLD, Waitchild);

	for(;;)
	{
		int		time_on;
		
		CheckHostState();
		if(!local_host_normal_state)
		{
			OutputDspInfo(OUTPUT_WARN, "local host state abnormal, stop work");
			Sleep(5000);
			continue;
		}


		listen_sock = socket(AF_INET, SOCK_STREAM, 0);
            
		if (listen_sock == INVALID_SOCKET)
		{
			OutputDspInfo(OUTPUT_ERROR,"creat socket() failed!\n");
			Sleep(5000);
			continue;
		}

		time_on=1;
		if(setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,(char *)&time_on,sizeof (time_on)))
		{
			OutputDspInfo(OUTPUT_ERROR, "ERROR %d setting SO_REUSEPORT socket option", errno);
			closesocket(listen_sock);
			Sleep(5000);
			continue;
		}
		
		iec104port = (unsigned short)IEC104_PORT;
		memset((char *)&clientaddr,0,sizeof(struct sockaddr_in));
		clientaddr.sin_family      = AF_INET;
		clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		clientaddr.sin_port        = htons(iec104port);  

		while(bind(listen_sock,(struct sockaddr*)&clientaddr,sizeof(clientaddr))==SOCKET_ERROR)
		{
			OutputDspInfo(OUTPUT_ERROR,"iec104s_com:: bind() error: %d!\n", errno);
			closesocket(listen_sock);
			Sleep(5000);
			continue;
		}

		if(listen(listen_sock,4)<0)
		{
			OutputDspInfo(OUTPUT_ERROR,"iec104s_com::listen() error: %d!\n", errno);
			closesocket(listen_sock);
			Sleep(5000);
			continue;
		}

		for(;;)
		{
			int		clientaddrlen;
			int		new_sock;


			OutputDspInfo(OUTPUT_ERROR,"main waiting for connection...\n");
			
			clientaddrlen=sizeof(clientaddr);
			new_sock=accept(listen_sock,(struct sockaddr *) &clientaddr,(socklen_t *)&clientaddrlen);
			
			if(new_sock<0)
			{
				OutputDspInfo(OUTPUT_ERROR,"accept() return %d wrong!\n", errno);
				continue;
			}
			link_sock= new_sock;
			
			CheckHostState();
			if(!local_host_normal_state)
			{
				OutputDspInfo(OUTPUT_WARN, "local host state abnormal, stop work");
				closesocket(link_sock);
				break;
			}

			strcpy(rmt_ip_str,inet_ntoa(clientaddr.sin_addr));
			OutputDspInfo(OUTPUT_WARN,"new connection! ip=%s socket=%d\n",rmt_ip_str,link_sock);
			
			for(stn_id = 0; stn_id < ProcNum; stn_id++)
			{
				if(strcmp(rmt_ip_str,sProcRec[stn_id].Ip) == 0)
					break;
			}
			if(stn_id >= ProcNum)
			{
				OutputDspInfo(OUTPUT_ERROR,"remote ip=%s not find in file %s! close socket!\n",rmt_ip_str,STN_DEF_FILE);
				shutdown(link_sock, 1);
				closesocket(link_sock);
				continue;
			}

			if(sProcRec[stn_id].Pid==-1)
			{
				OutputDspInfo(0,"station %d (%s) comm process is not running",stn_id, rmt_ip_str);
				OutputDspInfo(0,"create new station %d (%s) comm process",stn_id, rmt_ip_str);
				int	pid= fork();

				if(pid == -1)
				{
					OutputDspInfo(OUTPUT_ERROR,"Fail to fork");
					closesocket(link_sock);
					continue;
				}
				if(pid == 0) //child process
				{
					char	cmdline[256];
					char	strList[16][MAX_CHAR_NUM_IN_LINE];
					char	*argv[16+1];
					int		str_num;
					int		i;
					
					share_addr->station[stn_id].pipe_fd= socketPair[stn_id][0];
					
					sprintf(cmdline,"%s %d %d 0",sProcRec[stn_id].procname,stn_id,link_sock);
					str_num= GetStringsFromStr(cmdline, 16, strList);
					for(i=0; i<str_num; i++)
						argv[i]= strList[i];
					argv[i]= 0;
					
					if(execv(argv[0], argv)<0)
					{
						OutputDspInfo(OUTPUT_ERROR, "Fail to start child process: errno= %d %s!", errno, cmdline);
						exit(0);
					}					
				}
				else //main process
				{
					OutputDspInfo(OUTPUT_INFO,"Success to fork child process");
					sProcRec[stn_id].Pid = pid;
				}
			}
			else
				OutputDspInfo(0,"station %d (%s) comm process is already running",stn_id, rmt_ip_str);

			if(SendNewSocket(socketPair[stn_id][1], link_sock, stn_id))
			{
				OutputDspInfo(OUTPUT_ERROR,"Fail to duplicate new socket");
				shutdown(link_sock, 1);
				closesocket(link_sock);
				continue;
			}
			else
				OutputDspInfo(0,"success to send new link socket for station %d (%s)",stn_id, rmt_ip_str);
			
			share_addr->station[stn_id].new_link_flag= 1;
			closesocket(link_sock);

		} //for(;;)

		closesocket(listen_sock);

	}//for(;;)
}
int ChildCommProc(int stn_id)
{
	pthread_t	h_thread;
	
	signal(SIGTERM, ChildExitRoutine);
	signal(SIGINT, ChildExitRoutine);
	signal(SIGSEGV, ChildExitRoutine);
	
	share_addr= (IEC104S_SHARE_BUF*)LinktoIEC104ShareMem();
	if(share_addr == (IEC104S_SHARE_BUF*)-1)
	{
		OutputDspInfo(OUTPUT_ERROR, "\nFail to link share memory");
		exit(-1);
	}
	
	local_stn_id= stn_id;
	
	InitOutputFilter();
	
	spdef.number = dpdef.number =stdef.number = 0;
	
	InitConfig(sProcRec[stn_id].NameID); 
	
	cmd_src = sProcRec[stn_id].cmd_src;
	
	
	localinitflag = 1;
	
	
	if(pthread_create(&h_thread, NULL, IEC104_SOE, NULL))
		OutputDspInfo(OUTPUT_ERROR, "Fail to create thread IEC104_SOE: %d",errno);
	else
		OutputDspInfo(0, "Success to create thread IEC104_SOE");
	
	for(;;)
	{
		int	i;
		
		OutputDspInfo(OUTPUT_INFO,  "child wait new link...");
		
		link_sock =GetNewSocket(share_addr->station[stn_id].pipe_fd, stn_id);
		if(link_sock==-1)
		{
			OutputDspInfo(OUTPUT_ERROR,  "Fail to get new socket");
			continue;
		}
		
		OutputDspInfo(OUTPUT_INFO,  "new link arrived");

		for(i=0; i<SOE_NUM; i++)
			soebuf.nr_send[i]= -1;
		soebuf.nr_ack= 0;
		soebuf.sendpnt= soebuf.ackpnt;
		
		Net_Com(stn_id,link_sock);
		
		shutdown(link_sock, 1);
		closesocket(link_sock);
		UpdateChannelFlag(stn_id,(unsigned short)1);
		
	}
}

void main(int argc,char *argv[])
{
	int		stn_id, child_process_flag;
	
	ouput_info_filter= 0;
	
	gethostname(host_name, 32);

	if(-1==MountDB(NULL))
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to MountDB!");
		exit(-1);
	}
	
	if(lan_out_init() == FALSE)
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to lan_out_init()!");
		exit(-1);
	}
	if(-1==InterLockFileLoad())
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to Load InterLock File!");
		exit(-1);
	}
	
	for(;;)
	{
		if( ReadStnDef(STN_DEF_FILE) == true )
			break;

		OutputDspInfo(OUTPUT_ERROR,"###ALARM!!! IEC104::Read Station define file %s Error!",STN_DEF_FILE);
		Sleep(5000);
	}

	
	if ( !CreateMutex("IEC104S_CHILD" ) )
	{
		child_process_flag= 1;
		if(argc != 4)
		{
			OutputDspInfo(OUTPUT_WARN,"iec104子通信进程参数数量错误：%d, 必须为4个", argc);
			exit(-1);
		}
	}
	else
	{	
		child_process_flag= 0;
	}

	if(child_process_flag == 0) //main process
		MainCommProc();
	else
	{
		if(sscanf(argv[1], "%d", &stn_id)!=1)
		{
			OutputDspInfo(OUTPUT_ERROR, "Command Line parameter %s Wrong!\n",argv[1]);
			exit(-1);
		}
		
		if(stn_id<0 || stn_id>=MAX_PROC_NUM)
		{
			OutputDspInfo(OUTPUT_ERROR, "Command Line parameter %s Wrong!\n",argv[1]);
			exit(-1);
		}
		
		ChildCommProc(stn_id);
		
	}	
}

#endif /*SUN_OS*/


#ifdef  WINDOWS_OS /*WINDOWS_OS start*/

void*	CreateIEC104ShareMemory(void)
{
	HANDLE		hMapObject;
	char		name[128];
	void		*shm_addr;
	
	sprintf(name, "SHM_NAME_IEC104S");
	hMapObject= CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0, sizeof(IEC104S_SHARE_BUF), name);
	if( hMapObject == NULL )
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to CreateFileMapping: %s errno=%d\n", name, GetLastError());
		return (void*)-1;
	}
	
	shm_addr = MapViewOfFile (hMapObject,FILE_MAP_ALL_ACCESS,0, 0, 0);  
	if (shm_addr == NULL)
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to MapViewOfFile: %s errno=%d\n", name, GetLastError());
		CloseHandle(hMapObject);
		return (void*)-1;
	}
	return shm_addr;
}
void*	LinktoIEC104ShareMem(void)
{
	HANDLE	hMapObject;
	void	*shm_addr;
	char	shm_name[128];
	
	sprintf(shm_name, "SHM_NAME_IEC104S");
    hMapObject = OpenFileMapping (FILE_MAP_ALL_ACCESS, FALSE, shm_name);
	if (hMapObject == NULL)
	{
		OutputDspInfo(OUTPUT_ERROR, "Fail to OpenFileMapping: %s errno=%d\n", shm_name, GetLastError());
		return (void*)-1;
	}
	shm_addr = MapViewOfFile (hMapObject,FILE_MAP_ALL_ACCESS,0, 0, 0);  
	if (shm_addr == NULL)
	{
		OutputDspInfo(OUTPUT_ERROR, "Fail to MapViewOfFile: %s errno=%d\n", shm_name, GetLastError());
		CloseHandle(hMapObject);
		return (void*)-1;
	}
	CloseHandle(hMapObject);
	return shm_addr;
}

int SendNewSocket(int new_sock, int stn_id)
{
	return WSADuplicateSocket(new_sock, sProcRec[stn_id].Pid, 
		&share_addr->station[stn_id].newProtocolInfo);
}
int GetNewSocket(int stn_id)
{
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, 
		&share_addr->station[stn_id].newProtocolInfo, 0, 0);
}

int	Create_NewIEC104Console(int stn_id, int sock)
{
	STARTUPINFO				sui;
	char	cmdline[256];//, filename[256];
	
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
	
// 	if(!GetModuleFileName(NULL, filename, sizeof(filename)))
// 	{	
// 		OutputDspInfo(OUTPUT_ERROR, "Fail to get process filename: errno=%d", GetLastError());
// 		return(-1);
// 	}
	
	sprintf(cmdline,"%s %d %d 0",sProcRec[stn_id].procname,stn_id, sock);
	if( CreateProcess(NULL,cmdline, NULL, NULL, TRUE,
		CREATE_NEW_CONSOLE, NULL, NULL, &sui, &sProcRec[stn_id].pi)==FALSE )
	{	
		OutputDspInfo(OUTPUT_ERROR, "Fail to create child process %s: stn= %d, errno= %d",sProcRec[stn_id].procname,stn_id,GetLastError());
		return(-1);
	}
	sProcRec[stn_id].Pid= sProcRec[stn_id].pi.dwProcessId;
	return 0; 
}

void WatchChild(void)
{
	int	i,ret,num;
	HANDLE	procHandle[MAX_PROC_NUM];
	int		procStnNo[MAX_PROC_NUM];
	int		stn_no;

	num= 0;
	for(i=0; i<ProcNum; i++)
	{
		if(-1==sProcRec[i].Pid)
			continue;
		procHandle[num]= sProcRec[i].pi.hProcess;
		procStnNo[num]= i;
		num++;
	}		

	ret= WaitForMultipleObjects(num, procHandle, FALSE, 0);
	i= ret-WAIT_OBJECT_0;
	if( i>=0 && i<num )
	{
		stn_no= procStnNo[i];
		OutputDspInfo(OUTPUT_ERROR,"station %d %s is exited! ( pid=%d )",stn_no,sProcRec[stn_no].NameID, sProcRec[stn_no].Pid);
		sProcRec[stn_no].Pid= -1;
		CloseHandle(sProcRec[stn_no].pi.hThread);
		CloseHandle(sProcRec[stn_no].pi.hProcess);
	}
}
void MainExitRoutine ( DWORD signal )
{
	int		i;
	UINT	uExitCode= 0;
	
	shutdown(listen_sock, 2);
	closesocket(listen_sock);
	
	for(i=0;i<ProcNum;i++)
	{
		if ( TerminateProcess ( sProcRec[i].pi.hProcess, uExitCode ) == 0 )
			OutputDspInfo(OUTPUT_ERROR, "ERROR in TerminateProcess %d ( num=%d ), Err=%d %d\n", sProcRec[i].pi.hProcess,i,uExitCode,GetLastError());
		
		if ( TerminateProcess ( sProcRec[i].pi.hThread, uExitCode ) == 0 )
			OutputDspInfo(OUTPUT_ERROR, "ERROR in TerminateProcess (hThread ) %d ( num=%d ), Err=%d %d\n",sProcRec[i].pi.hThread,i,uExitCode,GetLastError());
		
		OutputDspInfo(OUTPUT_ERROR, "now start to terminate %s iec104s_com", sProcRec[i].NameID);
	}
	OutputDspInfo(OUTPUT_ERROR, "iec104s_com 通信主进程 exit Now");
	exit (0 );
}

void ChildExitRoutine ( DWORD signal )
{
	UINT	uExitCode= 0;
	
	UpdateChannelFlag(local_stn_id,(unsigned short)1);
	
	shutdown(link_sock, 1);
	closesocket(link_sock);
	
	OutputDspInfo(OUTPUT_ERROR, "%s iec104s_com exit", sProcRec[local_stn_id].NameID);
	if(dsp_info_file)
		fclose(dsp_info_file);
	exit (0 );
}

void main(int argc,char *argv[])
{
	HANDLE	hThread_SOE;
	int		ThreadID_SOE;
	int		child_process_flag;
	int		stn_id;
	WORD   		wvr;
	WSADATA 	WSAData;
	int 		status;
	unsigned short iec104port;
	SOCKADDR_IN    	clientaddr;
	char			rmt_ip_str[256];
	char			tmp[256];

	ouput_info_filter= 0;
	
	wvr=MAKEWORD(2,2);
	if ((status = WSAStartup(wvr, &WSAData)) != 0) 
	{
		OutputDspInfo(OUTPUT_ERROR,"iec104s_com:: %d is the err\n",status);
		exit(-1);
	}
	
	gethostname(host_name, 32);

	if(-1==MountDB(NULL))
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to MountDB!");
		exit(-1);
	}
	
	if(lan_out_init() == FALSE)
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to lan_out_init()!");
		exit(-1);
	}
	if(-1==InterLockFileLoad())
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to Load InterLock File!\n");
		exit(-1);
	}
	
	for(;;)
	{
		if( ReadStnDef(STN_DEF_FILE) == true )
			break;

		OutputDspInfo(OUTPUT_ERROR,"###ALARM!!! IEC104::Read Station define file %s Error!\n",STN_DEF_FILE);
		Sleep(5000);
	}

	CreateMutex( NULL, FALSE, "IEC104S_CHILD" );
	if ( GetLastError () == ERROR_ALREADY_EXISTS )
	{
		child_process_flag= 1;
		if(argc != 4)
		{
			OutputDspInfo(OUTPUT_WARN,"iec104 通信进程已存在，主进程退出！\n");
			exit(-1);
		}
	}
	else
		child_process_flag= 0;

	if(child_process_flag == 0) //main process
	{
		OutputDspInfo(0,"iec104 通信主进程启动！\n");
		if ( SetConsoleCtrlHandler ((void *)MainExitRoutine,TRUE ) == 0 )
		{	
			OutputDspInfo(OUTPUT_ERROR,"SetConsoleCtrlHandler ERROR %d!!! \n",GetLastError());
		}

		share_addr= (IEC104S_SHARE_BUF*)CreateIEC104ShareMemory();
		if(share_addr == (IEC104S_SHARE_BUF*)-1)
		{
			OutputDspInfo(OUTPUT_ERROR,"Fail to create share memory");
			exit(-1);
		}

		for(;;)
		{
			int		time_on;

			CheckHostState();
			if(!local_host_normal_state)
			{
				OutputDspInfo(OUTPUT_WARN, "local host state abnormal, stop work");
				Sleep(5000);
				continue;
			}


			listen_sock = socket(AF_INET, SOCK_STREAM, 0);
                
			if (listen_sock == INVALID_SOCKET)
			{
				OutputDspInfo(OUTPUT_ERROR,"creat socket() failed!\n");
				Sleep(5000);
				continue;
			}

			time_on=1;
			if(setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,(char *)&time_on,sizeof (time_on)))
			{
				OutputDspInfo(OUTPUT_ERROR, "ERROR %d setting SO_REUSEPORT socket option", errno);
				Sleep(5000);
				continue;
			}

			iec104port = (unsigned short)IEC104_PORT;
			memset((char *)&clientaddr,0,sizeof(struct sockaddr_in));
			clientaddr.sin_family      = AF_INET;
			clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
			clientaddr.sin_port        = htons(iec104port);  

			while(bind(listen_sock,(struct sockaddr FAR *)&clientaddr,sizeof(clientaddr))==SOCKET_ERROR)
			{
				OutputDspInfo(OUTPUT_ERROR,"iec104s_com:: bind() error: %d!\n", errno);
				closesocket(listen_sock);
				Sleep(5000);
				continue;
			}

			if(listen(listen_sock,4)<0)
			{
				OutputDspInfo(OUTPUT_ERROR,"iec104s_com::listen() error: %d!\n", errno);
				closesocket(listen_sock);
				Sleep(5000);
				continue;
			}

			for(;;)
			{
				int		clientaddrlen;
				int		new_sock;


				OutputDspInfo(OUTPUT_ERROR,"main waiting for connection...\n");
				
				clientaddrlen=sizeof(clientaddr);
				new_sock=accept(listen_sock,(struct sockaddr *) &clientaddr,(socklen_t *)&clientaddrlen);
				
				if(new_sock<0)
				{
					OutputDspInfo(OUTPUT_ERROR,"accept() return %d wrong!\n", errno);
					continue;
				}
				link_sock= new_sock;
				
				CheckHostState();
				if(!local_host_normal_state)
				{
					OutputDspInfo(OUTPUT_WARN, "local host state abnormal, stop work");
					closesocket(link_sock);
					break;
				}

				strcpy(rmt_ip_str,inet_ntoa(clientaddr.sin_addr));
				OutputDspInfo(OUTPUT_WARN,"new connection! ip=%s socket=%d\n",rmt_ip_str,link_sock);
				
				for(stn_id = 0; stn_id < ProcNum; stn_id++)
				{
					if(strcmp(rmt_ip_str,sProcRec[stn_id].Ip) == 0)
						break;
				}
				if(stn_id >= ProcNum)
				{
					OutputDspInfo(OUTPUT_ERROR,"remote ip=%s not find in file %s! close socket!\n",rmt_ip_str,STN_DEF_FILE);
					shutdown(link_sock, 1);
					closesocket(link_sock);
					continue;
				}

				WatchChild();	
				if(-1==sProcRec[stn_id].Pid)
				{
					OutputDspInfo(0,"station %d (%s) comm process is not running",stn_id, rmt_ip_str);
					OutputDspInfo(0,"create new station %d (%s) comm process",stn_id, rmt_ip_str);
					Create_NewIEC104Console(stn_id, link_sock);
				}
				else
				{
					OutputDspInfo(0,"station %d (%s) comm process is already running",stn_id, rmt_ip_str);
/************add by zyf kill process when receive new connect***************/
/*
					OutputDspInfo(0,"***************");
					OutputDspInfo(0,"station %d (%s) comm process is already running,close ols connect",stn_id, rmt_ip_str);
					//Sleep(1000);
					TerminateProcess(sProcRec[stn_id].pi.hProcess,0); 
					//Sleep(1000);
					OutputDspInfo(0,"create new station %d (%s) comm process",stn_id, rmt_ip_str);
					Create_NewIEC104Console(stn_id, link_sock);
*/
/************end by zyf kill process when receive new connect***************/
				}

				if(SendNewSocket(link_sock, stn_id))
				{
					OutputDspInfo(OUTPUT_ERROR,"Fail to duplicate new socket");
					shutdown(link_sock, 1);
					closesocket(link_sock);
					continue;
				}
				
				share_addr->station[stn_id].new_link_flag= 1;
				closesocket(link_sock);

			} //for(;;)

			closesocket(listen_sock);

		}//for(;;)
	} //if(child_process_flag == 0)
	
	else //child process
	{
		if ( SetConsoleCtrlHandler ((void *)ChildExitRoutine,TRUE ) == 0 )
		{	
			OutputDspInfo(OUTPUT_ERROR, "SetConsoleCtrlHandler ERROR %d!!! \n",GetLastError());
		}

		if(sscanf(argv[1], "%d", &stn_id)!=1)
		{
			OutputDspInfo(OUTPUT_ERROR, "Command Line parameter %s Wrong!\n",argv[1]);
			exit(-1);
		}

		if(stn_id<0 || stn_id>=MAX_PROC_NUM)
		{
			OutputDspInfo(OUTPUT_ERROR, "Command Line parameter %s Wrong!\n",argv[1]);
			exit(-1);
		}

		share_addr= (IEC104S_SHARE_BUF*)LinktoIEC104ShareMem();
		if(share_addr == (IEC104S_SHARE_BUF*)-1)
		{
			OutputDspInfo(OUTPUT_ERROR, "Fail to link share memory");
			exit(-1);
		}
		
		local_stn_id= stn_id;

		InitOutputFilter();

		sprintf(tmp,"H9000通信系统: %s iec104协议(iec104s_com) IP：%s",sProcRec[stn_id].NameID,sProcRec[stn_id].Ip);
		SetConsoleTitle ( tmp );

		spdef.number = dpdef.number =stdef.number = 0;

		InitConfig(sProcRec[stn_id].NameID); 

		cmd_src = sProcRec[stn_id].cmd_src;


		localinitflag = 1;

	
		hThread_SOE = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)IEC104_SOE,
									 NULL, CREATE_SUSPENDED, &ThreadID_SOE);
		if (hThread_SOE == NULL)
			OutputDspInfo(OUTPUT_ERROR, "Error in CreateThread ThreadID_SOE:%d\n",GetLastError());
		else
			OutputDspInfo(OUTPUT_ERROR, "hThread_SOE: %d OK!\n",hThread_SOE);

		SetThreadPriority(hThread_SOE,THREAD_PRIORITY_BELOW_NORMAL);	 

		ResumeThread( hThread_SOE);	

		for(;;)
		{
			int i;
			OutputDspInfo(OUTPUT_INFO,  "child wait new link...");
			if(!share_addr->station[stn_id].new_link_flag)
			{
				Sleep(200);
				continue;
			}
 
			OutputDspInfo(OUTPUT_INFO,  "new link arrived");
			share_addr->station[stn_id].new_link_flag= 0;
			
			link_sock =GetNewSocket(stn_id);
			if(link_sock==-1)
			{
				OutputDspInfo(OUTPUT_ERROR,  "Fail to get new socket");
				continue;
			}

			for(i=0; i<SOE_NUM; i++)
				soebuf.nr_send[i]= -1;
			soebuf.nr_ack= 0;

			soebuf.sendpnt= soebuf.ackpnt;

			Net_Com(stn_id,link_sock);

			shutdown(link_sock, 1);
			closesocket(link_sock);
			UpdateChannelFlag(stn_id,(unsigned short)1);

		}
	} //child process
}

#endif /*WINDOWS_OS*/


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
#ifdef WINDOWS_OS	
	timeout = SEND_TIMEOUT_VALUE;
	if(setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout)))
	{
		OutputDspInfo(OUTPUT_ERROR, "ERROR %d setting SO_SNDTIMEO socket option", errno);
		return -1;
	}
	
	timeout = RECV_TIMEOUT_VALUE;
	if(setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout)))
	{
		OutputDspInfo(OUTPUT_ERROR, "ERROR %d setting SO_RCVTIMEO socket option", errno);
		return -1;
	}
#endif

#ifdef TCP_NODELAY
	if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (const char *) &sockopt_on, sizeof (int) ) )
	{
		OutputDspInfo(OUTPUT_ERROR, "ERROR %d setting NODELAY socket option", errno);
		return -1;
	}
#endif
	return 0;
}

void ResetTimeout(time_t *test_tim)
{
	unsigned short no;
	time_t	tim_now= time(NULL);

	//T1 timeout
	if(nr_ack%2 || nr_send%2)
		OutputDspInfo(OUTPUT_ERROR, "ack_no:%d, send_no: %d error", nr_ack, nr_send);
	else
	{
		for(no=nr_ack; no!=nr_send; no+=2)
			tim_waitack[no]= tim_now;
	}

	//recv frame timeout
	no_recv_tim= tim_now;

	//test frame timeout
	*test_tim= tim_now;
}


int CheckT1Timeout(time_t tim_now)
{
	unsigned short no;

	if(nr_ack%2 || nr_send%2)
	{
		OutputDspInfo(OUTPUT_ERROR, "ack_no:%d, send_no: %d error", nr_ack, nr_send);
		
		return TRUE;
	}
	for(no=nr_ack; no!=nr_send; no+=2)
	{
		if((tim_now - tim_waitack[no])>t1)
			return TRUE;
	}
	return FALSE;
}

int CheckRecvTimeout(time_t tim_now)
{
	if((tim_now-no_recv_tim)>total_timeout)
		return TRUE;
	return FALSE;
}

int CheckWaitAckK(void)
{
	int	wait_ack_num;

	if(nr_send >= nr_ack)
		wait_ack_num = nr_send - nr_ack;
	else
		wait_ack_num = 0x10000 + nr_send - nr_ack;
	if(wait_ack_num >= K*2)
		return TRUE;
	else
		return FALSE;
}


int CheckUnAckW(void)
{
	int	un_ack_num;
	
	if(ns_ack > nr_recv)
		un_ack_num = 0x10000 + nr_recv - ns_ack;
	else
		un_ack_num = nr_recv - ns_ack;
	if(un_ack_num >= W*2)
		return TRUE;
	else
		return FALSE;
}


int GetNackSoeTimePtId(POINTER *point)
{
	char	name_buf[128];
	if(0!=GetHostStnId(host_name, &point->stn_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostStnId() %s No this hostname point in database file!\n",host_name);
		return -1;
	}
	
	if(0!=GetHostSysLcuId(host_name, &point->dev_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostSysLcuId() %s No this hostname point in database file!\n",host_name);
		return -1;
	}
	
	point->data_type= ANA_TYPE;
	
	strcpy(name_buf,sProcRec[local_stn_id].NameID);
	strcat(name_buf,"_SOETIME");
	if(-1==GetEntryIdByName(name_buf, point))
	{
		OutputDspInfo(OUTPUT_ERROR, "%s %s %s No this com channel status point in stn:%d dev:%d the database file!\n",
			sProcRec[local_stn_id].NameID,sProcRec[local_stn_id].Ip,name_buf,point->stn_id,point->dev_id);
		return -1;
	}
	
	return 0;
}


void UpdateSoeAck(void)
{
	unsigned short soepnt;
	POINTER		point;
	DATE_TIME	ack_time;
	int			soe_ack_flag=0;

	soepnt= soebuf.ackpnt;
	while(soepnt!=soebuf.sendpnt)
	{
		if(soebuf.nr_send[soepnt]==-1)
			memset(&soebuf.soerec[soepnt], 0, sizeof(SOEREC));

		else
		{
			if(soebuf.nr_ack<nr_ack)
			{
				if(soebuf.nr_send[soepnt]>=soebuf.nr_ack && soebuf.nr_send[soepnt]<nr_ack)
				{
					soe_ack_flag= 1;
					soebuf.count--;
					ack_time= soebuf.soerec[soepnt].time;
					soebuf.nr_send[soepnt]= -1;
				}
				else
					break;
			}
			else
			{
				if(soebuf.nr_send[soepnt]>=soebuf.nr_ack || soebuf.nr_send[soepnt]<nr_ack)
				{
					soe_ack_flag= 1;
					soebuf.count--;
					ack_time= soebuf.soerec[soepnt].time;
					soebuf.nr_send[soepnt]= -1;
				}
				else
					break;
			}
		}

		soepnt++;
		if(soepnt>=SOE_NUM)
			soepnt= 0;
	}
	soebuf.ackpnt= soepnt;
	soebuf.nr_ack= nr_ack;

	OutputDspInfo(0, "UpdateSoeAck: head=%d send=%d ack=%d count=%d", 
		soebuf.bufpnt, soebuf.sendpnt, soebuf.ackpnt, soebuf.count);

	if(soe_ack_flag && !GetNackSoeTimePtId(&point))  //modified by wzg 20120305
	{
		ANLG_STATUS	status;

		memset(&status, 0, sizeof(status));
		BroadCastAnaPt1(point, 0, (float)ack_time.ms, status, &ack_time);
	}
}

void ResetSoeSendPtr(void)
{
	POINTER		point;
	USHORT		bufpnt;
	DATE_TIME	date_time;
	time_t		nack_time_sec;
	int			nack_time_ms;
	float		fValue;
	unsigned short count;
	
	OutputDspInfo(0, "Reset soe send ptr because received STARTDT");
	OutputDspInfo(0, "ResetSoeSendPtr begin: head=%d send=%d ack=%d count=%d", 
		soebuf.bufpnt, soebuf.sendpnt, soebuf.ackpnt, soebuf.count);

	if(soebuf.count<=0)
	{
		soebuf.ackpnt= soebuf.sendpnt= soebuf.bufpnt;	
		soebuf.count= 0;
		return;
	}

	if(GetNackSoeTimePtId(&point))
	{
		soebuf.ackpnt= soebuf.sendpnt= soebuf.bufpnt;	
		soebuf.count= 0;
		return;
	}

	GetPtUpdateTimeById(&point, &date_time);
	nack_time_sec= GetAbsTimeFromDateTime(&date_time);
	if(nack_time_sec==-1)
	{
		soebuf.ackpnt= soebuf.sendpnt= soebuf.bufpnt;	
		soebuf.count= 0;
		return;
	}

	GetAnaPtfValueById(&point, &fValue);
	nack_time_ms= (int)fValue;

	count= soebuf.count;	
	bufpnt= (soebuf.bufpnt+SOE_NUM-soebuf.count)%SOE_NUM; 

	while(count>0)
	{
		int soe_ms= soebuf.soerec[bufpnt].time.ms;
		time_t	soe_time;
		
		soe_time= GetAbsTimeFromDateTime(&soebuf.soerec[bufpnt].time);
		if(soe_time==-1)
			break;

		if(soe_time > nack_time_sec) //SOE时间大于确认SOE时间
			break;
		if((soe_time == nack_time_sec) && (soe_ms >= nack_time_ms)) //SOE时间大于或等于确认SOE时间
			break;
		
		count--;
		bufpnt= (bufpnt+1)%SOE_NUM;			
	}

	soebuf.ackpnt= soebuf.sendpnt= bufpnt;	
	soebuf.count= (soebuf.bufpnt+SOE_NUM-soebuf.sendpnt)%SOE_NUM;

	OutputDspInfo(0, "ResetSoeSendPtr end: head=%d send=%d ack=%d count=%d", 
		soebuf.bufpnt, soebuf.sendpnt, soebuf.ackpnt, soebuf.count);
}


int Net_Com(int stn_id,int sockfd)
{
	unsigned int	sendbufheadpt,recvbufheadpt;
	unsigned int	sendbufptr1,sendbufptr2;
	extern	unsigned short	m_cRespGenInterroGrpNum,m_cRespCountGrpNum,m_cCyclicGrpNum,m_cScanGrpNum;
	extern	unsigned short	m_cRespParamNum,m_cRespICGrpNum,m_cRespCIGrpNum;

	int	byte_recvd,pkt_len,count;
	int	test_act_state;
	int	ret;
	
	fd_set	fset;
	struct timeval	tout;

	time_t	tim,test_tim;
/*********add by zyf for t2**********/
	time_t	t2_tim;
	int		t2_flag;
/*********end by zyf for t2**********/
/*********add by zyf for t1 U**********/
	time_t	t1_tim_U;
/*********end by zyf for t1 U**********/
	struct tm *timeptr;

	unsigned short peer_send_no;


#ifdef REC_YF_AGC_CMD
	tim=time(&tim);
	timeptr=localtime(&tim);
	daysaved = timeptr->tm_mday;
#endif

	OutputDspInfo(OUTPUT_ERROR, "Thread Net_Com() start!\n");
	
	
	if(SetSocket(sockfd))
		return false;
	
	tim=time(&tim);
	no_recv_tim = tim;
	test_tim= tim;
/*********add by zyf for t2**********/
	t2_tim= tim;
	t2_flag= 0;
/*********end by zyf for t2**********/
/*********add by zyf for t1 U**********/
	t1_tim_U= tim;
/*********end by zyf for t1 U**********/
	InitFlag();

	sendbufheadpt = 0;
	sendbufptr1 = 0;
	sendbufptr2 = 0;
	DT_Enable_Flag = 0;
	test_act_state = 0;
	nr_recv = nr_send = nr_ack = ns_ack= 0;
	m_cRespGenInterroGrpNum=m_cRespCountGrpNum=m_cCyclicGrpNum=m_cScanGrpNum=0;


	for(;;)
	{
		CheckHostState();
		if(!local_host_normal_state)
		{
			if(ns_ack != nr_recv)
				SFormatFR(sockfd);
			
			Sleep(5000);

			CheckHostState();
			if(!local_host_normal_state)
			{
				OutputDspInfo(0, "host state abnormal, stop work, close connection\n");
				return 0;
			}
		}

		if(ClockSyncflag==1)
		{
			ClockSyncflag= 0;
			ResetTimeout(&test_tim);
		}
		
		memset(recvbuf, 0, VARFRAMELEN);
		FD_ZERO(&fset);
		FD_SET(sockfd,&fset);
		tout.tv_sec = 0;
		tout.tv_usec = 500*1000;
		ret = select(sockfd+1, &fset, NULL, NULL, &tout);
		tim=time(&tim);

		if((cmdbuf.selected == 1) && ((tim-cmdbuf.sel_tim)>cmd_period))
		{
			cmdbuf.selected= 0;
			OutputDspInfo(OUTPUT_WARN, "Wait execute command timeout, selected command cancel\n");
		}

		if(ret <= 0)
		{
			if(CheckRecvTimeout(tim))
			{
				OutputDspInfo(OUTPUT_ERROR, "recv frame timeout, close connection\n");
				return false;
			}
/*********add by zyf for t2**********/
/*
			if((t2_flag == 1)&&((tim - t2_tim)>t2))
			{
				OutputDspInfo(OUTPUT_WARN, "t2 timeout, send SFormatFR \n");	
				if(!SFormatFR(sockfd))
					return(false);
				t2_flag = 0;
				t2_tim = tim;
			}
*/
/*********end by zyf for t2**********/
			if((test_act_state == 0)&&((tim - test_tim)>t3))
			{
				OutputDspInfo(OUTPUT_WARN, "t3 timeout, send TestFR \n");				
				if(!TestFR(sockfd))
					return(false);
/*********add by zyf for t1 U**********/
				t1_tim_U= tim;
/*********end by zyf for t1 U**********/
				test_act_state = 1;
				test_tim = tim;
			}
/*********change by zyf for t1 U**********/
			//if((test_act_state == 1) && (tim - test_tim)>t1)
			if((test_act_state == 1) && (tim - t1_tim_U)>t1)
/*********end change by zyf for t1 U**********/
			{
				OutputDspInfo(OUTPUT_ERROR, "%s %s Waiting MASTER Test ACK Frame timeout, close connection\n",
					sProcRec[stn_id].NameID,sProcRec[stn_id].Ip);
				return(false);
			}

			if(DT_Enable_Flag)
			{
				if(CheckWaitAckK())
				{
					OutputDspInfo(OUTPUT_WARN, "Waiting MASTER %s %s ACK! nr_send=%d, nr_ack=%d\n",
						sProcRec[stn_id].NameID,sProcRec[stn_id].Ip,nr_send/2, nr_ack/2);
				}
				else
				{
					ret = RespondUserDataClass1(sockfd);
					if(ret == false)
						return(false);
				}

			}
		}
		else //recved frame
		{
			byte_recvd=recv(sockfd,(char *)recvbuf,(int)MINFRAMELEN,(int)0);
			if(byte_recvd <= 0)
			{
				OutputDspInfo(OUTPUT_ERROR, "%s %s recv() Error %d! close connection\n",
					sProcRec[stn_id].NameID,sProcRec[stn_id].Ip,errno);
				
				return(false);
			}
			no_recv_tim = tim;
			test_tim = tim;             

			if(byte_recvd!=MINFRAMELEN)
			{
				OutputDspInfo(OUTPUT_ERROR, "Byte number received=%d error\n",byte_recvd);
				
				continue;
			}

			UpdateChannelFlag(stn_id,(unsigned char)0);
			if((recvbuf[0]==0x68) && (recvbuf[1]==4) && ((recvbuf[2] & 0x3) == 3))	/* U Frame */
			{
				PrintRecvData(byte_recvd);
				
				if(recvbuf[2] == 0x07)	/* START DT */
				{
					OutputDspInfo(OUTPUT_INFO, "\n%s %s received U Frame START DT",
						sProcRec[stn_id].NameID,sProcRec[stn_id].Ip);
					
					DT_Enable_Flag = 1;
					if(!RespondStartDT(sockfd))
						return(false);
					
					ResetOrgData();
					
					if(localinitflag)
					{
						ret = EndofInitialization(sockfd);
						if(ret == false)
							return(false);
						localinitflag = 0;
						continue;
					}
					else
						ResetSoeSendPtr();
					
				}
				else if(recvbuf[2] == 0x13)	/* STOP DT */
				{
					OutputDspInfo(OUTPUT_INFO, "\n%s %s received U Frame STOP DT",
						sProcRec[stn_id].NameID,sProcRec[stn_id].Ip);
					DT_Enable_Flag = 0;
					if(!RespondStopDT(sockfd))
						return(false);
				}
				else if(recvbuf[2] == 0x43)	/* TEST FR */
				{
					OutputDspInfo(OUTPUT_INFO, "\n%s %s received U Frame TEST FR",
						sProcRec[stn_id].NameID,sProcRec[stn_id].Ip);
					if(!RespondTestFR(sockfd))
						return(false);
/*********move by zyf for t1 U**********/
					//test_act_state = 0;
/*********end move by zyf for t1 U**********/
				}
				else if(recvbuf[2] == 0x83)	/*ACK TEST FR	*/
				{
/*********add by zyf for t1 U**********/
						t1_tim_U= tim;
/*********end by zyf for t1 U**********/
					OutputDspInfo(OUTPUT_INFO, "\n%s %s received U Frame ACK TEST FR",
						sProcRec[stn_id].NameID,sProcRec[stn_id].Ip);
					test_act_state = 0;
				}
			}
			else if((recvbuf[0]==0x68) && (recvbuf[1]==4) && ((recvbuf[2] & 0x3) == 1))	/* S Frame */
			{
				OutputDspInfo(OUTPUT_INFO, "\n%s %s received S Frame",
					sProcRec[stn_id].NameID,sProcRec[stn_id].Ip);

				PrintRecvData(byte_recvd);
				
				if(!DT_Enable_Flag)
				{
					OutputDspInfo(OUTPUT_WARN, "%s %s received S frame but DT is stopped; close connection\n",
						sProcRec[stn_id].NameID,sProcRec[stn_id].Ip,pkt_len);
					
					return(false);
				}

				memcpy(&nr_ack,&recvbuf[4],2);
				Swap2Byte(&nr_ack);

				UpdateSoeAck();

				if(CheckWaitAckK())
				{
					OutputDspInfo(OUTPUT_INFO, "Waiting MASTER %s %s ACK! nr_send=%d, nr_ack=%d\n",
						sProcRec[stn_id].NameID,sProcRec[stn_id].Ip,nr_send/2, nr_ack/2);
					
					continue;
				}
				
				ret = RespondUserDataClass1(sockfd);
				if(ret == false)
				{
					return(false);
				}
			}
			else if((recvbuf[0]==0x68) && ((recvbuf[2] & 0x1) == 0))	/* I Frame */
			{
				if(!DT_Enable_Flag)
				{
					OutputDspInfo(OUTPUT_INFO, "%s %s received I frame but DT is stopped;close connection\n",
						sProcRec[stn_id].NameID,sProcRec[stn_id].Ip,pkt_len);
					
					return(false);
				}

				pkt_len = recvbuf[1]+2;
				if(pkt_len <=6)
				{
					OutputDspInfo(OUTPUT_INFO, "%s %s Received length %d <= 6 wrong;close connection\n",
						sProcRec[stn_id].NameID,sProcRec[stn_id].Ip,pkt_len);
					
					return(false);
				}
				count = 6;
				while (count < pkt_len)
				{					/* keep receiving until "pkt_len" bytes are received */
					FD_ZERO(&fset);
					FD_SET(sockfd,&fset);
					tout.tv_sec = 1;
					tout.tv_usec = 0;
					ret = select(sockfd+1, &fset, NULL, NULL, &tout);
					if(ret <= 0)
					{
						OutputDspInfo(OUTPUT_WARN, "%s %s select() 2 TIMEOUT! ret=%d\n",
							sProcRec[stn_id].NameID,sProcRec[stn_id].Ip,ret);
						
						return(false);
					}
					byte_recvd = recv (sockfd, (char *)recvbuf+count, pkt_len-count, 0);

					if (byte_recvd <= 0)		/* 0 if link is disconnected, negative if error.*/
					{
						OutputDspInfo(OUTPUT_ERROR, "%s %s recv() 2 Error %d! close connection\n",
							sProcRec[stn_id].NameID,sProcRec[stn_id].Ip,errno);
						
						return(false);
					}
					else
						count += byte_recvd;	/* add the bytes read to "count" */
				}	/* end "while" loop */
/*********add by zyf for t2**********/
/*
				t2_flag = 1;
				t2_tim = tim;
*/
/*********end by zyf for t2**********/
				byte_recvd = pkt_len;
				OutputDspInfo(OUTPUT_INFO, "\n%s %s received I Frame",sProcRec[stn_id].NameID,sProcRec[stn_id].Ip);
				PrintRecvData(byte_recvd);

				memcpy(&peer_send_no,&recvbuf[2],2);
				memcpy(&nr_ack,&recvbuf[4],2);
				Swap2Byte(&peer_send_no);
				Swap2Byte(&nr_ack);

				UpdateSoeAck();

				if(peer_send_no != nr_recv)
				{
					OutputDspInfo(OUTPUT_ERROR, "%s %s Received Nr %d not equiv V(R) %d; close connection\n",
						sProcRec[stn_id].NameID,sProcRec[stn_id].Ip,peer_send_no/2,nr_recv/2);
					
					return(false);
				}
				nr_recv +=2;

				if(CheckUnAckW())
				{
					if(!SFormatFR(sockfd))
					{
						return(false);
					}
				}

				recvbufheadpt = recv_data.pnt;
				memset(recv_data.data[recvbufheadpt],0,CMD_DATA_SIZE);
				memcpy(recv_data.data[recvbufheadpt],&recvbuf[6],pkt_len - 6);
				recv_data.length[recvbufheadpt] =(unsigned short) (pkt_len - 6);
				
				ret = IEC870_5_Recv(stn_id,recvbufheadpt,sockfd);
				if(ret == false)
				{
					return(false);
				}

				recvbufheadpt ++;
				if(recvbufheadpt >= CMD_ENTRY_NUMBER)
					recvbufheadpt = 0;
				recv_data.pnt = recvbufheadpt;
			}
			else
			{
				OutputDspInfo(OUTPUT_ERROR, "%s %s Wrong %02X type id %d frame!\n",
					sProcRec[stn_id].NameID,sProcRec[stn_id].Ip,recvbuf[2],recvbuf[6]);
			}

		} //recv frame

		if(ResetProcessflag)
		{
			OutputDspInfo(OUTPUT_ERROR, "process will to restart because recved C_RP_NA command!\n");
			
			if(ns_ack != nr_recv)
				SFormatFR(sockfd);
			
			ChildExitRoutine(0);
		}
		
		if(CheckT1Timeout(tim))
		{
			OutputDspInfo(OUTPUT_WARN, "wait ack T1 timeout, close connection\n");
			
			if(ns_ack != nr_recv)
				SFormatFR(sockfd);
			return(false);
		}
		
	}/* for(;;) */
}

int UpdateChannelFlag(int stn_id, unsigned short status)
{
	char	name_buf[128];
	char	host_name[36];
	POINTER		point;
	USHORT	old_state;

	gethostname(host_name, 32);
	if(0!=GetHostStnId(host_name, &point.stn_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostStnId() %s No this hostname point in database file!\n",host_name);
		return -1;
	}

	if(0!=GetHostSysLcuId(host_name, &point.dev_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostSysLcuId() %s No this hostname point in database file!\n",host_name);
		return -1;
	}

	point.data_type= SYSMSG_TYPE;

	strcpy(name_buf,host_name);
	strcat(name_buf,"_");
	strcat(name_buf,sProcRec[stn_id].NameID);
	if(-1==GetEntryIdByName(name_buf, &point))
	{
		OutputDspInfo(OUTPUT_ERROR, "%s %s %s No this com channel status point in stn:%d dev:%d the database file!\n",
			sProcRec[stn_id].NameID,sProcRec[stn_id].Ip,name_buf,point.stn_id,point.dev_id);
		return -1;
	}

	GetIndPtRtStateById(&point, &old_state);
	if(old_state != status)
	{
		WR_VAR_UNION	new_value;
		IOST		iost={0};
		DATE_TIME	update_time;

		BroadCastIndPtState(point, 0, status);	
		new_value.ind.rt_state= status;
		GetSysTime(&update_time);
		SetPtValueIostById(&point, new_value, iost, update_time, RW_UPDATE_TIME | RW_RT_VALUE);
	}
	return 0;
}





void OutputDspInfo(int nFilter,	// how to handle and where to report
				   // 0: default - normal flag
				   // else flags
				   const char* format,// from here the function uses printf syntax
				   ...
				   )
{
	int		bufptr,len,charlen;
    va_list ap;
    char	buf[1024], *ptr= buf;
	char	timeStr[128];
	
	if(nFilter<ouput_info_filter)
		return;

	GetSysShortTimeStr1(timeStr);
	
	ptr += sprintf(buf, "\n%s: ", timeStr);
	
    va_start(ap, format);
    ptr += vsprintf(ptr, format, ap);
	va_end(ap);
	*ptr= 0;
	
	printf("%s", buf);
	
	if(dsp_info_file)
	{
		fprintf(dsp_info_file, "%s", buf);
		fflush(dsp_info_file);
	}

	if ((long *)dsp_buf_addr==(long *)-1)
		return;

	bufptr = dsp_buf_addr->header_ptr;
	len = strlen(buf);
	charlen = DSP_INFO_SIZE - bufptr;
	if(charlen < len)
	{
		memcpy(&dsp_buf_addr->buf[bufptr],buf,charlen);
		bufptr = len - charlen;
		memcpy(&dsp_buf_addr->buf[0],&buf[charlen],bufptr);
	dsp_buf_addr->header_ptr = bufptr;
	}
	else
	{
		memcpy(&dsp_buf_addr->buf[bufptr],buf,len);
		charlen  = bufptr + len;
		if(charlen >= DSP_INFO_SIZE)
			dsp_buf_addr->header_ptr = 0;
		else
			dsp_buf_addr->header_ptr = charlen;
	}
}



  

