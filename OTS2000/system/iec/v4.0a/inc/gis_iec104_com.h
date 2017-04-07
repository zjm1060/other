#include 	<sys/types.h>
/*#include	<term.h>
*/
#include 	<string.h>                      
#include 	<stdio.h>
#include 	<signal.h>
#include	<stdlib.h>
#include 	<fcntl.h>
#ifdef SUN_OS
#include 	<xti.h>
#include	<unistd.h>
#include 	<sys/shm.h>
#include 	<sys/sem.h>

#include <sys/socket.h>  /* for AF_INET        */
#include <netinet/in.h>  /* for struct in_addr */
#include <arpa/inet.h>   /* for inet_ntoa()    */
#include <netdb.h>       /* for struct hostent */
#include <sys/errno.h>
#endif
/* #include	"/DBINC/dms_com.h" */
#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"gis_iec_cmm_def.h"



#define DEBUG


#ifdef SUN_OS
#include <errno.h>	/*added 2003.4.11 zy */
#include <sys/wait.h>
#include	<sys/time.h>
#define Sleep(ms)	usleep(ms*1000)
#else
#define	 sleep(sec)		Sleep(sec*1000)
#include <time.h>
#endif

#include    "../../../dps/v4.0a/inc/ipc_common.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/dps_functions_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"


#define	SCAN_PERIOD	300	/*300 minutes */
#define	CYCLE_PERIOD	20	/*20 seconds */
#define	CMD_TIMEOUT	20	/*20 seconds */

#define	CMM_ADRESS	1
#define	SRC_ADRESS	0
#define	RESOLUTE	0x7FFF

#define	STN_DEF_FILE	"IEC104_STATION.DEF"
#define CONFIG_FILE		"IEC104_CONFIG.DEF"
#define SPDEF_FILE		"SP_DATA.DEF"
#define DPDEF_FILE		"DP_DATA.DEF"
#define STDEF_FILE		"ST_DATA.DEF"
#define BODEF_FILE		"BO_DATA.DEF"
#define MEDEF_FILE		"ME_DATA.DEF"
#define ITDEF_FILE		"IT_DATA.DEF"
#define SCDEF_FILE		"SC_CTRL.DEF"
#define DCDEF_FILE		"DC_CTRL.DEF"
#define RCDEF_FILE		"RC_CTRL.DEF"
#define SEDEF_FILE		"SE_CTRL.DEF"
#define NR9698_COMM_PT_FILE		"9698COMM.DEF"

#define	IEC_HOMEPATH		"/home/ems/h9000/gis_iec104/"
#define	IEC_LOGPATH			"/home/ems/h9000/his/trap/"
#define COM_SEND_SUCCESS		"GIS104:√¸¡Óœ¬¥Ô≥…π¶"
#define COM_CTRL_TERM			"GIS104:√¸¡Ó÷¥––Ω· ¯"
#define COM_CTRL_FAIL			"GIS104:√¸¡Ó÷¥–– ß∞‹"
#define COM_CTRL_TIMEOUT		"GIS104:√¸¡Ó÷¥––≥¨ ±"	


#define ALL_CHANNEL_NUM		4

#define	YES		1
#define	NO		0
#define	DOG_PERIOD	10
#define	BUFSIZE		1024
#define	LINE_BUF_CHAR	256

#define	MAX_ADDR_NUM	4
#define	MAX_PROC_NUM	25
#define	MAX_YC_NUM	4000
#define	MAX_YX_NUM	6000

#define	true	(int)1
#define	false	(int)0

#define	ONLINE        0x00
#define	OFFLINE       0x04
#define	EXIT          0x08

#define	MAINHOST      0x00
#define	VICEHOST      0x04


/************************************************/
/*              for iec60870-5-104              */
/************************************************/
#define	TESTFR_ACK		0x80
#define	TESTFR_ACT		0x40
#define	STOPDT_ACK		0x20
#define	STOPDT_ACT		0x10
#define	STARTDT_ACK		0x08
#define	STARTDT_ACT		0x04

#define	I_APCI			0x00
#define	S_APCI			0x01
#define	U_APCI			0x03

#define	t0	30	/* the timeout for net connect */
#define	t1	500 //ms //15	/* send or test APDU timeout */
#define	t2	10	/* ACK timout when no data transmitting. t2<t1 */
#define	t3	500 //ms //20	/* the timeout for send test frame at link free status */

#define	K	12	/* at send direction, the max number of un ack APDU */
#define	W	8 	/* at recieve direction, befor ack, the max number of recieved APDU */
//#define	W	5   // add by zwj 090624

/* TYPE IDENTIFICATION define: */
#define M_SP_NA		1		/*single point without time tag (class 1, IC) */
#define M_SP_TA		2		/*single point with time tag (SOE) */
#define M_DP_NA		3		/*double point without time tag (class 1, IC) */
#define M_DP_TA		4		/*double point with time tag (SOE) */
#define M_ST_NA		5		/*step point without time tag (class 2, IC) */
#define M_ST_TA		6		/*step point with time tag */
#define M_BO_NA		7		/*slave station remote terminal status */
#define M_BO_TA		8		/*Bitstring of 32 bit with time tag */

#define M_ME_NA		9		/*measure value without time tag (class 2, IC) */
#define M_ME_TA		10		/*measure value with time tag */
#define M_ME_NB		11		/*measure value without time tag,scaled */
#define M_ME_TB		12		/*measure value with time tag,scaled */
#define M_ME_NC		13		/*measure value without time tag,short float */
#define M_ME_TC		14		/*measure value with time tag,short float */

#define M_IT_NA		15		/*integrated totals without time tag */
#define M_IT_TA		16		/*integrated totals with time tag */

#define M_PS_NA		20		/*status detected block sp information (class 1, IC) */
#define M_ME_ND		21		/*measure value, normalized without quality descriptor (class 2, IC) */

#define M_SP_TB		30		/*Single point with time tag CP56Time2a (SOE) */
#define M_DP_TB		31		/*Doule point with time tag CP56Time2a (SOE) */
#define M_ST_TB		32		/*Step point with time tag CP56Time2a */
#define M_BO_TB		33		/*Bitstring of 32 bit with time tag CP56Time2a */
#define M_ME_TD		34		/*Measure value with time tag CP56Time2a */
#define M_ME_TE		35		/*Measure value with time tag CP56Time2a,scaled */
#define M_ME_TF		36		/*Measure value with time tag CP56Time2a,float value */
#define M_IT_TB		37		/*Integrated totals with time tag CP56Time2a */

#define M_EI_NA		70		/*end of initialization */

#define M_BD_NA		232		/*BCD code water level (class 2, IC) */

#define C_SC_NA		45		/*single point control command */
#define C_DC_NA		46		/*double point control command */
#define C_RC_NA		47		/*regulating step command */
#define C_SE_NA		48		/*set point command */
#define C_SE_NB		49		/*set point command */
#define C_SE_NC		50		/*set point command floating point value */
#define C_BO_NA		51		/*Bitstring of 32 bit */

#define C_IC_NA		100		/*interrogation command */
#define C_CI_NA		101		/*counter interrogation command */
#define C_RD_NA		102		/*read command */
#define C_CS_NA		103		/*clock synchronization command */
#define C_TS_NA		104		/*test command */
#define C_RP_NA		105		/*Reset process command */
#define C_CD_NA		106		/*Delay acquire command */

#define P_ME_NA		110		/*parameter for measurements,normalized */
#define P_ME_NB		111		/*parameter for measurements,scaled */
#define P_AC_NA		113		/*parameter activation */

#define C_SE_ND		136		/*multi set point command normalized value */

/* cause of transmit */
#define	PER_CYC		1		/*cyclic transmission */
#define	BACK		2
#define	SPONT		3		/*spontaneous transmission */
#define	INIT		4
#define	REQ		5
#define	ACT		6		/*activation C direction */
#define	ACTCON		7		/*activation confirmation M direction */
#define	DEACT		8		/*deactivation C direction */
#define	DEACTCON	9		/*deactivation confirmation M direction */
#define	ACTTERM		10		/*activation terminal C direction */
#define	INTROGEN	20		/*interrogation C direction */
#define	INTRO1		21		/*respond interrogation group 1 SP/DP */
#define	INTRO2		22		/*respond interrogation group 2 SP/DP */
#define	INTRO3		23		/*respond interrogation group 3 SP/DP */
#define	INTRO4		24		/*respond interrogation group 4 SP/DP */
#define	INTRO5		25		/*respond interrogation group 5 SP/DP */
#define	INTRO6		26		/*respond interrogation group 6 SP/DP */
#define	INTRO7		27		/*respond interrogation group 7 SP/DP */
#define	INTRO8		28		/*respond interrogation group 8 SP/DP */
#define	INTRO9		29		/*respond interrogation group 9 ME */
#define	INTRO10		30		/*respond interrogation group 10 ME */
#define	INTRO11		31		/*respond interrogation group 11 ME */
#define	INTRO12		32		/*respond interrogation group 12 ME */

#define	REQCOGCN	37		/*respond integrated Total */
#define	REQCO1		38		/*respond integrated Total group 1 */
#define	REQCO2		39		/*respond integrated Total group 2 */
#define	REQCO3		40		/*respond integrated Total group 3 */
#define	REQCO4		41		/*respond integrated Total group 4 */

#define	UKTYPE		44		/*unknown type id */
#define	UKCOT		45		/*unknown cause of transmit */
#define	UKASDUAD	46		/*unknown ASDU common address */
#define	UKINFOAD	47		/*unknown information address */

#define FIXFRAMELEN	6
#define VARFRAMELEN	288
#define MINFRAMELEN	6

#define MAX_BUF_SIZE    SHM_DATA_SEG_SIZE
/*#define MAX_RECV_NUM    SHM_DATA_SEG_COUNTER
*/
#define CMD_ENTRY_NUMBER	10
#define CMD_DATA_SIZE		16
#define ASDU_ENTRY_NUMBER	100
#define ASDU_DATA_SIZE		288

#define	MAX_SP_NUM		20000
#define	MAX_DP_NUM		20000
#define	MAX_ME_NUM		10000
#define	MAX_ST_NUM		32
#define	MAX_BO_NUM		32
#define	MAX_IT_NUM		64

#define	MAX_SC_NUM		100
#define	MAX_DC_NUM		512
#define	MAX_RC_NUM		100
#define	MAX_SE_NUM		256
/*
#define	SC_CMD_BASE_ADDR	0xB01
#define	DC_CMD_BASE_ADDR	0xB01
#define	RC_CMD_BASE_ADDR	0xB01
#define	SE_CMD_BASE_ADDR	0xB81
#define	BC_CMD_BASE_ADDR	0xCA1
*/
/* control 6001H--6100H 24577-24832(256) */
#define	SC_CMD_BASE_ADDR	0x6001
#define	DC_CMD_BASE_ADDR	0x6001

/*RC control 6101H--6200H 24833-25088(256) */
#define	RC_CMD_BASE_ADDR	0x6101

/* setpoint 6201H--6400H 25089-25600(512) */
#define	SE_CMD_BASE_ADDR	0x6201
#define	SE_CMD_SCALE_ADDR	0x6201
#define	SE_CMD_FLOAT_ADDR	0x6201

#define	BC_CMD_BASE_ADDR	0xCA1

/*system status 0001H--0020H 1-32(32) */
#define	SYS_BASE_ADDR	0x0001

/*indication status 0021H--4000H 33-16384(16352) */
/*#define	SP_BASE_ADDR	0x0021 */
#define	SP_BASE_ADDR	0x0001
#define	DP_BASE_ADDR	0x0001

/*measurement 4001H--5000H 16385-20480(4096) */
#define	ME_BASE_ADDR	0x4001
#define	ME_SCALE_ADDR	0x4001
#define	ME_FLOAT_ADDR	0x4001
#define	CYC_BASE_ADDR	0x4800
#define	CYC_SCALE_ADDR	0x4800
#define	CYC_FLOAT_ADDR	0x4800

/*parameter 5001H--6000H 20481-24576(4096) */
#define	PM_BASE_ADDR	0x5001

/*integrated total 6401H--6600H 25601-26112(512) */
#define	IT_BASE_ADDR	0xC01

/*step position 6601H--6700H 26113-26368(256) */
#define	ST_BASE_ADDR	0xC81

#define	BO_BASE_ADDR	0xCA1

#define	IC_SP_GROUP_NUM	64
#define	IC_ME_GROUP_NUM	32
#define	CI_GROUP_NUM	32

#define	ME_GROUP_NUM	32
#define	SP_GROUP_NUM	64
#define	PS_GROUP_NUM	480
#define	DP_GROUP_NUM	64
#define	IT_GROUP_NUM	16
#define	ST_GROUP_NUM	32

#define	SOE_NUM		128
#define	SP_REC_NUM	128
#define	DP_REC_NUM	128

#define	NAME_SIZE	36
#define	CMDLINKERR	88	/*Channel fail database point address */

#define MAX_IEC104_IPADDR_NUM	4
typedef struct
{	char	NameID[64];
	char	ip[MAX_IEC104_IPADDR_NUM][64];
	int		ip_num;
#ifdef WINDOWS_OS
	PROCESS_INFORMATION		pi;
#else
	int		Pid;
#endif
}PROC_REC;

typedef struct         /* command data structure ASDU */
{
	unsigned short	pnt;
	unsigned short	length[CMD_ENTRY_NUMBER];
	unsigned char	data[CMD_ENTRY_NUMBER][CMD_DATA_SIZE];
}ASDUCOMDDATA;

typedef struct         /* received data structure ASDU */
{
	unsigned short	pnt;
	unsigned short	length[ASDU_ENTRY_NUMBER];
	unsigned char	data[ASDU_ENTRY_NUMBER][ASDU_DATA_SIZE];
}ASDUDATA;

typedef struct 
{
	unsigned char	type_id;
	unsigned char 	stn_id;
	unsigned char 	dev_id;
	char    data_type;
	short	number;
	short	point_id;
	IND_DATA   status;
	short   ms;	  
	unsigned char 	host_id;	
	char    sec;
	char    min;
	char    hour;
	char    day;
	char    month;
	short   year;
} SW_MSG;

typedef struct 
{
	unsigned char	type_id;
	unsigned char 	stn_id;
	unsigned char 	dev_id;
	char    data_type;
	short	number;
	short	point_id;
	IND_DATA   status;
	unsigned char 	host_id;	
	char    sec;
	char    min;
	char    hour;
	char    day;
	char    month;
	short   year;
} ALL_SW_MSG;
/*
typedef struct
{
	POINTER		ptr;
	ANLG_DATA	value;
}ANA_FMT;
*/
typedef	struct
{
	USHORT	number;
	char	name[MAX_SP_NUM][POINT_NAME_SIZE];
	int		asdutype[MAX_SP_NUM];
} SPDEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_DP_NUM][POINT_NAME_SIZE];
	int		asdutype[MAX_DP_NUM];
} DPDEF;

typedef	struct
{
	USHORT	number;
	float	lorange[MAX_ME_NUM];
	float	hirange[MAX_ME_NUM];
	float	scale[MAX_ME_NUM];
	float	databand[MAX_ME_NUM];
	int		asdutype[MAX_ME_NUM];
	int		timetag[MAX_ME_NUM];
	char	name[MAX_ME_NUM][POINT_NAME_SIZE];
} MEDEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_ST_NUM][POINT_NAME_SIZE];
} STDEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_IT_NUM][POINT_NAME_SIZE];
} ITDEF;

#define MAX_OPR_NUM	4
typedef	struct
{
	USHORT	number;
	unsigned char	cmdtype[MAX_SC_NUM];
	unsigned char	optype[MAX_SC_NUM][MAX_OPR_NUM];
	char	name[MAX_SC_NUM][POINT_NAME_SIZE];
} SCDEF;

typedef	struct
{
	USHORT	number;
	POINTER	ptr[MAX_DC_NUM];
	unsigned char	cmdtype[MAX_DC_NUM];
	unsigned char	optype[MAX_DC_NUM][MAX_OPR_NUM];
	char	name[MAX_DC_NUM][POINT_NAME_SIZE];
} DCDEF;

typedef	struct
{
	USHORT	number;
	POINTER	ptr[MAX_SE_NUM];
	float	lorange[MAX_SE_NUM];
	float	hirange[MAX_SE_NUM];
	unsigned char	cmdtype[MAX_SE_NUM];
	char	name[MAX_SE_NUM][POINT_NAME_SIZE];
} SEDEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_RC_NUM][POINT_NAME_SIZE];
} RCDEF;

typedef	struct
{
	unsigned char	tpyeId;
	unsigned char	cot;
	unsigned short	infoaddr;
	unsigned char	stat;
	unsigned char	ack_flag;
	unsigned char	selected;
	unsigned char	term_flag;
	time_t		sel_tim;
	unsigned char	ackdata[CMD_DATA_SIZE];
} CMDBUF;

typedef	struct
{
	unsigned char	tpyeId;
	unsigned char	cot;
	unsigned short	infoaddr;
	unsigned char	qoc;
	unsigned char	ack_flag;
	unsigned char	term_flag;
	unsigned char	infonum;
	unsigned short	setvalue;
	float		fvalue;
	unsigned int	rcvpnt;
} SEBUF;


#define	MAX_SOE_REC_NUM		5000
typedef struct  
{
	SW_MSG	soe[MAX_SOE_REC_NUM];
	time_t	recv_time[MAX_SOE_REC_NUM];
	int		num;
} SOE_REC_BUFF;


typedef struct  
{
	char stn_name[64];
	char ipaddr[64];
	int ip_no;
	int	ctrl_priv;
	int getCtrlPrivFlag;
} COMM_INFO;



typedef struct  
{
	unsigned short	nASDUId;
	unsigned char	nSrcId;
	int	Gps;		/* 0: Without GPS; 1: With GPS */
	int	GiType;		/* General interrogation type> 1: M_SP_NA; 20: M_PS_NA */
	int	MeASDUType;	/* Me ASDU type> 0: M_ME_NA; 1: M_ME_ND; 3: M_ME_NC */
	int	MeType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
	int	SEType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
	int	sp_cos;		/* sp_cos> 0: sp/dp transmit once; 1: twice */
	int	Cmd_Type;	/* 0: single point ctrl C_SC_NA; 1: double point ctrl C_DC_NA */
	int	Cmd_Sel;	/* 0: cmd without select; 1: cmd with select */
	
	time_t	scan_period;
	time_t	cmd_period;
	
	unsigned short	sp_base_addr;
	unsigned short	dp_base_addr;
	unsigned short	me_base_addr;
	unsigned short	me_scale_addr;
	unsigned short	me_float_addr;
	unsigned short	cyc_base_addr;
	unsigned short	cyc_scale_addr;
	unsigned short	cyc_float_addr;
	unsigned short	pm_base_addr;
	unsigned short	it_base_addr;
	unsigned short	st_base_addr;
	unsigned short	bo_base_addr;
	
	unsigned short	sc_cmd_base_addr;
	unsigned short	dc_cmd_base_addr;
	unsigned short	rc_cmd_base_addr;
	unsigned short	se_cmd_base_addr;
	unsigned short	se_cmd_scale_addr;
	unsigned short	se_cmd_float_addr;
	unsigned short	bc_cmd_base_addr;
	
	SPDEF		spdef;
	DPDEF		dpdef;
	MEDEF		medef;
	DCDEF		dcdef;
	SEDEF		sedef;
	ITDEF		itdef;
	STDEF		stdef;
	SCDEF		scdef;
	RCDEF		rcdef;

	unsigned int	byte_to_send;	
	unsigned int	send_byte;
	int				sock;
	short			cmd_end;
	int				timeout;
	int				cmd_ack;
	unsigned char	comdflag;
	unsigned short	nr_recv,nr_send,nr_ack,ns_ack,nr_rs;

	unsigned char recvbuf[1024];
	unsigned char sendbuf[1024];
	unsigned char cmdsavbuf[36];

	
	CMDBUF			cmdbuf;
	SEBUF			sebuf;
	ASDUCOMDDATA	send_data;
	ASDUDATA		recv_data;
	SOE_REC_BUFF	soe_rec_buff;
} IEC104_CMM_INFO;





int MakeComm(IEC104_CMM_INFO *info, char *STName, COMM_INFO *comm_info);
void* MakeLink(void *para);
int IEC870_5_Recv(IEC104_CMM_INFO *info, COMM_INFO *comm_info, char *pName,unsigned int recvdatapt);
int IEC870_5_Send(IEC104_CMM_INFO *info, COMM_INFO *comm_info);
BOOL GetCtrlPrvg(void);

float Datcvt(short status,unsigned short data);
ANLG_DATA FloatToAnaData(float data);
void InitData(IEC104_CMM_INFO *info, char *NameID, int ip_no);

int StartDT(IEC104_CMM_INFO *info);
int ACKStartDT(IEC104_CMM_INFO *info);
int StopDT(IEC104_CMM_INFO *info);
int ACKStopDT(IEC104_CMM_INFO *info);
int TestFR(IEC104_CMM_INFO *info);
int ACKTestFR(IEC104_CMM_INFO *info);
int SFormatFR(IEC104_CMM_INFO *info);
int	CommandExec(IEC104_CMM_INFO *info);
int SinglePointCtrl(char se,USHORT address,char stat,IEC104_CMM_INFO *info);
int DoublePointCtrl(char se,USHORT address,char stat,IEC104_CMM_INFO *info);
int RegulatStepCtrl(char se,USHORT address,char stat,IEC104_CMM_INFO *info);
int SetPointCmdNA(char se,USHORT address,USHORT value,IEC104_CMM_INFO *info);
int SetPointCmdNB(char se,USHORT address,USHORT value,IEC104_CMM_INFO *info);
int SetPointCmdNC(char se,USHORT address,float value,IEC104_CMM_INFO *info);
int TestCmd(IEC104_CMM_INFO *info);
int	ResetProcessAck(unsigned char cot,unsigned char qrp,IEC104_CMM_INFO *info);
int	ReadCmd(unsigned char qoc,unsigned short infoaddr,IEC104_CMM_INFO *info);
int	ParamSetCmdNA(char kpa,USHORT address,USHORT value,IEC104_CMM_INFO *info);

/*int CommandSend(); */
/*int ClockSynchronisation();*/
int	ClockSynchronise(IEC104_CMM_INFO *info);
int GeneralInterrogation(IEC104_CMM_INFO *info);
int CounterInterrogation(unsigned char qcc,IEC104_CMM_INFO *info);
void PrintSendData(unsigned char sendbuf[], unsigned int nbsend);
void PrintRecvData(unsigned char recvbuf[], unsigned int nbrecv);
int ReadStnDef(char *pStnDefFile);
int GenConfigTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile);
int GenSPTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile);
int GenDPTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile, int ip_no);
int GenMETable(IEC104_CMM_INFO *info, char *pName, char *pDefFile);
int GenSCTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile);
int GenDCTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile);
int GenSETable(IEC104_CMM_INFO *info, char *pName, char *pDefFile);
int GenRCTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile);

BOOL InsertSoeRecBuff(IEC104_CMM_INFO *info, SW_MSG *soe_msg, time_t recv_time);
void DeleteSoeRecFromBuff(IEC104_CMM_INFO *info, IND_REC *soe_rec);
void	CheckRecvSoeRec(IEC104_CMM_INFO *info, COMM_INFO *comm_info);
void	BroadcastSoeRec(IEC104_CMM_INFO *info, COMM_INFO *comm_info);

void	MasterStnProc(int i);
#ifdef SUN_OS
void MainEndproc(int sig_no);
void ChildEndproc(int sig_no);
#endif

#ifdef SUN_OS
void OpenNewTermalWindow (void );
void MainExitRoutine ( DWORD signal_type );
void ChildExitRoutine ( DWORD signal_type );
#endif


