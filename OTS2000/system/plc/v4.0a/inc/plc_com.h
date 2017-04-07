/**************************************************************************************************************
modified by yyp on 2005.11.1 for H9000 V4.0

 note when using MSVC/C++ 5.0 - create workspace as a win32 console app and
 change project/settings/link Object/library modules list to include wsock32.lib

Modifying History:
2005.11.1	start to modify the main code yyp
**************************************************************************************************************/
#ifndef	_PLC_COM
#define	_PLC_COM

#ifdef __cplusplus
extern "C" {
#endif

typedef struct	
{
	unsigned char stnid;
	unsigned char devid;
	unsigned char thrtype;	/* thread type */
}THRARG;			/* thread argument structure */
#define		CPUA		0
#define		CPUB		1
#define		CPUSTOP		0
#define		CPURUN		1
#define		CPUBACKUP	2
#define		CPUERR		3
#define		NETA		0
#define		NETB		1
#define		GEPLC		1
#define		MBPLC		2
#define		PREPLC		3
#define		SIEPLC		4
#define		ABPLC		5
#define		UNKNOWPLC	20
#define		MAXRETRY_CMD	1
#define		MAXRETRY	1
#define		MAXRETRY_CONN	3
#define		MAXRETRY_HEAD	3
#define		MAXRETRY_CHILD	1
#define		MAXRETRY_QLT	10
#define		MAXSOELOOP	5		/* read plc ana data until reading soe loops match */
#define		MAX_CMD_LOOP	2000
#define		CUR_CMD_HOST	0		/* send cmd host */
#define		CUR_CMD_VICE	1		/* send cmd vice */
#define		THR_HEAD	1		/* thread define added by yyp 06.02.20 */
#define		THR_ANA		2
#define		THR_IMP		3
#define		THR_SYSMSG	4
#define		THR_INTALM	5
#define		THR_WRITEDATA	6
#define		THR_CMD		7
#define		THR_POL		8

#define		READ_0X		1			/* Function Code of Modbus */
#define		READ_1X		2
#define		READ_3X		4
#define		READ_4X		3
		
#define		MAX_QUERY_SIZE	125		/* max query word num of modbus PLC */
#define		MAX_RECV_SIZE	260		/* max size of mobus TCP recv byte */
#define		MAX_QUERY_SIZE_GE	1024		/* max query word num of GE PLC */
#define		MAX_QUERY_SIZE_AB	200		/* max query word num of GE PLC */

#define		READ_STATUS	0
#define		WRITE_STATUS	1
#define		EXCPT_STATUS	2
#define		RECV_FLAGS      (0)
#define		ICMP_TIMEOUT_S	2		/* icmp sock recv/send timeout : sec */
#define		ICMP_TIMEOUT_MS	0		/* icmp sock recv/send timeout : msec */
#define		SOCK_TIMEOUT_S	2		/* sock recv/send timeout : sec */
#define		SOCK_TIMEOUT_MS	500		/* sock recv/send timeout : msec. note: must be less than 1000 */
#define		SOCK_TIMEOUT_MS_0X	900		/* sock recv/send timeout 0X : msec. note: must be less than 1000 */
#define		NODELAY		0	
#define		CONN_TIMEOUT_S	0		/* sock conn timeout : sec */
#define		CONN_TIMEOUT_MS	500		/* sock conn timeout : msec */
#define		TIMEOUT_ALARM	2		/* sock alarm timeout : sec */
#define		MAXIPNUM		300

#define		CMDDB_NO    1			/* defined for siemens below */
#define		HEADDB_NO	2
#define		ANADB_NO    3
#define		SOEDB_NO    3
#define		SOEALMDB_NO	4
#define		POLDB_NO    5
#define		PAMDB_NO    6
#define		OBJDB_NO    7
#define		OBJALMDB_NO	8
#define		SENDDB_NO	9
#define		SYSDB_NO    10
#define		WRDDB_NO    9
#define		SEND_SIZE_GE	56		/* for GE plc */
#define		PORT_GE		18245
#define		PLCMON_MULTITHREAD
#define		PLCMON_MULTIDEV

struct hostconf
{
	unsigned	char	stnid;
	unsigned	char	devid;
	char		name[40];
	unsigned	char	ipnum;	
	char		ipaddr[4][40];
	unsigned	char	scan_host_num;
	char		scan_host[MAX_SCAN_HOST_NUM][40];
	unsigned	short port;		/* TCP port, 18245 for geplc,502 for quantum	*/
	char		typestr[12];		/* plc type name,"GE" or "QUANTUM" or PREMIUM	05/11/03 by yyp*/
	unsigned	char	plctype;	/* plc type id, GEPLC or MBPLC or PREPLC */
	unsigned	char	cpu_net_mode;
	BOOL		isdualcpu;
	char		valid;			/* the dev is defined in 'hosts.def' or not, 05/11/03 by yyp*/
};
                                                                                                            
struct plcvar
{
	unsigned	short	cmdptr;
	unsigned	short	netcmdptr;
	unsigned	char hotnet;
	unsigned	char hotcpu;
	unsigned	char cpua_state;
	unsigned	char cpub_state;
	int		readslave;
	int		timeout;				/* alarm timeout : YES or NO */
	unsigned	short		soeloopcount;				/* read soe loop times */
	unsigned	short		polloopcount;				/* read pol loop times */
	unsigned	short		cpu_flag;
	unsigned	short		cpu_flag_count;				
	unsigned	short		pol_ptr_end;				/* pointer of pol package */
	unsigned	short		soealm_ptr_end; 			/* pointer of int Alarm */
	unsigned	short		objalm_ptr_end; 			/* pointer of int Alarm */
	unsigned	short		plc_run_flag; 				/* the flag number  */
	unsigned	short		FirstIntAlm;
	unsigned	short		FirstHead;
	unsigned	short		FirstCmd;
	unsigned	short		FirstSend;				/* first run of read head data function  */
	unsigned	short		timeloopcount;
	unsigned	short		timecmdloopcount;
	unsigned	short		send_update_flag;
	unsigned	short		param_update_flag;
};


typedef struct _MYSOCKET
{
	SOCKET sock;
	int session;
	BOOL readslave;
}MYSOCKET;

/* ICMP header structure */

typedef struct _icmphdr
{
    char      i_type;
    char      i_code;        /* Type sub code */
    USHORT    i_cksum;
    USHORT    i_id;
    USHORT    i_seq;
    ULONG timestamp;
}IcmpHeader;


typedef struct _ipdef
{
	char ip[16];
	unsigned char stnid;
	unsigned char devid;
	unsigned char pos;
	char logicalname[50];
	char status;
	time_t lasttime;
}IpDef;

int	Swap2Head(struct plc_data_seg_head *p_head);
int	Swap2Str(unsigned char *buf,int wordlen);
int	PrintHead(struct plc_data_seg_head head);

int	GetPlcConf(int type);
short	GetPLCScanPrvg(LCUID plcid,int *err);
short   GetPLCMonPrvg(unsigned char stnid);
short	GetPLCCmdPrvg(LCUID plcid);
short	GetHostLinkStat(char * hostname);
short	GetHostNetStat(char * hostname);
short	GetDevLinkStat(LCUID plcid);
short	GetDevONOFFStat(LCUID plcid);
int	GetHeadStat(struct plc_data_seg_head head, unsigned char stnid,unsigned char devid);
SOCKET	xnet_select(SOCKET s, int sec, int usec, short x);
int	IsCmdSendPlc(LCUID plcid, unsigned char);
short	BdPlcNetStat(LCUID plcid,unsigned char cpuid, unsigned char netid, short state);
short	BdCommStat(LCUID,short);
short	BdPlcCpuStat(LCUID plcid,unsigned char cpuid, short state);
short	BdPlcCpuStatQlt(LCUID plcid,unsigned char cpuid, short state);
int	BdAllCmdErr(int type,LCUID plcid);
int	BdCurCmdErr(LCUID plcid);
int	Mod16(int len);
void	Timer();
void FlushSock(SOCKET s);



int g_sock;

int		timeout;				/* alarm timeout : YES or NO */
struct		hostconf	plconf[MAX_STN_NUM+1][MAX_LCU_NUM+1];
struct		plcvar		runvar[MAX_STN_NUM+1][MAX_LCU_NUM+1];
char 		hostname[20];
int	 IsPlcmonHost;
short	FirstLoop;

int	MyStrCmp(char * str1,char * str2);
void	GetStationInfo();

#ifdef __cplusplus
}
#endif

#endif


