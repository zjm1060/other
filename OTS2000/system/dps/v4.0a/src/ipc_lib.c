#include	"../../../dms/v4.0a/inc/dms_com.h"
#include 	"../inc/ipc_common.h"
#include 	"../inc/dps_type_def.h"
#include 	"../inc/dps_rec_def.h"
#include 	"../inc/fault_table_def.h"
#include 	"../../../lan/v4.0a/inc/lan_common.h"
#include 	"../../../plc/v4.0a/inc/lcu_data.h"
#include 	"../../../dpp/v4.0a/inc/dppdef.h"
#include 	"../../../hdb/v4.0a/inc/hdb_data.h"
#include 	"../../../pas/v4.0a/inc/sgzy.h"
#include 	"../../../pas/v4.0a/inc/trend_data_rec.h"
#include 	"../../../ots/SimuEngine/inc/SimuEngine_data.h"


/*#define		DEBUG*/




int		CreateSemSet(void);
void*	CreateShareMemory(char key_char, int size, char *shm_name);
int		UnlinkShareMemory(void *shm_addr, char *shm_name);




int	CreateEmsEnv(void)
{
	void	*shm_addr;
	int		stn, dev;
	
#ifndef	WINDOWS_OS /*UNIX_OS*/
	int		fd;
	char	keyfile[128];
	char	err_str[128];

	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	if( fd= open(keyfile, O_CREAT, 0444) == -1 )
	{
		sprintf(err_str, "Fail to create %s!\n", keyfile);
       	Err_log(err_str);	
    	return -1;
    }
    close(fd);	
#endif

	/* create record share memory */
	shm_addr= CreateShareMemory((char)SHM_REC_KEY_CHAR, SHM_REC_SIZE, "event_record");
	if((void*)-1==shm_addr)
    	return -1;

	((SHM_RECORD*)shm_addr)->head.buf_size= MAX_SHM_RECORD_NUM;
	((SHM_RECORD*)shm_addr)->head.count= 0;
	((SHM_RECORD*)shm_addr)->head.end_ptr= 0;
	((SHM_RECORD*)shm_addr)->head.header_ptr= 0;

	/* create data_send share memory */
	shm_addr= CreateShareMemory((char)SHM_SEND_KEY_CHAR, SHM_SEND_DATA_SIZE, "data_send");
	if((void*)-1==shm_addr)
    	return -1;

/*
	((SHM_SEND_DATA*)shm_addr)->head= 0;
	((SHM_SEND_DATA*)shm_addr)->flag= 0;
*/

	/* create data_recv share memory */
	shm_addr= CreateShareMemory((char)SHM_RECV_KEY_CHAR, SHM_RECV_DATA_SIZE, "data_recv");
	if((void*)-1==shm_addr)
    	return -1;

/*
	((SHM_RECV_DATA*)shm_addr)->head= 0;
	((SHM_RECV_DATA*)shm_addr)->flag= 0;
*/

	/* create net_cmd share memory */
	shm_addr= CreateShareMemory((char)SHM_NETCMD_KEY_CHAR, NET_CMD_SIZE, "net_cmd");
	if((void*)-1==shm_addr)
    	return -1;

	/* create plc_cmd share memory */
	shm_addr= CreateShareMemory((char)SHM_PLCCMD_KEY_CHAR, PLC_CMD_SIZE, "plc_cmd");
	if((void*)-1==shm_addr)
    	return -1;

	/* create plc_data share memory */
	shm_addr= CreateShareMemory((char)SHM_PLCDATA_KEY_CHAR, PLC_DATA_SIZE, "plc_data");
	if((void*)-1==shm_addr)
    	return -1;

	/* create sgzy share memory */
	shm_addr= CreateShareMemory((char)SHM_SGZY_KEY_CHAR, sizeof(SGZY_SHM_DATA), "sgzy");
	if((void*)-1==shm_addr)
    	return -1;

	/* create wdqs share memory */
	shm_addr= CreateShareMemory((char)SHM_WDQS_KEY_CHAR, sizeof(WDQS_SHM_DATA), "wdqs");
	if((void*)-1==shm_addr)
    	return -1;

	/* create hdb_data share memory */
	shm_addr= CreateShareMemory((char)SHM_HDB_KEY_CHAR, SHM_HDB_SIZE, "hdb_data");
	if((void*)-1==shm_addr)
    	return -1;

	/* create sys_log share memory */
	shm_addr= CreateShareMemory((char)SHM_SYSLOG_KEY_CHAR, SHM_LOG_SIZE, "sys_log");
	if((void*)-1==shm_addr)
    	return -1;

	/* create plc_send share memory */
	shm_addr= CreateShareMemory((char)SHM_PLCSEND_KEY_CHAR, PLC_SEND_SIZE, "plc_send");
	if((void*)-1==shm_addr)
    	return -1;

	/* create fault_table share memory */
	shm_addr= CreateShareMemory((char)SHM_FAULT_TABLE_KEY_CHAR, sizeof(FAULT_RECORD_TABLE), "fault_table");
	if((void*)-1==shm_addr)
    	return -1;

	((FAULT_RECORD_TABLE*)shm_addr)->head.buf_size= MAX_FAULT_RECORD_NUM;
	((FAULT_RECORD_TABLE*)shm_addr)->head.header_ptr= 0;
	((FAULT_RECORD_TABLE*)shm_addr)->head.end_ptr= 0;
	((FAULT_RECORD_TABLE*)shm_addr)->head.count= 0;

	/* create REAL_CURVE share memory */
	shm_addr= CreateShareMemory((char)SHM_TREND_KEY_CHAR, sizeof(CURVE_SHM_DATA), "real_curve");
	if((void*)-1==shm_addr)
    	return -1;

	/* create SYS_COMM_TABLE share memory */
	shm_addr= CreateShareMemory((char)SHM_SYSCOMM_TABLE_KEY_CHAR, sizeof(SYS_COMM_TABLE), "sys_comm_table");
	if((void*)-1==shm_addr)
    	return -1;

	for(stn=0;stn<STATION_ENTRY_NUMBER;stn++)
	{
		for(dev=0;dev<GROUP_ENTRY_NUM_PER_STATION;dev++)
		{
			((SYS_COMM_TABLE*)shm_addr)->dev_alm_disable[stn][dev]= 0;
			((SYS_COMM_TABLE*)shm_addr)->data_poll[stn][dev].poll_flag= 0;
		}
	}

	/* create info record share memory */
	shm_addr= CreateShareMemory((char)SHM_INFO_RECORD_KEY_CHAR, sizeof(INFO_RECORD), "info_record");
	if((void*)-1==shm_addr)
    	return -1;

	((INFO_RECORD*)shm_addr)->head.buf_size= MAX_INFO_RECORD_NUM;
	((INFO_RECORD*)shm_addr)->head.count= 0;
	((INFO_RECORD*)shm_addr)->head.end_ptr= 0;
	((INFO_RECORD*)shm_addr)->head.header_ptr= 0;

	/* create OTS share memory */
	shm_addr= CreateShareMemory((char)SHM_OTS_KEY_CHAR, SHM_OTS_SIZE, "ots_share");
	if((void*)-1==shm_addr)
		return -1;
	
	
	/* create COMMUNITCATION COMMAND share memory */
	/* modified by yyp 2011.03.09 */
	shm_addr= CreateShareMemory((char)SHM_COMMCMD_KEY_CHAR, COMM_CMD_SIZE, "comm_cmd_buf");
	if((void*)-1==shm_addr)
		return -1;
	
	
	/* create semaphore set */
	if( CreateSemSet() == -1 )
		return -1;

	Err_log("Succeed to create ems Env!");
	return 0;
}



#ifdef	SUN_OS

void*	CreateShareMemory(char key_char, int size, char *shm_name)
{
	char	err_str[128];
	int		shm_id;
	void	*shm_addr;
	key_t	key;
	char	keyfile[128];

	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	if( (key=ftok(keyfile, key_char))== -1 )
	{
		sprintf(err_str, "Fail to generate %s key: %d!", shm_name, errno);
       	Err_log(err_str);	
		return (void*)-1;
	}
	if( (shm_id=shmget(key, size, IPC_CREAT|0660)) == -1 )
	{
		sprintf(err_str, "Fail to create %s share memory: %d!", shm_name, errno);
        Err_log(err_str);	
    	return (void*)-1;
	}
    
    if( (shm_addr=shmat(shm_id, 0, 0)) == (void*)-1 )
	{
		sprintf(err_str, "Fail to shmat %s share memory: %d!", shm_name, errno);
        Err_log(err_str);	
    	return (void*)-1;
	}
	printf("\nCreate %s share memory: 0x%x", shm_name, key);
	return shm_addr;
}


int	UnlinkShareMemory(void *shm_addr, char *shm_name)
{
	char	err_str[128];

	if( shmdt(shm_addr) == -1 )
	{
		sprintf(err_str, "Fail to unlink %s share memory: %d", shm_name, errno);
        Err_log(err_str);	
    	return -1;
	}	
	return 0;
}



int CreateSemSet(void)
{
	int		sem_id;
	union semun 
	{
		int		val;
		struct	semid_ds	*buf;
		ushort	*array;
	} arg;
	USHORT	array[MAX_SEMAPHORE_NUM];
	char	err_str[128];
	int		i;
	key_t	key;
	char	keyfile[128];

	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	if( (key=ftok(keyfile, (char)SEM_KEY_CHAR))== -1 )
	{
       	Err_log("Fail to generate semaphore key!\n");	
		return -1;
	}
	if( (sem_id=semget(key, MAX_SEMAPHORE_NUM, IPC_CREAT|0660)) == -1 )
	{
		sprintf(err_str, "Fail to create semaphore set!\n");
        Err_log(err_str);	
    	return -1;
	}
	for(i=0; i<MAX_SEMAPHORE_NUM; i++)
		array[i]= 0;
	
	array[SEM_WR_SEND]= 1;
	array[SEM_WR_NETCMD]= 1;
	array[SEM_WR_SYSLOG]= 1;
	array[SEM_WR_COMMCMD]= 1;

	arg.array= array;
	if( semctl(sem_id, 0, SETALL, arg) == -1 )
	{
		sprintf(err_str, "Fail to initialize semaphore set!\n");
        Err_log(err_str);	
    	return -1;
	}
	printf("\nCreate sem set: 0x%x", key);
	return 0;
}



void*	LinktoShareMem(char key_char)
{
	key_t	key;
	int		shm_id;
	char	err_str[128];
	char	keyfile[128];
	
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	if( (key=ftok(keyfile, key_char))== -1 )
	{
		sprintf(err_str, "Fail to generate key %c!\n", key_char);
       	Err_log(err_str); 	
		return (void*)-1;
	}
	if( (shm_id=shmget(key, 0, 0660)) == -1 )
	{
		sprintf(err_str, "Fail to shmget share memory %c!\n", key_char);
       	Err_log(err_str); 	
    	return (void*)-1;
	}
	
    return shmat(shm_id, 0, 0);
}



static	int	semaphore_id= -1;

int	LinktoSemSet(int semName)
{
	char	err_str[128];
	char	keyfile[128];
	
	if(semName<0 || semName>=MAX_SEMAPHORE_NUM)
	{
		sprintf(err_str, "No such a semaphore %d in set!\n", semName);
        Err_log(err_str);	
    	return -1;
	}

	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	if( (semaphore_id=semget(ftok(keyfile, (char)SEM_KEY_CHAR), MAX_SEMAPHORE_NUM, 0660)) == -1 )
	{
		sprintf(err_str, "Fail to link semaphore set!\n");
        Err_log(err_str);	
    	return -1;
	}

	return 0;
}





int	WaitforSem(int semName, int timeout)
{
	char	err_str[128];
	struct	sembuf sops;
	int		ret, err;
	struct timespec time_out;

	if(-1==semaphore_id)
	{
       	Err_log("Semaphore set no link!\n");	
    	return -1;
	}

	if(semName<0 || semName>=MAX_SEMAPHORE_NUM)
	{
		sprintf(err_str, "No such a semaphore %d in set!\n", semName);
       	Err_log(err_str);	
    	return -1;
	}
	
	sops.sem_num= semName;
	sops.sem_op= -1;
	sops.sem_flg= SEM_UNDO;	

	if( timeout>=0 )
	{
		time_out.tv_sec= timeout;
		time_out.tv_nsec= 0;
		ret= semtimedop(semaphore_id, &sops, 1, &time_out);
	}
	else
		ret= semtimedop(semaphore_id, &sops, 1, NULL);
		
	err= errno;
	
	if( ret==-1 )
	{
		if(err==EINTR && timeout>0)
		{
			sprintf(err_str, "Wait semaphore %d timeout!\n", semName);
       		Err_log(err_str);
		}
		else
		{
			sprintf(err_str, "Wait semaphore %d error: errno= %d!\n", 
				semName, err);
       		Err_log(err_str);
		}
	}
	return ret;
}



int	ReleaseReadSem(int semName)
{
	char		err_str[128];
	unsigned short	semncnt;
	union semun 
	{
		int	val;
		struct	semid_ds	*buf;
		ushort	*array;
	} arg;
		
	if(-1==semaphore_id)
	{
       	Err_log("Semaphore set no link!\n");	
    	return -1;
	}

	if(semName<0 || semName>=MAX_SEMAPHORE_NUM)
	{
		sprintf(err_str, "No such a semaphore %d in set!\n", semName);
       	Err_log(err_str);	
    	return -1;
	}
	
	if( (semncnt= semctl(semaphore_id, semName, GETNCNT)) == (unsigned short)-1)
	{
		sprintf(err_str, "Fail to get semaphore %d semncnt!\n", semName);
        Err_log(err_str);	
    	return -1;
	}

	arg.val= semncnt;
	return semctl(semaphore_id, semName, SETVAL, arg);
}


int	ReleaseWriteSem(int semName)
{
	char		err_str[128];
	unsigned short	semval;
	struct		sembuf sops;
		
	if(-1==semaphore_id)
	{
       	Err_log("Semaphore set no link!\n");	
    	return -1;
	}

	if(semName<0 || semName>=MAX_SEMAPHORE_NUM)
	{
		sprintf(err_str, "No such a semaphore %d in set!\n", semName);
       	Err_log(err_str);	
    	return -1;
	}
	
	if( (semval= semctl(semaphore_id, semName, GETVAL)) == (unsigned short)-1)
	{
		sprintf(err_str, "Fail to get semaphore %d semval!\n", semName);
        Err_log(err_str);	
    	return -1;
	}
	
	if( semval>0 )
		return 0;

	sops.sem_num= semName;
	sops.sem_op= 1;
	sops.sem_flg= SEM_UNDO;	
	if(-1==semop(semaphore_id, &sops, 1))
	{
		sprintf(err_str, "Fail to release semaphore %d: errno= %d!\n", 
			semName, errno);
        Err_log(err_str);	
    	return -1;
	}
	return 0;
}



int	RemoveShareMemory(char key_char, char *shm_name)
{
	int		shm_id, sem_id, key;
	char	err_str[128];
	char	keyfile[128];

	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	if( (key=ftok(keyfile, key_char))== -1 )
	{
		sprintf(err_str, "Fail to generate %s key: %d", shm_name, errno);
       	Err_log(err_str);
		return -1;
	}

	if( (shm_id=shmget(key, 0, 0660)) == -1 )
	{
		sprintf(err_str, "Fail to shmget %s share memory: %d", shm_name, errno);
        Err_log(err_str);
		return -1;
	}
	if(shmctl(shm_id, IPC_RMID, NULL))
	{
		sprintf(err_str, "Fail to remove %s share memory: %d", shm_name, errno);
        Err_log(err_str);
		return -1;
	}
	return 0;
}

void	ClearEmsEnv(void)
{
	int		shm_id, sem_id, key;
	char	keyfile[128];

	printf("\n\n\n*---*---*---*---*---*start to remove ipcs*---*---*---*---*---*\n\n\n");
	RemoveShareMemory(SHM_REC_KEY_CHAR, "event_record");
	RemoveShareMemory(SHM_SEND_KEY_CHAR, "data_send");
	RemoveShareMemory(SHM_RECV_KEY_CHAR, "data_recv");
	RemoveShareMemory(SHM_NETCMD_KEY_CHAR, "net_cmd");
	RemoveShareMemory(SHM_PLCCMD_KEY_CHAR, "plc_cmd");
	RemoveShareMemory(SHM_PLCDATA_KEY_CHAR, "plc_data");
	RemoveShareMemory(SHM_SGZY_KEY_CHAR, "sgzy");
	RemoveShareMemory(SHM_WDQS_KEY_CHAR, "wdqs");
	RemoveShareMemory(SHM_HDB_KEY_CHAR, "hdb_data");
	RemoveShareMemory(SHM_SYSLOG_KEY_CHAR, "sys_log");
	RemoveShareMemory(SHM_PLCSEND_KEY_CHAR, "plc_send");
	RemoveShareMemory(SHM_FAULT_TABLE_KEY_CHAR, "fault_table");
	RemoveShareMemory(SHM_TREND_KEY_CHAR, "real_curve");
	RemoveShareMemory(SHM_SYSCOMM_TABLE_KEY_CHAR, "sys_comm_table");
	RemoveShareMemory(SHM_INFO_RECORD_KEY_CHAR, "info_record");
	RemoveShareMemory(SHM_OTS_KEY_CHAR, "ots_share");
	RemoveShareMemory(SHM_COMMCMD_KEY_CHAR, "comm_cmd_buf");
	
	/* remove semaphore set */
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	if( (key=ftok(keyfile, (char)SEM_KEY_CHAR))== -1 )
		printf("Fail to generate semaphore key: %d", errno);
	else if( (sem_id=semget(key, MAX_SEMAPHORE_NUM, 0660)) == -1 )
		printf("Fail to semget semaphore set: %d", errno);
	else if(semctl(sem_id,0,IPC_RMID ,0))
		printf("Fail to remove semaphore set: %d", errno);
}



void	CreateLockFile( char *filename )
{
	int		fd, val;
	struct	flock lock;
	char	err_str[128];
	char	name[256];
	char	buf[16];
	
	sprintf(name, "%s%s", SYS_TMP_PATH, filename);
	if( (fd=open(name, O_WRONLY|O_CREAT, 0644)) == -1 )
	{
		sprintf(err_str, "Fail to open %s!\n", name);
       	Err_log(err_str);	
    	exit(-1);
    }	
	
	lock.l_type= F_WRLCK;
	lock.l_start= 0;
	lock.l_whence= SEEK_SET;
	lock.l_len= 0;
	if( fcntl(fd, F_SETLK, &lock) < 0 )
	{
		if(errno==EACCES || errno==EAGAIN)	
        {
			if(strcmp(filename, SYS_LOCK_FILE))
				Err_log("Task run already! exit...\n");
			else	
				Err_log("H9000 system run already! exit...\n");
			exit(0);
		}
		else
        {
        	sprintf(err_str, "Fail to write_lock %s!\n", filename);
			Err_log(err_str);
			exit(-1);
		}
	}

    if( ftruncate(fd, 0) < 0 )
    {
		sprintf(err_str, "Fail to truncate %s!\n", filename);
		Err_log(err_str);
		exit(-1);
    }

    sprintf(buf, "%d\n", getpid());
    if( write(fd, buf, strlen(buf)) != strlen(buf) )
    {
		sprintf(err_str, "Fail to write pid to %s!\n", filename);
		Err_log(err_str);
		exit(-1);
    }

	val= fcntl(fd, F_GETFD, 0);
	if(val<0)
    {
		sprintf(err_str, "Fail to fcntl F_GETFD %s!\n", filename);
		Err_log(err_str);
		exit(-1);
    }

	val |= FD_CLOEXEC;
	if(fcntl(fd, F_SETFD, val)<0)
    {
		sprintf(err_str, "Fail to fcntl F_SETFD %s!\n", filename);
		Err_log(err_str);
		exit(-1);
    }
}




void	InitThreadMutex(pthread_mutex_t *mutex)
{
	pthread_mutex_init(mutex, NULL);
}

void	GetThreadMutex(pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
}

void	ReleaseThreadMutex(pthread_mutex_t *mutex)
{
	pthread_mutex_unlock(mutex);
}

int	NoConsoleSystemCmd(char *cmd, char *ret_info_buff, int buff_size)
{
	system(cmd);
	return 0;
}
#endif /*SUN_OS end*/



#ifdef  WINDOWS_OS /*WINDOWS_OS start*/

void*	CreateShareMemory(char key_char, int size, char *shm_name)
{
	HANDLE		hMapObject;
	char		name[20];
	void		*shm_addr;
	char		err_str[128];

	sprintf(name, "SHM_NAME_%c", key_char);
	hMapObject= CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0, size, name);
	if( hMapObject == NULL )
	{
		sprintf(err_str, "Fail to CreateFileMapping: %s errno=%d\n", shm_name, GetLastError());
		Err_log(err_str);
		return (void*)-1;
	}

	shm_addr = MapViewOfFile (hMapObject,FILE_MAP_ALL_ACCESS,0, 0, 0);  
	if (shm_addr == NULL)
	{
		sprintf(err_str, "Fail to MapViewOfFile: %s errno=%d\n", shm_name, GetLastError());
		Err_log(err_str);
		CloseHandle(hMapObject);
		return (void*)-1;
	}
	return shm_addr;
}


int		UnlinkShareMemory(void *shm_addr, char *shm_name)
{
	char	err_str[128];

	if(UnmapViewOfFile(shm_addr))
		return 0;

	sprintf(err_str, "Fail to UnmapViewOfFile: %s errno=%d\n", shm_name, GetLastError());
	Err_log(err_str);
	return -1;
}



int		CreateSemSet(void)
{
	int		i, init_count, max_count;
	char	err_str[128];
	char	sem_name[20];
	
	for(i=0; i<MAX_SEMAPHORE_NUM; i++)
	{
		sprintf(sem_name, "SEM_NAME_%d", i);
		if(i==SEM_WR_SEND || i==SEM_WR_NETCMD || i==SEM_WR_SYSLOG || i==SEM_WR_COMMCMD)
		{
			init_count= 1;
			max_count= 1;
		}
		else
		{
			init_count= 0;
			max_count= 10000;
		}

		if( CreateSemaphore(NULL, init_count, max_count, sem_name) == NULL )
		{
			sprintf(err_str, "Fail to CreateSemaphore: %s errno=%d\n", sem_name, GetLastError());
			Err_log(err_str);
			return -1;
		}
	}
	return 0;
}



void*	LinktoShareMem(char key_char)
{
	HANDLE	hMapObject;
	void	*shm_addr;
	char	err_str[128];
	char	shm_name[20];
	
	sprintf(shm_name, "SHM_NAME_%c", key_char);
    hMapObject = OpenFileMapping (FILE_MAP_ALL_ACCESS, FALSE, shm_name);
	if (hMapObject == NULL)
	{
		sprintf(err_str, "\nFail to OpenFileMapping: %s errno=%d\n", shm_name, GetLastError());
		Err_log(err_str);
		return (void*)-1;
	}
	shm_addr = MapViewOfFile (hMapObject,FILE_MAP_ALL_ACCESS,0, 0, 0);  
	if (shm_addr == NULL)
	{
		sprintf(err_str, "\nFail to MapViewOfFile: %s errno=%d\n", shm_name, GetLastError());
		Err_log(err_str);
		CloseHandle(hMapObject);
		return (void*)-1;
	}
	CloseHandle(hMapObject);
	return shm_addr;
}


HANDLE	semphore_set[MAX_SEMAPHORE_NUM];

int	LinktoSemSet(int semName)
{
	char	err_str[128];
	char	sem_name[20];
	
	sprintf(sem_name, "SEM_NAME_%d", semName);
	
	if(semName<0 || semName>=MAX_SEMAPHORE_NUM)
	{
		sprintf(err_str, "No such a semaphore %d in set!\n", semName);
        Err_log(err_str);	
    	return -1;
	}

	semphore_set[semName] = OpenSemaphore(SEMAPHORE_MODIFY_STATE|SEMAPHORE_ALL_ACCESS|SYNCHRONIZE,
                              FALSE, sem_name);  
	if(semphore_set[semName] == NULL)
	{
		sprintf(err_str, "\nFail to OpenSemaphore: %s errno=%d\n", sem_name, GetLastError());
		Err_log(err_str);
		return -1;
	}

	return 0;
}


int	WaitforSem(int semName, int timeout)
{
	char	err_str[128];
	DWORD	ret;

	if(semName<0 || semName>=MAX_SEMAPHORE_NUM)
	{
		sprintf(err_str, "No such a semaphore %d in set!\n", semName);
       	Err_log(err_str);	
    	return -1;
	}
	
	if(0==semphore_set[semName])
	{
 		sprintf(err_str, "Semaphore %d no link!\n", semName);
       	Err_log(err_str);	
    	return -1;
	}

	ret= WaitForSingleObject(semphore_set[semName], timeout*1000);
	if(WAIT_OBJECT_0 == ret)
		return 0;
	else if(WAIT_TIMEOUT == ret)
	{
		sprintf(err_str, "Wait semaphore %d timeout!\n", semName);
       	Err_log(err_str);	
    	return -1;
	}
	else
	{
		sprintf(err_str, "Wait semaphore %d error!\n", semName);
       	Err_log(err_str);	
    	return -1;
	}
}


int	ReleaseReadSem(int semName)
{
	char		err_str[128];
		
	if(semName<0 || semName>=MAX_SEMAPHORE_NUM)
	{
		sprintf(err_str, "No such a semaphore %d!\n", semName);
       	Err_log(err_str);	
    	return -1;
	}
	
	if(0==semphore_set[semName])
	{
 		sprintf(err_str, "Semaphore %d no link!\n", semName);
       	Err_log(err_str);	
    	return -1;
	}

	ReleaseSemaphore(semphore_set[semName], 2, NULL);
	return 0;
}


int	ReleaseWriteSem(int semName)
{
	char		err_str[128];
		
	if(semName<0 || semName>=MAX_SEMAPHORE_NUM)
	{
		sprintf(err_str, "No such a semaphore %d!\n", semName);
       	Err_log(err_str);	
    	return -1;
	}
	
	if(0==semphore_set[semName])
	{
 		sprintf(err_str, "Semaphore %d no link!\n", semName);
       	Err_log(err_str);	
    	return -1;
	}

	ReleaseSemaphore(semphore_set[semName], 1, NULL);
	return 0;
}

/*
void	CreateLockFile( char *filename )
{
	char	err_str[128];
	
	if(NULL==CreateSemaphore(NULL, 0, 1, filename))
	{
		sprintf(err_str, "Fail to create lock sem %s!\n", filename);
		Err_log(err_str);
		exit(-1);
	}

	if(ERROR_ALREADY_EXISTS==GetLastError())
	{
		if(strcmp(filename, SYS_LOCK_FILE))
			Err_log("Task run already! exit...\n");
		else	
			Err_log("H9000 system run already! exit...\n");
		exit(0);
	}
}
*/

void	CreateLockFile( char *filename )
{
	HANDLE	hFile;
	char	name[256];
	
	sprintf(name, "%s%s", SYS_TMP_PATH, filename);
	hFile= CreateFile(name, GENERIC_READ|GENERIC_WRITE, 
		0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	if(hFile!=INVALID_HANDLE_VALUE) /*task is not run*/
		return;
	else /*task is running*/
	{
		if(strcmp(filename, SYS_LOCK_FILE))
			Err_log("Task run already! exit...\n");
		else	
			Err_log("H9000 system run already! exit...\n");
		exit(0);
	}	
}



void	ClearEmsEnv(void)
{
}


void	InitThreadMutex(pthread_mutex_t *mutex)
{
	*mutex= CreateMutex(NULL, FALSE, NULL);
}

void	GetThreadMutex(pthread_mutex_t *mutex)
{
	if(*mutex)
		WaitForSingleObject(*mutex, INFINITE);
}

void	ReleaseThreadMutex(pthread_mutex_t *mutex)
{
	if(*mutex)
		ReleaseMutex(*mutex);
}



int	NoConsoleSystemCmd(char *cmd, char *ret_info_buff, int buff_size)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE				hReadPipe,hWritePipe; 
	STARTUPINFO			si; 
	PROCESS_INFORMATION pi;
	int					count, i;

    si.cb= sizeof(STARTUPINFO); 
	GetStartupInfo(&si);
	si.wShowWindow= SW_HIDE; /*隐藏命令行窗口*/
	si.dwFlags= STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	
	if(ret_info_buff)
	{
		sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
		sa.lpSecurityDescriptor = NULL; 
		sa.bInheritHandle = TRUE;
		if(!CreatePipe(&hReadPipe, &hWritePipe, &sa, buff_size))
			return -1;

		si.hStdError= hWritePipe; 
		si.hStdOutput= hWritePipe; 
	}

	/*创建命令行进程*/
	if(!CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi )) 
	{
		if(ret_info_buff)
		{
			CloseHandle(hWritePipe);
			CloseHandle(hReadPipe);
		}
		return -1;
	}

	/*创建成功*/
	if(!ret_info_buff)
	{
		CloseHandle(pi.hProcess); 
		CloseHandle(pi.hThread); 
		return 0;
	}

	count= 0;
	WaitForSingleObject(pi.hProcess, INFINITE); 
	CloseHandle(hWritePipe);
	if(ReadFile(hReadPipe, ret_info_buff, buff_size, &count, NULL))
	{
		for(i=0; i<count; i++)
		{
			if(!ret_info_buff[i])
				ret_info_buff[i]= ' ';
		}
		ret_info_buff[count]= 0;
		CloseHandle(pi.hProcess); 
		CloseHandle(pi.hThread); 
		CloseHandle(hReadPipe);
		return count;
	}
	else
	{
		CloseHandle(pi.hProcess); 
		CloseHandle(pi.hThread); 
		CloseHandle(hReadPipe);
		return -1;
	}
} 


#endif  /*WINDOSW_OS end*/













