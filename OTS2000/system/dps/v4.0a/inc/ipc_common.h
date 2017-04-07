#ifndef	_IPC_COMMON_H
#define	_IPC_COMMON_H

#include	"../../../dms/v4.0a/inc/os_type_def.h"
#include	"../../../dms/v4.0a/inc/dms_structure_def.h"


#ifdef	WINDOWS_OS
typedef	HANDLE	pid_t;	
typedef	HANDLE	pthread_mutex_t;	
#endif




#ifdef __cplusplus
extern "C"{
#endif 

int		CreateEmsEnv(void);
void	ClearEmsEnv(void);
void	CreateLockFile( char *filename );
void*	LinktoShareMem(char key_char);
int		LinktoSemSet(int semName);
int		WaitforSem(int semName, int timeout);
int		ReleaseReadSem(int semName);
int		ReleaseWriteSem(int semName);
void	InitThreadMutex(pthread_mutex_t *mutex);
void	GetThreadMutex(pthread_mutex_t *mutex);
void	ReleaseThreadMutex(pthread_mutex_t *mutex);
int		NoConsoleSystemCmd(char *cmd, char *ret_info_buff, int buff_size);

#ifdef __cplusplus
}
#endif



#define FILE_KEY			".key_file"
#define	SYS_LOCK_FILE		".sys_lockfile"


/*key_char defination*/
enum	KEY_CHAR
{		
		SHM_REC_KEY_CHAR='A',
		SHM_SEND_KEY_CHAR,
		SHM_RECV_KEY_CHAR,		
		SHM_NETCMD_KEY_CHAR,		
		SHM_PLCCMD_KEY_CHAR,		
		SHM_PLCDATA_KEY_CHAR,	
		SHM_SGZY_KEY_CHAR,		
		SHM_WDQS_KEY_CHAR,		
		SHM_HDB_KEY_CHAR,	
		SHM_SYSLOG_KEY_CHAR,		
		SHM_PLCSEND_KEY_CHAR,
		SHM_FAULT_TABLE_KEY_CHAR,
		SHM_TREND_KEY_CHAR,
		SHM_SYSCOMM_TABLE_KEY_CHAR,
		SEM_KEY_CHAR,
		SHM_INFO_RECORD_KEY_CHAR,
		SHM_OTS_KEY_CHAR,
		SHM_COMMCMD_KEY_CHAR	//added by wzg 20110221 for iec comm
};



#define	MAX_SEMAPHORE_NUM	16
/*semaphore name defination*/
enum	SEM_NAME 
{		
		SEM_RECORD, 
		SEM_WR_SEND, 
		SEM_RD_SEND, 
		SEM_RECVDATA, 
		SEM_WR_NETCMD, 
		SEM_RD_NETCMD, 
		SEM_PLCCMD,
		PLCDATA, 
		SEM_SGZY, 
		SEM_WDQS, 
		SEM_HDBCURVE, 
		SEM_WR_SYSLOG,
		SEM_RD_SYSLOG,
		SEM_INFO_REC,
		SEM_WR_COMMCMD,
		SEM_RD_COMMCMD
};

enum	EVENT_SEARCH_TYPE  {EVENT_SEARCH_POINT_ID, EVENT_SEARCH_POINT_NAME,
							EVENT_SEARCH_POINT_LONGNAME};

typedef struct	
{
	BOOL		search_flag;
	UINT		search_type;
	char		search_str[POINT_LONGNAME_SIZE];
	POINTER		pt_id;
	DATE_TIME	start_t;
	DATE_TIME	end_t;
	UINT		event_num;
} EVENT_SEARCH;


typedef struct
{
	struct
	{
		BOOL		poll_flag;
		UCHAR		data_type;
		USHORT		start_pt_id;
		USHORT		pt_num;		
	} data_poll[STATION_ENTRY_NUMBER+1][GROUP_ENTRY_NUM_PER_STATION+1];

	BOOL	dev_alm_disable[STATION_ENTRY_NUMBER+1][GROUP_ENTRY_NUM_PER_STATION+1];
	BOOL	simulate_state;

	EVENT_SEARCH	event_search;

	struct
	{
		int		sysValue; //sysmgr process supervision value 
		BOOL	exitFlag; //sysmgr exit flag
	} watchdog;

} SYS_COMM_TABLE;



#endif /*_IPC_COMMON_H*/

