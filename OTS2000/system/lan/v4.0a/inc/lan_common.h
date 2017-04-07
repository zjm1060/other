/**************************************************************************************************************************
FILENAME:	lan_common.h
FUNCTION:		
	(1) structure definition of 
	(2) external interface function definition such as lan_out()

VERSION:		v4.0a
OPERATE SYSTEM:	window 2000, Unix
HISTORY:
	2005.11.1	start to modify the code for H9000 V4.0		yyp
	
****************************************************************************************************************************/
#ifndef	_LAN_COMMON
#define	_LAN_COMMON   

#include "lan_os.h"
#include "common_head.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef TRUE
 #define TRUE 		1
 #define FALSE  	0
#endif 
#define	YES		1
#define	NO		0	
#define LCU_OFF		0		
#define LCU_ON		1		
#define	LCU_ERR		2

#define HDB_ON		0		
#define HDB_ERR		1		

#define	NET_ERR		1				
#define	NET_OK		0
#define	SCANSTOP	0
#define	SCANRUN		1
#define	CUR_WS_IN_EXIT	0
#define	CUR_WS_IN_HOST	1
#define	CUR_WS_IN_VICE	2
#define CMD_SUCCESS	0
#define CMD_SENDERR	1
#define CMD_CTRLERR	2
#define CMD_FORMATERR	3
#define LOCK_FLAG	1
#define IDLE_FLAG		0				
#define SHM_DATA_SEG_SIZE       1440
#define MAX_NETCMD_SEG_NUM	60
#define SHM_NETCMD_SEG_SIZE	32	
#define MAX_RECV_DATA_SEG_NUM	800
#define MAX_SEND_DATA_SEG_NUM	600
#define MAX_BUF_SIZE    SHM_DATA_SEG_SIZE			
#define MAX_RECV_NUM    MAX_RECV_DATA_SEG_NUM		
#define MAX_SEND_NUM    MAX_SEND_DATA_SEG_NUM
#define	MAX_STN_NUM 	STATION_ENTRY_NUMBER
#define MAX_STN_ID		MAX_STN_NUM					//added by yyp 2011.12.03	
#define	MAX_LCU_NUM	GROUP_ENTRY_NUM_PER_STATION
#define	MAX_PLCDATA_SEG_NUM	120			/* max seg number of plcdata seg,  added by yyp 06.04.02 */
#define	MAX_PLCALL_NUM	(MAX_STN_NUM * MAX_LCU_NUM)		/* max plc number of all stations added for V4.0 by yyp 05/11/03 */
#define	MAX_NET_NUM	2
#define	MAX_SCAN_HOST_NUM	80
#define MAX_WS_HOST_NUM	80
#define MAX_STACK_SIZE	0		/* bytes */
#define DB_NAME		NULL
#define CPU_D_NET_D_2	5	/* for cha ha wu shu total 2 NOE 2007.8.5 */
#define CPU_D_NET_D		4	/* for sx true dual cpu and true dual net  2007.8.5 */
#define CPU_D_NET_S		3	/* for xiaoxi true dual cpu and false dual net 2007.8.5 */
#define CPU_S_NET_D		2	/* 2007.8.5 */
#define CPU_S_NET_S		1	/* 2007.8.5 */
#define	MIXPLC	0
#define TYPE_ONE_SEG	0	/* added by yyp 2011.03.09 */
#define TYPE_ALL_SEG	1

/******************************* datastructure of shared memory of lan module **********************************************/

#define CMD_STATUS COMMAND_STATUS

typedef struct												
{
		unsigned char stnid ;
		unsigned char devid ;
}LCUID;

typedef struct												
{
	LCUID lcu_id;
    unsigned char stn_idx;
    unsigned char dev_idx;
}LCUID_INC_IDX;//cxs

	
typedef struct
{
	USHORT		length;
	UCHAR		dp_type;
	UCHAR		dest_id;
	UCHAR 		src_id;
	UCHAR 		dest_stn;
	USHORT		packet_num;
}LAN_HEAD;

typedef struct 
{
        LAN_HEAD	 lan_head;
        CHAR 		data[SHM_DATA_SEG_SIZE];
        UCHAR		netid;
}SHM_DATA_SEG;

typedef struct
{
	USHORT          head;
	SHORT           flag;
	SHM_DATA_SEG	shm_data_seg[MAX_RECV_DATA_SEG_NUM];
	SHORT			island;
}SHM_RECV_DATA;


typedef struct
{
	USHORT          head;
	SHORT           flag;
	SHM_DATA_SEG	shm_data_seg[MAX_SEND_DATA_SEG_NUM];
}SHM_SEND_DATA;




struct net_cmd_data
{
	unsigned char	type_id;
	unsigned char	stn_id;
	unsigned char	dev_id;
	unsigned char	data_type;
	unsigned short	point_id;
	CMD_STATUS	status;
	int		data;
	unsigned short	ctrl_addr;
	unsigned char	host_id;
	unsigned char	usr_id;
	unsigned char	res;
	unsigned char	second;
	unsigned char	minute;
	unsigned char	hour;
	unsigned char	day;
	unsigned char	month;
	unsigned short	year;
	char	usr_login_name[8];
}; 

struct net_cmd_seg
{
	LAN_HEAD  lan_head;
	union
	{
		unsigned  char  data[SHM_NETCMD_SEG_SIZE];
		struct net_cmd_data netdata;
	}net_cmd_buf;
};

struct net_cmd
{
	short  head;
	short	flag;	
	struct  net_cmd_seg  seg[MAX_NETCMD_SEG_NUM];
};


struct comm_cmd
{
	struct  net_cmd  stn_data[MAX_STN_NUM+1];
};

#define LANH_SIZE       	sizeof(LAN_HEAD)
#define SHM_SEND_DATA_SIZE   	sizeof(SHM_SEND_DATA)
#define SHM_RECV_DATA_SIZE   	sizeof(SHM_RECV_DATA)
#define	NET_CMD_SIZE		sizeof(struct net_cmd)
#define	COMM_CMD_SIZE		sizeof(struct comm_cmd)

typedef struct 
{
	unsigned char	type_id;
	unsigned char 	stn_id;
	unsigned char 	dev_id;
	char    data_type;
	short	number;
	short	point_id;
	short   status;
	short   ms;	  
	unsigned char 	host_id;	
	char    sec;
	char    min;
	char    hour;
	char    day;
	char    month;
	short   year;
} SYS_MSG;

typedef struct 
{
	unsigned char	type_id;
	unsigned char 	stn_id;
	unsigned char 	dev_id;
	char    data_type;
	short	number;
	short	point_id;
	short   status1;
	short	status2;
	short	param;
	short   ms;	  
	unsigned char 	host_id;	
	char    sec;
	char    min;
	char    hour;
	char    day;
	char    month;
	short   year;
} OBJ_MSG;

typedef struct 
{
	unsigned char	type_id;
	unsigned char 	stn_id;
	unsigned char 	dev_id;
	char    data_type;
	short	number;
	short	point_id;
	short   status;
	char	data[4];
	unsigned char 	host_id;	
	char    sec;
	char    min;
	char    hour;
	char    day;
	char    month;
	short   year;
} ANA_MSG;

typedef struct 
{
	unsigned char	type_id;
	unsigned char 	stn_id;
	unsigned char 	dev_id;
	char    data_type;
	short	number;
	short	point_id;
	short   status;
	short	data;
	short   ms;	  
	unsigned char 	host_id;	
	char    sec;
	char    min;
	char    hour;
	char    day;
	char    month;
	short   year;
} COMMON_MSG;

struct wsconf
{
	unsigned	char	stnid;
	unsigned	char	hostid;
	char		name[40];
	unsigned	char	ipnum;	
	char		ipaddr[4][40];
	char		valid;			
};


struct ipc_conf_seg
{
	char hostname[40];
	char stnname[10];
	char devname[20];
	char mbpaddr_p[40];	
	char mbpaddr_s[40];
	unsigned char stnid;
	unsigned char devid;
};
struct ipc_conf
{
	unsigned char num;
	struct ipc_conf_seg seg[MAX_LCU_NUM+1];
};



#define SYSMSG_SIZE       sizeof(SYS_MSG)
#define ANAMSG_SIZE       sizeof(ANA_MSG)
#define COMMONMSG_SIZE    sizeof(COMMON_MSG)
#define COMMAND_SIZE       sizeof(COMMAND_MSG)


/******************************* global variable of lanlib ********************************************************/

extern SHM_SEND_DATA data_send_addr, *shm_send_addr;
extern SHM_RECV_DATA data_recv_addr, *shm_recv_addr;
extern struct	plc_data   	plc_data_addr,*shm_plc_data;
extern struct	plc_cmd		plc_cmd_addr, *shm_plc_cmd;
extern struct	net_cmd		net_cmd_addr, *shm_net_cmd;
extern struct	comm_cmd	comm_cmd_addr, *shm_comm_cmd;
extern struct	plc_send	plc_send_addr, *shm_plc_send;
extern struct shm_ots		data_ots,*shm_ots_addr;
extern BOOL	g_ctrl_dualnet ;
extern BOOL	g_sys_dualnet ;
extern BOOL	g_cascade ;
extern unsigned char g_stnid ;
extern int	g_sys_netnum;
extern int	g_ctrl_netnum;
extern short segid[MAX_STN_NUM+1][MAX_LCU_NUM+1];
extern unsigned char index_stn[MAX_STN_ID+1];
extern unsigned char id_stn[MAX_STN_NUM+1];
extern int g_stn_num;
extern int g_dev_num[MAX_STN_NUM+1];
extern unsigned char g_sys_lcu[MAX_STN_NUM+1];
extern char g_brd_ip[4][40];
extern char g_grp_brd_ip[40];
extern int  g_plc_ip_num[MAX_STN_NUM+1][MAX_LCU_NUM+1];
extern char g_plc_ip[MAX_STN_NUM+1][MAX_LCU_NUM+1][4][40];
extern BOOL g_cpu_net_mode[MAX_STN_NUM+1][MAX_LCU_NUM+1];
extern int g_plcscan_host_num[MAX_STN_NUM+1][MAX_LCU_NUM+1];
extern char g_plcscan_host[MAX_STN_NUM+1][MAX_LCU_NUM+1][MAX_SCAN_HOST_NUM][40];
extern struct wsconf g_ws_host[MAX_STN_NUM+1][MAX_WS_HOST_NUM+1];
extern char g_hostname[100];
extern unsigned char g_localhost_id;
extern BOOL g_is_island;
extern BOOL g_is_grp_brd;
extern int	g_is_simuserver;
extern char sms_buf[200];
extern int app_id ;
extern char g_sid[50];
extern int g_loglevel;
extern int g_ipwithcpu;
extern int g_ipaddone;
extern int g_updatefirst;
extern BOOL g_wranyhost;
extern BOOL g_wrcond;
extern BOOL g_wroffset;
extern BOOL g_ctrl_net_independ;
extern BOOL g_plc_mode;
extern BOOL g_sync_time;
extern BOOL g_simumode;
//add by wayne
extern char g_hostname_mysql[50];

extern struct ipc_conf	ipcconf;
extern char g_mbpaddr[100];

/******************************* common api function of  lanlib   ********************************************************/
BOOL	lan_out(char *buff, LAN_HEAD head, unsigned char netid);		
						/* broadcast one record, buff: data to be sended. head: package head 
						   netid: which net to broadcast,1--NETA 2--NETB OTHER--both */
void wsa_startup();
int	lan_init_all();
int	lan_out_init ();
int	get_lan_shm(unsigned char shmid);	/* initialization for lan module,get the lan 
						   shared memory segment and attach it */
void	lan_finish();						
int	get_lcu_shm(unsigned char shmid);	/* get the lcu shared  memory segment and attach it */
int	get_rec_shm();
int	get_hdb_shm();
int	get_ots_shm();

		
int	WriteIndDBByStr(char * name, short status);
int	GetPlcTypeStr(unsigned char stnid,unsigned char devid, char * typestr);
void	sig_termm(int);
int	InitPlcDataShm( unsigned char type );	/* init plc data shm, type: 1. set seg stnid and devid   0. unset */
int	MySleep(int);				/* sleep some milliseconds */ 
int	BdcastMsg(unsigned char,unsigned char, char,short,short);
int	BdINDByID(POINTER pt,short status);
int	BdINDForceByID(POINTER pt,short status);

int	BdANAByID(POINTER pt,float value);
int BdObjByID(POINTER pt,short status);
int BdObjForceByID(POINTER pt,short status);
int	BdINDByName(char* logicalname,short status);
int	BdANAByName(char* logicalname,float value);
int	WrANA ( char *db_name, float value, short status );
int	WrIND ( char *name, short status );
short 	BdLCUONOFFStat(LCUID plcid,short state);
short	GetLCUONOFFStat(LCUID plcid, short* state);
short 	BdPlcLinkStat(LCUID plcid,short state);
short	GetPlcLinkStat(LCUID plcid, short* state);
short	BdPlcScanStat(LCUID plcid,short state);
short	GetPlcScanStat(LCUID plcid, short* state);
short	BdPlcMonHost(unsigned char stnid);
short	BdScanHost(LCUID plcid);
short GetScanHost(LCUID plcid, short* state);
int BdNetWkQlt(LCUID plcid,short status);
int BdCpuWkQlt(LCUID plcid,short status);
int BdCpuStatQlt(LCUID plcid,short status);
short BdLCUONOFFStatQlt(LCUID plcid,short state);
short BdPlcLinkStatQlt(LCUID plcid,short state);
short BdPlcScanStatQlt(LCUID plcid,short state);
short BdHdbStat(short state);
int GetPtQlt(POINTER pt,short *p_qlt);
int BdcastQlt(POINTER pt,short qlt_state);
int BdcastQltANA(POINTER pt,short qlt_state);
int BdcastQltWithSta(POINTER pt, short rt_state,short qlt_state);
int BdcastQltObj(POINTER pt,short qlt_state);
int BdcastForce(POINTER pt,short force_state);
int BdcastForceObj(POINTER pt,short force_state);
short	WriteScanHost(LCUID plcid,char *host_name);
int	BdSms(char* sms);
int BdLms(short lms_id);
int	BdNetHost(LCUID,short );
int	BdCpuHost(LCUID,short );
int BdCmdSendResult(struct net_cmd_seg,unsigned short);
int	GetSrcId(unsigned char * src_id);	/* broadcast one db point's status in intranet */
int	rdb_net(unsigned char,unsigned char);
int	rdb_cpu(unsigned char,unsigned char);
unsigned char GetCmdBufId(unsigned char src_id);
int	GetEmsConfig();
void	ReadAppDefFile();
#ifdef MSWIN32
 void	* CreateShm(char *name, int size );
 int	CreateAllShm();
#endif
int ClearCommShm(unsigned char stn_id,int id,int type);
int GetIPCConf();
int GetIdByHostname(char *hname,unsigned char *p_stnid,unsigned char *p_devid);


extern struct shm_hdb data_hdb,*shm_hdb_addr;

int	get_hdb_shm();
int	get_rec_shm();

#ifdef __cplusplus
}
#endif

#endif


