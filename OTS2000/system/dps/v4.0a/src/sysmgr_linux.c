#include 	<signal.h>

#include 	"../../../dms/v4.0a/inc/dms_com.h"
#include 	"../inc/dps_rec_def.h"
#include 	"../inc/dps_type_def.h"
#include 	"../inc/ipc_common.h"
#include 	"../inc/dps_functions_def.h"


#ifdef		SUN_OS
#include 	<sys/ipc.h>
#include 	<sys/sem.h>
#include 	<sys/shm.h>
#include 	<sys/socket.h>  /* for AF_INET        */
#include 	<netinet/in.h>  /* for struct in_addr */
#include 	<arpa/inet.h>   /* for inet_ntoa()    */
#include 	<sys/types.h>
#include 	<sys/ioctl.h>
#include	<fcntl.h>
#include 	<netdb.h>       /* for struct hostent */
#include	<errno.h>
#include	<wait.h>
#endif



#ifdef	WINDOWS_OS
#define		SIGQUIT					SIGBREAK
#define 	bzero(addr, size)		memset(addr, 0, size)
#define		sleep(sec)				Sleep(sec*1000)
#define		kill(run_pid, SIGNAL)	TerminateProcess(run_pid, 0)
#define		errno					WSAGetLastError()
#endif



typedef struct 
{
	int		config, db, calcu, interlock, event;
} FILE_COPY_SELECT;



static	void ExitEmsSystem( int signo );
void*	MakeSysServeSock(void *para);
int		RestoreEmsRunFiles(FILE_COPY_SELECT	*file_copy_sel);
void	SaveSysStopTime(void);	
int		CreateSysRunTasks(void);
pid_t	StartTaskProcess(char *process_name);
void	ClearOldEmsTasks(void);
int		CreateSockNoBlock(void);




#define	SYS_RUN_SEM				"sys_run_sem"

int		proc_num;
pid_t	run_pid[MAX_PROCESS_NUM+1];
int		run_serve_sock= -1;
char	local_hostname[MAX_HOST_NAME_SIZE];

int		stop_system_flag=0;





#ifdef	WINDOWS_OS
BOOL IsTaskRunning(char *task)
{
	char	cmd_str[256], task_name[256], cmd_ret_str[1024];
	
	sprintf(cmd_str, "tasklist /nh /fi \"imagename eq %s.exe\"", task);
	if(-1==NoConsoleSystemCmd(cmd_str, cmd_ret_str, sizeof(cmd_ret_str)))
		return FALSE;

	sprintf(task_name, "%s.exe", task);
	if(strstr(cmd_ret_str, task_name))
		return TRUE;
	else
		return FALSE;
}

#else /*SUN_OS*/
BOOL IsTaskRunning(char *task)
{
	char	cmd_str[256], filename[256], task_name[256];	
	char	err_str[256];
	char	buff_new[256];
	FILE	*fp;
	BOOL	ret;
	
	sprintf(filename, "%ssysmgr_tmp", SYS_TMP_PATH);
	sprintf(cmd_str, "ps -ef > %s", filename);
	sprintf(task_name, "%s.dxe", task);
	system(cmd_str);
	if(NULL==(fp=fopen(filename, "r")))
	{
		sprintf(err_str, "Fail to open %s", filename);
		Err_log(err_str);
		return FALSE;
	}
	ret= FALSE;
	while(fgets(buff_new, sizeof(buff_new), fp)!=NULL)
	{
		if(strstr(buff_new, task_name))
		{
			ret= TRUE;
			break;
		}
	}
	fclose(fp);
	unlink(filename);
	return ret;
}
#endif




int	CreateRunPath(char *path)
{
	char	err_str[256];
	char	tmp[256];

	strcpy(tmp, path);
	tmp[strlen(tmp)-1]= 0;
	if(-1==access(tmp, 0))
	{
#ifdef	WINDOWS_OS
		if(-1==mkdir(tmp))
#else
		if(-1==mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
#endif
		{
			sprintf(err_str, "Fail to create path %s", tmp);
			Err_log(err_str);
			return -1;
		}
	}
	return 0;
}	
		

int	CheckSysRunPaths(void)
{
	if(-1==CreateRunPath(SYS_DATA_PATH))
		return -1;
	if(-1==CreateRunPath(SYS_RUN_PATH))
		return -1;
	if(-1==CreateRunPath(SYS_DEFINE_PATH))
		return -1;
	if(-1==CreateRunPath(DATABASE_DEFINE_PATH))
		return -1;
	if(-1==CreateRunPath(ERROR_LOG_PATH))
		return -1;
	if(-1==CreateRunPath(EVENT_LOG_PATH))
		return -1;
	if(-1==CreateRunPath(EVENT_TEXT_LOG_PATH))
		return -1;
	if(-1==CreateRunPath(SYS_TMP_PATH))
		return -1;
	if(-1==CreateRunPath(OIX_DSP_PATH))
		return -1;
	if(-1==CreateRunPath(OIX_PIC_PATH))
		return -1;
	if(-1==CreateRunPath(AGC_LOG_PATH))
		return -1;
	if(-1==CreateRunPath(AUD_FILE_PATH))
		return -1;
	if(-1==CreateRunPath(SGZY_LOG_PATH))
		return -1;
	if(-1==CreateRunPath(CURVE_LOG_PATH))
		return -1;
	if(-1==CreateRunPath(SFTCP_LOG_PATH))
		return -1;
	if(-1==CreateRunPath(RPT_DATA_PATH))
		return -1;
	
	return 0;
}


#ifdef	WINDOWS_OS /*WINDOWS_OS*/
pid_t FindRunningSystem(void)
{
	HANDLE	hFile, hSem;
	char	filename[256];
	
	sprintf(filename, "%s%s", SYS_TMP_PATH, SYS_LOCK_FILE);
	hFile= CreateFile(filename, GENERIC_READ|GENERIC_WRITE, 
		0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	if(hFile!=INVALID_HANDLE_VALUE) /*task is not run*/
	{
		return OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SYS_RUN_SEM);
	}
	else /*task is running*/
	{
		hSem= OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SYS_RUN_SEM);
		if(hSem)
			return hSem;
		else
		{
			MessageBox(NULL, _CS_("SYSMGR_MSG_CSTR_RUNNING"), NULL, MB_OK);
			exit(0);
		}
	}	
}
#endif /*WINDOWS_OS*/

#ifdef	SUN_OS
pid_t FindRunningSystem(void)
{
	int		fd;
	struct	flock lock;
	char	filename[256];
	
	sprintf(filename, "%s%s", SYS_TMP_PATH, SYS_LOCK_FILE);
	if( (fd=open(filename, O_RDONLY, 0666)) == -1 )
		return 0;
	
	lock.l_type= F_WRLCK;
	lock.l_start= 0;
	lock.l_whence= SEEK_SET;
	lock.l_len= 0;
	if( fcntl(fd, F_GETLK, &lock) < 0 )
		return 0;
	
    if( lock.l_type==F_UNLCK )
		return 0;
	else
		return lock.l_pid;
}
#endif /*SUN_OS*/



void PopupMessageBox(char * message)
{
}


int	err_output_mode;

int	main(int argc, char *argv[])
{
	char	err_str[128];
	char	*display;
	int		i;
	char	process[MAX_PROCESS_NAME_SIZE];
	pid_t	ret_pid, sys_run_sem;
	int		show_ask_window_flag;
	FILE_COPY_SELECT	file_copy_sel={0, 1, 0, 0, 1};
#ifdef	WINDOWS_OS
	WSADATA	WSAData;
#else
	pthread_t	h_thread;
#endif

	if(-1==InitStrTextDefine())
		exit(0);
	
#ifdef	WINDOWS_OS
	if(WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
	{
		PopupMessageBox("Fail to initialize net lib");
		exit(0);
	}
#endif
	
	if(gethostname(local_hostname, sizeof(local_hostname)))	
	{
		PopupMessageBox("Fail to gethostname");
		exit(0);
	}
	
#ifdef	WINDOWS_OS
	strupr(local_hostname);
#endif
	
	if(argc>1 && strstr(argv[1], "local")) /*远程启动OIX*/
	{
		display= getenv("DISPLAY");
		if(display && display[0]!=':')
		{
			StartTaskProcess("oix");
			exit(0);
		}
	}

	if(argc>1 && strstr(argv[1], "no")) /*不弹出询问窗口*/
		show_ask_window_flag= 0;
	else
		show_ask_window_flag= 1;

	ret_pid= FindRunningSystem();
	if(ret_pid!=0) /*系统正在运行*/
	{
		SendTraceMsg(0, 0, "Now stop H9000 system");
		
#ifdef	WINDOWS_OS
		ReleaseSemaphore(ret_pid, 1, NULL);
#else
		kill(ret_pid, SIGTERM);
#endif
		exit(0);
	}

	
#ifndef	WINDOWS_OS
	CreateLockFile(SYS_LOCK_FILE);
#endif
		
	if(-1==chdir(SYS_RUN_PATH))
	{
		char	msg[258];

		sprintf(msg, "Fail to change work path to %s", SYS_RUN_PATH);
		PopupMessageBox(msg);
		exit(0);
	}

	if(-1==CheckSysRunPaths())
	{
		PopupMessageBox("Fail to create sys run paths");
		exit(0);
	}

	SendTraceMsg(0, 0, "Now start H9000 system");
	
	if(-1==MountDB(NULL))
	{
		PopupMessageBox("Fail to mount DATABASE");
		exit(0);
	}	

	if(show_ask_window_flag)
		RestoreEmsRunFiles(&file_copy_sel);		

	if(SIG_ERR==signal(SIGTERM, ExitEmsSystem))
	{
		PopupMessageBox("Fail to signal SIGTERM");
		exit(0);
	}
	if(SIG_ERR==signal(SIGINT, ExitEmsSystem))
	{
		PopupMessageBox("Fail to signal SIGINT");
		exit(0);
	}
	if(SIG_ERR==signal(SIGQUIT, ExitEmsSystem))
	{
		PopupMessageBox("Fail to signal SIGQUIT");
		exit(0);
	}
	
#ifdef	WINDOWS_OS
	if(-1==CreateEmsEnv())
	{
		PopupMessageBox("Fail to create ems environment");
		ClearEmsEnv();
		exit(0);
	}
	
#else
	if(fork()==0)
	{ /*sub process*/
		if(-1==CreateEmsEnv())
		{
			PopupMessageBox("Fail to create ems environment");
			ClearEmsEnv();
		}
		exit(0);
	}
	else
		wait((int*)NULL);
#endif
	
	if(!lan_out_init())
	{
		PopupMessageBox("Fail to init lan module");
		ClearEmsEnv();
		exit(0);
	}
	
	proc_num= GetRunProcessNum();
	SendTraceMsg(0, 0, "Total tasks num: %d", proc_num);
	if(-1==proc_num)
	{
		PopupMessageBox("Fail to get run task num");
		ClearEmsEnv();
		exit(0);
	}
	
#ifdef	WINDOWS_OS
	sys_run_sem=CreateSemaphore(NULL, 0, 1, SYS_RUN_SEM);
	if(NULL==sys_run_sem)
	{
		PopupMessageBox("Fail to create sys run sem");
		ClearEmsEnv();
		exit(0);
	}
#endif

	ClearOldEmsTasks();
	sleep(1);

	if(-1==CreateSysRunTasks())
		ExitEmsSystem(0);



#ifdef	WINDOWS_OS
	if(!CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MakeSysServeSock, 0, 0, NULL))
#else
	if(pthread_create(&h_thread, NULL, MakeSysServeSock, NULL))
#endif			
		SendTraceMsg(0, 0, "Fail to create thread MakeSysServeSock!");
	else
		SendTraceMsg(0, 0, "Succeed to create thread MakeSysServeSock!");
	

	for(;;)
	{
#ifdef	WINDOWS_OS
		run_pid[proc_num]= sys_run_sem;

		i= (int)(WaitForMultipleObjects(proc_num+1, run_pid, FALSE, INFINITE)-WAIT_OBJECT_0);
		if(i==proc_num || WAIT_OBJECT_0==WaitForSingleObject(sys_run_sem, 0)) /*received system stop singal*/
			ExitEmsSystem(0);
#else

		if(proc_num==0)
		{
			for(;;)
				sleep(1000000);
		}
		ret_pid= wait((int*)NULL);
		if(-1==ret_pid)
		{
			Err_log("Fail to waitpid");
			continue;
		}
		for(i=0; i<proc_num; i++)
		{
			if(ret_pid==run_pid[i])
				break;
		}
#endif

		sleep(1);
		if(i>=proc_num)
			continue;

		if(stop_system_flag)
		{
			for(;;)
				sleep(10000);
		}

		run_pid[i]= (pid_t)-1;
		if(-1==GetRunProcess(i, process))
		{
			SendTraceMsg(0, 0, "Fail to get task: %d!", i);
			continue;
		}

		SendTraceMsg(0, 0, "Task %s exit", process);
		sprintf(err_str, "%s %s exit", local_hostname, process);
		BroadCastSms(err_str, TRUE);

		run_pid[i]= StartTaskProcess(process);
		if(run_pid[i]<0)
			continue;
		SendTraceMsg(0, 0, "Success to restart task %s", process);
	}
}



#define	MAX_PROC_PARAM_NUM	16

pid_t	StartTaskProcess(char *process_name)
{
	char	task[MAX_PROCESS_NAME_SIZE+64];
	char	name[128], *ptr;

#ifdef	WINDOWS_OS
	STARTUPINFO				sui;
	PROCESS_INFORMATION		proc_info;
static	char	*suffix= ".exe"; 
#else
	pid_t	process_id;
static	char	*suffix= ".dxe"; 
#endif


	strcpy(name, process_name);						
	if(ptr=strchr(name, ' '))
	{
		*ptr= 0;
		sprintf(task, "%s%s%s %s", SYS_RUN_PATH, name, suffix, ptr+1);
	}
	else
		sprintf(task, "%s%s%s", SYS_RUN_PATH, process_name, suffix);

	if(err_output_mode != ERROR_LOG_NO)
		sprintf(task, "%s -errlog %d", task, err_output_mode);

#ifdef	WINDOWS_OS
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
    
	if(CreateProcess(NULL, task, NULL, NULL, FALSE, 0,  
				NULL, NULL, &sui, &proc_info))
		return proc_info.hProcess;
	SendTraceMsg(0, 0, "Fail to start task: %s!", task);
	return (pid_t)-1;

#else

	process_id= fork();
	if(process_id<0)
	{
		Err_log("Fail to fork!\n");
		return process_id;
	}

	else if(process_id==0)  /*child*/
	{
		char	strList[MAX_PROC_PARAM_NUM][MAX_CHAR_NUM_IN_LINE];
		char	*argv[MAX_PROC_PARAM_NUM+1];
		int		str_num;

		str_num= GetStringsFromStr(task, MAX_PROC_PARAM_NUM, strList);
		if(str_num==-1 || str_num==0)
			SendTraceMsg(0, 0, "Task string format error: %s!", task);
		else
		{
			int	i;
			for(i=0; i<str_num; i++)
				argv[i]= strList[i];
			argv[i]= 0;

			if(execv(argv[0], argv)<0)
				SendTraceMsg(0, 0, "Fail to start task: %s!", task);
		}
		exit(0);
	}
	/*father*/
	return process_id;
#endif
}


int	CreateSysRunTasks(void)
{
	char	process[MAX_PROCESS_NAME_SIZE];
	int		i;

	for(i=0; i<proc_num; i++)
	{
		run_pid[i]= (pid_t)-1;
		if(-1==GetRunProcess(i, process))
		{
			SendTraceMsg(0, 0, "Fail to get task: %d!", i);
			return -1;
		}

		if(strstr(process, "sysmgr"))
		{
			SendTraceMsg(0, 0, "Sysmgr can't be defined in task list");
			return -1;
		}

		run_pid[i]= StartTaskProcess(process);
		if((int)run_pid[i]<0)
			return -1;

		SendTraceMsg(0, 0, "Success to create task %s\n", process);
	}

	return 0;
}


void ClearOldEmsTasks(void)
{
	int		i;
	char	process[MAX_PROCESS_NAME_SIZE];
	char	cmd[256], *ptr;

	for(i=0; i<proc_num; i++)
	{
		if(-1==GetRunProcess(i, process))
		{
			SendTraceMsg(0, 0, "Fail to get task: %d!", i);
			continue;
		}
		if(ptr=strchr(process, ' '))
			*ptr= 0;
		if(IsTaskRunning(process))
		{
#ifdef WINDOWS_OS
			sprintf(cmd, "tskill /A %s", process);
#else
			sprintf(cmd, "pkill %s", process);
#endif
			system(cmd);
		}
	}
}


static void ExitEmsSystem( int signo )
{
	int		i;
	char	process[MAX_PROCESS_NAME_SIZE];
	POINTER	point;
	
	stop_system_flag= 1;
	
	if(-1!=GetHostStatePtId(local_hostname, &point))
		BroadCastIndPtState(point, 0, 0);
	
	SaveSysStopTime();
	
	sleep(1);
	
	for(i=0; i<proc_num; i++)
	{
		if((pid_t)-1==run_pid[i])
			continue;
		
		if(-1==GetRunProcess(i, process))
		{
			SendTraceMsg(0, 0, "Fail to get task: %d!", i);
			continue;
		}
		kill(run_pid[i], SIGTERM);
		SendTraceMsg(0, 0, "stop task %s", process);
	}
	
	if(IsTaskRunning("oix"))
#ifdef WINDOWS_OS
		system("tskill /A oix");
#else
		system("pkill oix");
#endif
	
	sleep(1);
	ClearEmsEnv();
	UnMountDB();
	if(run_serve_sock!=-1)
		closesocket(run_serve_sock);
	exit(0);
}


typedef	struct
{
	char	cmd;
	char	res[3];
	char	para[128];
} SESSION_INFO;

enum SESSION_CMD  {SESSION_CMD_GET_FILE,
					SESSION_CMD_SET_FILE,
					SESSION_CMD_GET_EVENTFILE_NUM,
					SESSION_CMD_END};


void SaveSysStopTime(void)	
{ 
	char		filename[256];
	struct tm	t_tm;
	long		newtime; 
	FILE		*fp;
	
	newtime=time(NULL);
	t_tm= *localtime( &newtime );
	sprintf(filename, "%s%s", SYS_TMP_PATH, EMS_RUN_TIME_FILE);
	if((fp=fopen(filename, "w")) == NULL)
	{
		SendTraceMsg(0, 0, "Fail to open %s", filename);
		return;
	}
	
	fprintf(fp, "H9000_IS_STOPPING_NOW!!!\n%d (%04d-%02d-%02d %02d:%02d:%02d)\n",
		newtime, t_tm.tm_year+1900, t_tm.tm_mon+1, t_tm.tm_mday,
		t_tm.tm_hour, t_tm.tm_min, t_tm.tm_sec);
	fclose(fp);
}


unsigned int GetEmsDate(void)		
{ 
	char	filename[256];
	long	newtime;
	char    err_str[128];
	FILE    *fp;
	
	sprintf(filename, "%s%s", SYS_TMP_PATH, EMS_RUN_TIME_FILE);
	if((fp=fopen(filename, "r")) == NULL)
	{
		SendTraceMsg(0, 0, "Fail to open %s", EMS_RUN_TIME_FILE);
		return 0;
	}
	
	if(fscanf(fp, "%s %d", err_str, &newtime) != 2)
		newtime= 0;
	
	fclose ( fp );
	return newtime;
}


void GetEventFileName(long time, int nNum, char *filename)
{
	struct tm      t_tm;
	
	t_tm = *localtime( &time ); 
	if(nNum==0)
	{
#ifdef ENGLISH_VER
		sprintf(filename, "%sEVENT%02d-%s-%04d", EVENT_LOG_PATH_UNIX, t_tm.tm_mday, mthstr[t_tm.tm_mon],t_tm.tm_year+1900);
#else                
		sprintf(filename, "%sEVENT%04d%02d%02d", EVENT_LOG_PATH_UNIX, t_tm.tm_year+1900,t_tm.tm_mon+1,t_tm.tm_mday);
#endif
	}
	else
	{
#ifdef ENGLISH_VER
		sprintf(filename, "%sEVENT%02d-%s-%04dNo%02d.REC", EVENT_LOG_PATH_UNIX, t_tm.tm_mday, mthstr[t_tm.tm_mon],t_tm.tm_year+1900,nNum);
#else                
		sprintf(filename, "%sEVENT%04d%02d%02dNo%02d.REC", EVENT_LOG_PATH_UNIX, t_tm.tm_year+1900,t_tm.tm_mon+1,t_tm.tm_mday,nNum);
#endif
	}
}


void GetInfoFileName(long time, char *filename)
{
	struct tm      t_tm;
	
	t_tm = *localtime( &time ); 
	
#ifdef ENGLISH_VER
	sprintf(filename, "%sINFO%02d-%s-%04d.REC", EVENT_LOG_PATH_UNIX, t_tm.tm_mday, mthstr[t_tm.tm_mon],t_tm.tm_year+1900);
#else                
	sprintf(filename, "%sINFO%04d%02d%02d.REC", EVENT_LOG_PATH_UNIX, t_tm.tm_year+1900,t_tm.tm_mon+1,t_tm.tm_mday);
#endif
}

BOOL	IsTextFILE(char *filename)
{
	if(!strcmp(filename+strlen(filename)-4, ".def"))
		return TRUE;
	if(!strcmp(filename+strlen(filename)-4, ".DEF"))
		return TRUE;
	if(!strcmp(filename+strlen(filename)-4, ".txt"))
		return TRUE;
	if(!strcmp(filename+strlen(filename)-4, ".TXT"))
		return TRUE;
	return FALSE;
}

int SocketReadable(int sock, int sec)
{
	fd_set	rset;
	struct timeval tv;
	int		len, error;
	
	len= sizeof(error);
	getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
	FD_ZERO(&rset);
	FD_SET(sock, &rset);
	tv.tv_sec= sec;
	tv.tv_usec= 0;
	return select(sock+1, &rset, NULL, NULL, &tv);	
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
		SendTraceMsg(0, 0, "Fail to set socket NONBLOCK");
		return -1;
	}
	return 0;	
}

int	SetSocketBlock(int sockfd)
{
	int		ret;
	
	ret= 0;
#ifdef	WINDOWS_OS
	if( ioctlsocket(sockfd, FIONBIO, &ret) )
#else
	if( -1==ioctl(sockfd, FIONBIO, &ret) )
#endif
	{ 
		SendTraceMsg(0, 0, "Fail to set socket BLOCK");
		return -1;
	}
	return 0;	
}



int SendEndSessionCmd(int sock)
{
	int		ret;
	SESSION_INFO	info;
	
	memset(&info, 0, sizeof(info));
	info.cmd= SESSION_CMD_END;
	
	ret=send(sock, (char*)&info, sizeof(info), 0);
	return 0;
}	


int GetEventFilesNumFromElseHost(int sock, long time)
{
	int		ret, file_num;
	SESSION_INFO	info;
	
	memset(&info, 0, sizeof(info));
	info.cmd= SESSION_CMD_GET_EVENTFILE_NUM;
	GetEventFileName(time, 0, info.para);
	
	ret=send(sock, (char*)&info, sizeof(info), 0);
	
	ret= SocketReadable(sock, 10);
	if(ret == 0)
	{
		SendTraceMsg(0, 0, "wait server respond timeout");
		return -1;
	}
	else if(ret < 0)
	{
		SendTraceMsg(0, 0, "wait server respond error");
		return -1;
	}
	
	ret= recv(sock, (char*)&file_num, sizeof(file_num), 0);
	if(ret != sizeof(file_num))
	{
		SendTraceMsg(0, 0, "receive server respond eventfile num error: ret= %d, error= %d %s", ret, errno, info.para);
		return -1;
	}
	
	return ntohl(file_num);
}

int GetFileFromElseHost(int sock, char *filename, int timeout)
{
	FILE	*fp;
	int		ret, file_size, recv_size;
	char	*buff;
	SESSION_INFO	info;
	BOOL	txt_file_flag;
	
	txt_file_flag= IsTextFILE(filename);
	
	memset(&info, 0, sizeof(info));
	info.cmd= SESSION_CMD_GET_FILE;
	strcpy(info.para, filename);
	
	ret=send(sock, (char*)&info, sizeof(info), 0);

	ret= SocketReadable(sock, timeout);
	if(ret == 0)
	{
		SendTraceMsg(0, 0, "wait server respond timeout");
		return -1;
	}
	else if(ret < 0)
	{
		SendTraceMsg(0, 0, "wait server respond error");
		return -1;
	}

	ret= recv(sock, (char*)&file_size, sizeof(file_size), 0);
	if(ret != sizeof(file_size))
	{
		SendTraceMsg(0, 0, "receive server respond file size error: ret= %d, error= %d %s", ret, errno, filename);
		return -1;
	}
	file_size= ntohl(file_size);
	
	if(-1==file_size)	
	{
		SendTraceMsg(0, 0, "Fail to get %s from else host", filename);
		return -1;
	}

	ret= SocketReadable(sock, 30);
	if(ret == 0)
	{
		SendTraceMsg(0, 0, "wait server respond timeout");
		return -1;
	}
	else if(ret < 0)
	{
		SendTraceMsg(0, 0, "wait server respond error");
		return -1;
	}
	
	ret= recv(sock, (char*)&info.para, sizeof(info.para), 0);
	if(ret != sizeof(info.para))
	{
		SendTraceMsg(0, 0, "receive server respond file name error: ret= %d error= %d %s", ret, errno, filename);
		return -1;
	}
	if(strcmp(info.para, filename))
	{
		SendTraceMsg(0, 0, "received file name isn't same as sent: %s %s", info.para, filename);
		return -1;
	}
	
	buff= malloc(file_size+16);
	if(buff == NULL)
	{
		SendTraceMsg(0, 0, "Fail to malloc size %d", file_size);
		return -1;
	}
	
	recv_size= 0;
	while(recv_size<file_size)
	{
		ret= SocketReadable(sock, 10);
		if(ret == 0)
		{
			SendTraceMsg(0, 0, "wait server respond timeout");
			free(buff);
			return -1;
		}
		else if(ret < 0)
		{
			SendTraceMsg(0, 0, "wait server respond error");
			free(buff);
			return -1;
		}
		
		ret= recv(sock, buff+recv_size, file_size-recv_size, 0);
		if(ret <= 0)
		{
			SendTraceMsg(0, 0, "receive file %s error: ret= %d, error= %d recv_size= %d", filename, ret, errno, recv_size);
			free(buff);
			return -1;
		}
		recv_size += ret;
	}
	SendTraceMsg(0, 0, "Succeed to receive file %s: file_size= %d", filename, file_size);

	if(txt_file_flag)
	{
		if(NULL== (fp=fopen(filename, "wt")))
		{
			free(buff);
			SendTraceMsg(0, 0, "Fail to open file: %s", filename);
			return 0;
		}
	}
	else
	{
		if(NULL== (fp=fopen(filename, "wb")))
		{
			free(buff);
			SendTraceMsg(0, 0, "Fail to open file: %s", filename);
			return 0;
		}
	}
	
	if(file_size>0 && 1!=fwrite(buff, file_size, 1, fp))
	{
		fclose(fp);
		free(buff);
		SendTraceMsg(0, 0, "Fail to write file: %s", filename);
		return 0;
	}
	
	fclose(fp);
	free(buff);
	return 0;
}


int SendEventFileNum(int sock, SESSION_INFO *info)
{
	FILE	*fp;
	int		num=1, send_value;
	char	filename[256];
	int		ret;
	
	for(;;)
	{
		sprintf(filename, "%sNo%02d.REC", info->para, num);
		if(NULL==(fp=fopen(filename, "r")))
			break;
			
		fclose(fp);
		num++;
	}
	num--;

	send_value= htonl(num);
	ret=send(sock, (char*)&send_value, sizeof(send_value), 0);
	if(ret != sizeof(num))
	{
		SendTraceMsg(0, 0, "Fail to send event files num: ret= %d num= %d", ret, num);
		return -1;
	}
	return 0;
}

int SendFileToElseHost(int sock, SESSION_INFO *info)
{
	FILE	*fp;
	int		ret, file_size, read_size, send_value;
	char	*buff;
	char	filename[256];
	BOOL	txt_file_flag;

	file_size= -1;
	strcpy(filename, info->para);
	txt_file_flag= IsTextFILE(filename);
	
	if(txt_file_flag)
	{
		if(NULL== (fp=fopen(filename, "rt")))
		{
			SendTraceMsg(0, 0, "Fail to open file: %s", filename);
			send_value= htonl(file_size);
			return send(sock, (char*)&send_value, sizeof(send_value), 0);
		}
	}
	else
	{
		if(NULL== (fp=fopen(filename, "rb")))
		{
			SendTraceMsg(0, 0, "Fail to open file: %s", filename);
			send_value= htonl(file_size);
			return send(sock, (char*)&send_value, sizeof(send_value), 0);
		}
	}
	
	fseek(fp, 0, 2);
	file_size= ftell(fp);
	buff= malloc(file_size+16);
	if(buff == NULL)
	{
		SendTraceMsg(0, 0, "Fail to malloc size %d", file_size);
		fclose(fp);
		file_size= -1;
		send_value= htonl(file_size);
		return send(sock, (char*)&send_value, sizeof(send_value), 0);
	}
	fseek(fp, 0, 0);
	read_size= fread(buff, 1, file_size, fp);
	fclose(fp);
	if(!txt_file_flag && read_size!=file_size)
	{
		SendTraceMsg(0, 0, "Fail to read file %s filesize= %d", filename, file_size);
		free(buff);
		file_size= -1;
		send_value= htonl(file_size);
		return send(sock, (char*)&send_value, sizeof(send_value), 0);
	}

	send_value= htonl(read_size);
	ret=send(sock, (char*)&send_value, sizeof(send_value), 0);
	if(ret != sizeof(read_size))
	{
		SendTraceMsg(0, 0, "Fail to send file size: ret= %d err= %d", ret, errno);
		free(buff);
		return -1;
	}

	ret=send(sock, info->para, sizeof(info->para), 0);
	if(ret != sizeof(info->para))
	{
		SendTraceMsg(0, 0, "Fail to send file name: ret= %d error= %d %s", ret, errno, filename);
		free(buff);
		return -1;
	}
	
	ret=send(sock, buff, read_size, 0);
	if(ret != read_size)
	{
		SendTraceMsg(0, 0, "Fail to send file: ret= %d error= %d read_size= %d %s", ret, errno, read_size, filename);
		free(buff);
		return -1;
	}

	SendTraceMsg(0, 0, "Succeed to send file %s", filename);
	
	free(buff);
	return 0;
}


int CopyRunFilesSession(int sock, FILE_COPY_SELECT	*file_copy_sel)
{
	char	filename[256];
	long	newtime, oldtime; 
	
	if(file_copy_sel->config)
	{
		sprintf(filename, "%s%s", SYS_DEFINE_PATH_UNIX, SYS_DEFINE_FILE);
		if(-1==GetFileFromElseHost(sock, filename, 10))
			return -1;
	}

	if(file_copy_sel->db)
	{
		UnMountDB();
		sprintf(filename, "%s%s", DATABASE_OUT_PATH_UNIX, DATABASE_NAME);
		if(-1==GetFileFromElseHost(sock, filename, 60))
		{
			SendEndSessionCmd(sock);
			MountDB(NULL);
			return -1;
		}
		if(-1==MountDB(NULL))
		{
			SendTraceMsg(0, 0, "Fail to MountDB: Database file or ems_config file gotten is bad");
			SendEndSessionCmd(sock);
			return -1;
		}
	}
	
	if(file_copy_sel->calcu)
	{
		sprintf(filename, "%s%s", DATABASE_OUT_PATH_UNIX, CALCU_DATA_FILE);
		if(-1==GetFileFromElseHost(sock, filename, 10))
		{
			SendEndSessionCmd(sock);
			return -1;
		}
		
		sprintf(filename, "%s%s", DATABASE_OUT_PATH_UNIX, CALCU1_DATA_FILE);
		if(-1==GetFileFromElseHost(sock, filename, 10))
		{
			SendEndSessionCmd(sock);
			return -1;
		}
	}

	if(file_copy_sel->interlock)
	{
		sprintf(filename, "%s%s", DATABASE_OUT_PATH_UNIX, INTERLOCK_DATA_FILE);
		if(-1==GetFileFromElseHost(sock, filename, 10))
		{
			SendEndSessionCmd(sock);
			return -1;
		}
		
	}

	if(file_copy_sel->event)
	{
		newtime = time(NULL);
		if((oldtime= GetEmsDate()) == 0)
		{
			SendTraceMsg(0, 0, "Fail to get H9000 run last time");
			oldtime= newtime;
		}
		GetInfoFileName(newtime, filename);
		if(-1==GetFileFromElseHost(sock, filename, 10))
		{
			SendEndSessionCmd(sock);
			return -1;
		}
		
		do
		{
			int	i, file_num;
			file_num= GetEventFilesNumFromElseHost(sock, newtime);
			for(i=1; i<=file_num; i++)
			{
				GetEventFileName(newtime, i, filename);
				if(-1==GetFileFromElseHost(sock, filename, 10))
					break;
			}
			newtime = newtime - (60*60*24);
		} while( newtime >= oldtime );		
	}
	
	SendEndSessionCmd(sock);

	return 0;	
}


void* SessionServeRespond(void *para)
{
	int		sock= (int)para;
	int		ret;
	SESSION_INFO	info;

	for(;;)
	{
		if(-1==SetSocketNoBlock(sock))
			break;
		
		ret= SocketReadable(sock, 20);
		if(ret == 0)
		{
			SendTraceMsg(0, 0, "wait client request timeout");
			break;
		}
		else if(ret < 0)
		{
			SendTraceMsg(0, 0, "wait client request error");
			break;
		}
		
		ret= recv(sock, (char*)&info, sizeof(info), 0);
		if(ret==0)
			break;
		
		if(ret!=sizeof(info))
		{
			SendTraceMsg(0, 0, "Fail to recv client info: ret= %d error= %d", ret, errno);
			break;
		}
		
		if(-1==SetSocketBlock(sock))
			break;
		
		if(info.cmd==SESSION_CMD_GET_FILE)
		{
			if(-1==SendFileToElseHost(sock, &info))
			{
				SendTraceMsg(0, 0, "Fail to send file: %s", info.para);
				break;
			}
		}
		else if(info.cmd==SESSION_CMD_SET_FILE)
		{


		}
		else if(info.cmd==SESSION_CMD_GET_EVENTFILE_NUM)
		{
			if(-1==SendEventFileNum(sock, &info))
			{
				SendTraceMsg(0, 0, "Fail to send event files num: %s", info.para);
				break;
			}
		}
		else if(info.cmd==SESSION_CMD_END)
			break;
	}
	closesocket(sock);
	return (void*)0;	
}


void* MakeSysServeSock(void *para)
{
#ifndef	WINDOWS_OS
	int		val;
#endif

	int		optval;
	struct  sockaddr_in local_addr, client_addr;

	for(;;)
	{
		if(stop_system_flag)
		{
			run_serve_sock= -1;
			return NULL;
		}
		
		if(-1==(run_serve_sock=socket(AF_INET, SOCK_STREAM, 0)))
		{ 
			Err_log("Fail to open system running serve socket");
			sleep(2);
			continue;
		}

	#ifndef	WINDOWS_OS
		val= fcntl(run_serve_sock, F_GETFD, 0);
		if(val<0)
		{
			Err_log("Fail to fcntl F_GETFD serve socket\n");
			closesocket(run_serve_sock);
			run_serve_sock= -1;
			sleep(2);
			continue;
		}

		val |= FD_CLOEXEC;
		if(fcntl(run_serve_sock, F_SETFD, val)<0)
		{
			Err_log("Fail to fcntl serve socket CLOSE_EXEC");
			closesocket(run_serve_sock);
			run_serve_sock= -1;
			sleep(2);
			continue;
		}
	#endif

		optval= 1;
		if(setsockopt(run_serve_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&optval, sizeof(optval)))
		{ 
			Err_log("Fail to reuse system running serve socket");
			closesocket(run_serve_sock);
			run_serve_sock= -1;
			sleep(2);
			continue;
		}

		bzero((char *)&local_addr, sizeof(local_addr));
		local_addr.sin_addr.s_addr=htonl(INADDR_ANY);
		local_addr.sin_family     =AF_INET;
		local_addr.sin_port       =htons(SYS_RUN_SERV_TCP_PORT);

		if(bind(run_serve_sock, (struct sockaddr *)&local_addr, sizeof(local_addr)))
		{ 
			Err_log("Fail to bind system running serve socket");
			closesocket(run_serve_sock);
			run_serve_sock= -1;
			sleep(2);
			continue;
		}

		if(listen(run_serve_sock, 5))
		{ 
			Err_log("Fail to listen system running serve socket");
			closesocket(run_serve_sock);
			run_serve_sock= -1;
			sleep(2);
			continue;
		}
		
		for(;;)
		{
			int		accept_sock;
			
			if(stop_system_flag)
			{
				closesocket(run_serve_sock);
				run_serve_sock= -1;
				return NULL;
			}
			
			optval= sizeof(client_addr);
			accept_sock= accept(run_serve_sock, (struct sockaddr*)&client_addr, &optval);
			if(accept_sock < 0)
			{
				Err_log("Fail to accept incoming link");
				closesocket(run_serve_sock);
				run_serve_sock= -1;
				sleep(2);
				break;
			}

#ifdef	WINDOWS_OS
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SessionServeRespond, (void*)accept_sock, 0, NULL);
#else
//			thr_create(NULL, 0, SessionServeRespond, (void*)accept_sock, 0, NULL);
			if(fork()==0)
			{
				SessionServeRespond((void*)accept_sock);
				exit(0);
			}
			closesocket(accept_sock);			
#endif
		}
	}

	exit(-1);
}


int	CreateSockNoBlock(void)
{
	int		sockfd;

	if(-1==(sockfd=socket(AF_INET, SOCK_STREAM, 0)))
	{ 
		SendTraceMsg(0, 0, "Fail to create stream socket");
		return -1;
	}

	if(-1==SetSocketNoBlock(sockfd))
	{ 
		closesocket(sockfd);
		return -1;
	}
	return sockfd;
}


int RestoreEmsRunFiles(FILE_COPY_SELECT	*file_copy_sel)
{ 
	struct sockaddr_in   serv_addr;
	int		hostNo, netNo;
	char	rcp_hostname[MAX_HOST_NAME_SIZE];
	char	ipaddr[MAX_IPADDR_SIZE];
	int		link_sock;
	fd_set	wset;
	struct	timeval tv;
	int		sys_net_num, rcp_host_num;
	char	err_str[128];
	int		ret, error, len;

	sys_net_num= GetSysNetNum();
	rcp_host_num= GetRcpHostNum();
	if(rcp_host_num==0)
	{ 
		Err_log("No define rcp host");
		return -1;
	}

	for(hostNo=0; hostNo<rcp_host_num; hostNo++)
	{
		if(-1==GetRcpHostName(hostNo, rcp_hostname))
			continue;

		if(!strcmp(local_hostname, rcp_hostname))
			continue;

		for(netNo=0; netNo<sys_net_num; netNo++)
		{
			if(-1==GetHostIpaddr(rcp_hostname, netNo, ipaddr))
				break;

			bzero((char *)&serv_addr,sizeof(serv_addr));
			if(-1==(serv_addr.sin_addr.s_addr=inet_addr(ipaddr)))
			{ 
				sprintf(err_str, "Host %s ipaddr %s error", rcp_hostname, ipaddr);
				Err_log(err_str);
				continue;
			}
			serv_addr.sin_family     =AF_INET;
			serv_addr.sin_port       =htons(SYS_RUN_SERV_TCP_PORT);

			if(-1==(link_sock=CreateSockNoBlock()))
			{ 
				Err_log("Fail to create link socket");
				return -1;
			}

			ret= connect(link_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
			if(ret == 0)
				goto SUCCESS;               /* connect completed immediately */
			
			/*or else ret==-1 SOCKET_ERROR*/
#ifdef	WINDOWS_OS
			if(WSAGetLastError() != WSAEWOULDBLOCK)
#else
			if(errno != EINPROGRESS)
#endif
			{
				sprintf(err_str, "fail to connect %s", ipaddr);
				Err_log(err_str);
				closesocket(link_sock);
				continue;
			}

			len= sizeof(error);
			getsockopt(link_sock, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
			FD_ZERO(&wset);
			FD_SET(link_sock, &wset);
			tv.tv_sec= 1;
			tv.tv_usec= 0;
			ret= select(link_sock+1, NULL, &wset, NULL, &tv);
			if(ret == 0)
			{
				sprintf(err_str, "connect %s timeout", ipaddr);
				Err_log(err_str);
				closesocket(link_sock);
				continue;
			}
			else if(ret < 0)
			{
				sprintf(err_str, "select connect %s error", ipaddr);
				Err_log(err_str);
				closesocket(link_sock);
				continue;
			}
			else /* ret > 0 */
			{
				len= sizeof(error);
				getsockopt(link_sock, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
				if(error)
				{
					sprintf(err_str, "connect %s refused", ipaddr);
					Err_log(err_str);
					closesocket(link_sock);
					continue;
				}				
			}

SUCCESS:
			sprintf(err_str, "connect %s success", ipaddr);
			Err_log(err_str);
				
			ret= CopyRunFilesSession(link_sock, file_copy_sel);
			closesocket(link_sock);
			if(ret==0)
				return 0;
		}
	}
	return -1;
}









