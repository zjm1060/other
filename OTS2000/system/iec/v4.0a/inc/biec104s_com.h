#include 	<sys/types.h>
/*#include	<term.h>
*/
#include 	<string.h>                      
#include 	<stdio.h>
#include 	<signal.h>
#include	<stdlib.h>
#include 	<fcntl.h>
#include 	<xti.h>
#include	<unistd.h>
#include 	<sys/shm.h>
#include 	<sys/sem.h>

#include <sys/socket.h>  /* for AF_INET        */
#include <netinet/in.h>  /* for struct in_addr */
#include <arpa/inet.h>   /* for inet_ntoa()    */
#include <netdb.h>       /* for struct hostent */
#include <sys/errno.h>
/* #include	"/DBINC/dms_com.h" */
#include	"../../../dms/v4.0a/inc/dms_com.h"

#ifdef SUN_OS
#include <errno.h>	/*added 2003.4.11 zy */
#include <wait.h>
#include <time.h>
#else
#include	<sys/time.h>
#endif

#include        "/DPSINC/ipc_common.h"
#include	"/DPSINC/dps_type_def.h"
#include	"/DPSINC/dps_rec_def.h"
#include	"/LANINC/lan_common.h"

#define	REC_YF_AGC_CMD
/*
#define ME_POSITIVE_ONLY
#define CYC_POSITIVE_ONLY
#define SE_POSITIVE_ONLY
#define PM_POSITIVE_ONLY
*/

#define	true	(int)1
#define	false	(int)0

#define IEC104_PORT	2404
#define	MAX_PROC_NUM 6

#define SEND_TIMEOUT_VALUE	1000
#define RECV_TIMEOUT_VALUE	500

#define	SRC_ADRESS	0
#define	CMM_ADRESS	1
#define	DATABAND	20

#define	SCAN_PERIOD	300	/* 300 minutes */
#define	CYCLE_PERIOD	20	/* 20 seconds */
#define	CMD_TIMEOUT	20	/* 20 seconds */
#define	DIFF_TIME	3000	/* 3 seconds */

/* RESOLUTE	0xFFFF: 65535.0 /* 0x7FFF: 32767.0 */
#ifdef ME_POSITIVE_ONLY
#define	MERANGE	32767.0
#else
#define	MERANGE	65535.0
#endif

#ifdef SE_POSITIVE_ONLY
#define	SERANGE	32767.0
#else
#define	SERANGE	65535.0
#endif

#define COM_CHN		0

#define		t0	30		/* the timeout for net connect */
#define		t1	15		/* send or test APDU timeout */
#define		t2	10		/* ACK timout when no data transmitting. t2<t1 */
#define		t3	20		/* the timeout for send test frame at link free status */

#define		K	12		/* at send direction, the max number of un ack APDU */
#define		W	8		/* at recieve direction, befor ack, the max number of recieved APDU */

#define CONFIG_FILE		"/IECDEF/IEC104S_CONFIG.DEF"
#define EXE_FILE		"/IECSRC/iec104s_com.exe"
#define	HOMEPATH		"/IECDEF/"
#define	TMPFILE			"/IECDEF/tmpfile"	
#define	STN_DEF_FILE		"/IECDEF/IEC104S_STATION.DEF"	

#define SPDEF_FILE		"/IECDEF/IEC104_SP_DATA.DEF"
#define DPDEF_FILE		"/IECDEF/IEC104_DP_DATA.DEF"
#define STDEF_FILE		"/IECDEF/IEC104_ST_DATA.DEF"
#define BODEF_FILE		"/IECDEF/IEC104_BO_DATA.DEF"
#define MEDEF_NORM		"/IECDEF/IEC104_ME_NORM.DEF"
#define MEDEF_SCALE		"/IECDEF/IEC104_ME_SCALE.DEF"
#define MEDEF_FLOAT		"/IECDEF/IEC104_ME_FLOAT.DEF"
#define CYCDEF_NORM		"/IECDEF/IEC104_CYC_NORM.DEF"
#define CYCDEF_SCALE		"/IECDEF/IEC104_CYC_SCALE.DEF"
#define CYCDEF_FLOAT		"/IECDEF/IEC104_CYC_FLOAT.DEF"
#define ITDEF_FILE		"/IECDEF/IEC104_IT_DATA.DEF"
#define SCDEF_FILE		"/IECDEF/IEC104_SC_CTRL.DEF"
#define DCDEF_FILE		"/IECDEF/IEC104_DC_CTRL.DEF"
#define RCDEF_FILE		"/IECDEF/IEC104_RC_CTRL.DEF"
#define SEDEF_NORM		"/IECDEF/IEC104_SE_CTRL.DEF"
#define SEDEF_SCALE		"/IECDEF/IEC104_SE_SCALE_CTRL.DEF"
#define SEDEF_FLOAT		"/IECDEF/IEC104_SE_FLOAT_CTRL.DEF"


/* TYPE IDENTIFICATION define: */
#define M_SP_NA		1		/* single point without time tag (class 1, IC) */
#define M_SP_TA		2		/* single point with time tag (SOE) */
#define M_DP_NA		3		/* double point without time tag (class 1, IC) */
#define M_DP_TA		4		/* double point with time tag (SOE) */
#define M_ST_NA		5		/* step point without time tag (class 2, IC) */
#define M_ST_TA		6		/* step point with time tag */
#define M_BO_NA		7		/* slave station remote terminal status */
#define M_BO_TA		8		/* Bitstring of 32 bit with time tag */

#define M_ME_NA		9		/* measure value without time tag (class 2, IC) */
#define M_ME_TA		10		/* measure value with time tag */
#define M_ME_NB		11		/* measure value without time tag,scaled */
#define M_ME_TB		12		/* measure value with time tag,scaled */
#define M_ME_NC		13		/* measure value without time tag,short float */
#define M_ME_TC		14		/* measure value with time tag,short float */

#define M_IT_NA		15		/* integrated totals without time tag */
#define M_IT_TA		16		/* integrated totals with time tag */

#define M_PS_NA		20		/* status detected block sp information (class 1, IC) */
#define M_ME_ND		21		/* measure value, normalized without quality descriptor (class 2, IC) */

#define M_SP_TB		30		/* Single point with time tag CP56Time2a (SOE) */
#define M_DP_TB		31		/* Doule point with time tag CP56Time2a (SOE) */
#define M_ST_TB		32		/* Step point with time tag CP56Time2a */
#define M_ME_TD		34		/* Measure value with time tag CP56Time2a */
#define M_ME_TE		35		/* Measure value with time tag CP56Time2a,scaled */
#define M_ME_TF		36		/* Measure value with time tag CP56Time2a,float value */
#define M_IT_TB		37		/* Integrated totals with time tag CP56Time2a */

#define M_EI_NA		70		/* end of initialization */

#define M_BD_NA		232		/* BCD code water level (class 2, IC) */

#define C_SC_NA		45		/* single point control command */
#define C_DC_NA		46		/* double point control command */
#define C_RC_NA		47		/* regulating step command */
#define C_SE_NA		48		/* set point command normalized value */
#define C_SE_NB		49		/* set point command scaled value */
#define C_SE_NC		50		/* set point command floating point value */
#define C_BO_NA		51		/* Bitstring of 32 bit */

#define C_SE_ND		136		/* multi set point command normalized value */

#define C_IC_NA		100		/* interrogation command */
#define C_CI_NA		101		/* counter interrogation command */
#define C_RD_NA		102		/* read command */
#define C_CS_NA		103		/* clock synchronization command */
#define C_TS_NA		104		/* test command */
#define C_RP_NA		105		/* Reset process command */
#define C_CD_NA		106		/* Delay acquire command */

#define P_ME_NA		110		/* parameter for measurements,normalized */
#define P_ME_NB		111		/* parameter for measurements,scaled */
#define P_ME_NC		112		/* parameter for measurements,float */
#define P_AC_NA		113		/* parameter activation */

/* cause of transmit */
#define	PER_CYC		1		/* cyclic transmission */
#define	BACK		2
#define	SPONT		3		/* spontaneous transmission */
#define	INIT		4
#define	REQ			5
#define	ACT			6		/* activation C direction */
#define	ACTCON		7		/* activation confirmation M direction */
#define	NACTCON		0x47	/* activation confirmation M direction */
#define	DEACT		8		/* deactivation C direction */
#define	DEACTCON	9		/* deactivation confirmation M direction */
#define	ACTTERM		10		/* activation terminal M direction */
#define	INTROGEN	20		/* interrogation C direction */
#define	INTRO1		21		/* respond interrogation group 1 SP/DP */
#define	INTRO2		22		/* respond interrogation group 2 SP/DP */
#define	INTRO3		23		/* respond interrogation group 3 SP/DP */
#define	INTRO4		24		/* respond interrogation group 4 SP/DP */
#define	INTRO5		25		/* respond interrogation group 5 SP/DP */
#define	INTRO6		26		/* respond interrogation group 6 SP/DP */
#define	INTRO7		27		/* respond interrogation group 7 SP/DP */
#define	INTRO8		28		/* respond interrogation group 8 SP/DP */
#define	INTRO9		29		/* respond interrogation group 9 ME */
#define	INTRO10		30		/* respond interrogation group 10 ME */
#define	INTRO11		31		/* respond interrogation group 11 ME */
#define	INTRO12		32		/* respond interrogation group 12 ME */
#define	INTRO13		33		/* respond interrogation group 13 ME */
#define	INTRO14		34		/* respond interrogation group 14 ME Databand Parameter */
#define	INTRO15		35		/* respond interrogation group 15 ME */
#define	INTRO16		36		/* respond interrogation group 16 ME */

#define	REQCOGCN	37		/* respond integrated Total */
#define	REQCO1		38		/* respond integrated Total group 1 */
#define	REQCO2		39		/* respond integrated Total group 2 */
#define	REQCO3		40		/* respond integrated Total group 3 */
#define	REQCO4		41		/* respond integrated Total group 4 */

#define	UKTYPE		44		/* unknown type id */
#define	UKCOT		45		/* unknown cause of transmit */
#define	UKASDUAD	46		/* unknown ASDU common address */
#define	UKINFOAD	47		/* unknown information address */

#define FIXFRAMELEN	6
#define VARFRAMELEN	288
#define MINFRAMELEN	6
/*
#define MAX_BUF_SIZE    SHM_DATA_SEG_SIZE
#define MAX_RECV_NUM    SHM_DATA_SEG_COUNTER
*/
#define CMD_ENTRY_NUMBER	100
#define CMD_DATA_SIZE		288		/* 16 */
#define ASDU_ENTRY_NUMBER	100
#define ASDU_DATA_SIZE		288

#define	MAX_SP_NUM		20000
#define	MAX_DP_NUM		20000
#define	MAX_ME_NUM		5000
#define	MAX_CYC_NUM		256
#define	MAX_ST_NUM		32
#define	MAX_BO_NUM		32
#define	MAX_IT_NUM		64

#define	MAX_SC_NUM		100
#define	MAX_DC_NUM		512
#define	MAX_RC_NUM		100
#define	MAX_SE_NUM		256

/* control 6001H--6100H 24577-24832(256) */
#define	SC_CMD_BASE_ADDR	0x6001
#define	DC_CMD_BASE_ADDR	0x6001

/* RC control 6101H--6200H 24833-25088(256) */
#define	RC_CMD_BASE_ADDR	0x6101

/* setpoint 6201H--6400H 25089-25600(512) */
#define	SE_CMD_BASE_ADDR	0x6201
#define	SE_CMD_SCALE_ADDR	0x6201
#define	SE_CMD_FLOAT_ADDR	0x6201

#define	BC_CMD_BASE_ADDR	0xCA1

/* system status 0001H--0020H 1-32(32) */
#define	SYS_BASE_ADDR	0x0001

/* indication status 0021H--4000H 33-16384(16352) */
/* #define	SP_BASE_ADDR	0x0021 */
#define	SP_BASE_ADDR	0x0001
#define	DP_BASE_ADDR	0x0001

/* measurement 4001H--5000H 16385-20480(4096) */
#define	ME_BASE_ADDR	0x4001
#define	ME_SCALE_ADDR	0x4001
#define	ME_FLOAT_ADDR	0x4001
#define	CYC_BASE_ADDR	0x4800
#define	CYC_SCALE_ADDR	0x4800
#define	CYC_FLOAT_ADDR	0x4800

/* parameter 5001H--6000H 20481-24576(4096) */
#define	PM_BASE_ADDR	0x901

/* integrated total 6401H--6600H 25601-26112(512) */
#define	IT_BASE_ADDR	0xC01

/* step position 6601H--6700H 26113-26368(256) */
#define	ST_BASE_ADDR	0xC81

#define	BO_BASE_ADDR	0xCA1

#define	SPONT_SP_NUM		60
#define	SPONT_DP_NUM		60
#define	TA_SP_NUM		32
#define	TA_DP_NUM		32
#define	TB_SP_NUM		20
#define	TB_DP_NUM		20
#define	SPONT_ME_NUM		40
#define	SPONT_ME_FLOAT_NUM	30

#define	ME_GROUP_NUM		80
#define	ME_FLOAT_GROUP_NUM	48
#define	SP_GROUP_NUM		120
#define	PS_GROUP_NUM		960
#define	DP_GROUP_NUM		120
#define	IT_GROUP_NUM		16
#define	ST_GROUP_NUM		120
#define	CI_GROUP_NUM		32

#define	SOE_NUM			128
#define	SP_REC_NUM		128
#define	DP_REC_NUM		128

#define	CMD_SOURCE		3	/* Cascade dispatch center */

#define	NAME_SIZE		36
#define	CMDLINKERR		88	/* Channel fail database point address */

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
{	char	NameID[50];
	char	Ip[30];
	char	ctrl_priv[40];
	int	Pid;
/*	PROCESS_INFORMATION	pi; */
}PROC_REC;

typedef	struct
{
	USHORT	number;
	char	name[MAX_SP_NUM][POINT_NAME_SIZE];
	int	TimeTag[MAX_SP_NUM];
	POINTER	ptr[MAX_SP_NUM];
} SPDEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_DP_NUM][POINT_NAME_SIZE];
	int	TimeTag[MAX_DP_NUM];
	POINTER	ptr[MAX_DP_NUM];
} DPDEF;

typedef	struct
{
	USHORT	number;
	float	lorange[MAX_ME_NUM];
	float	hirange[MAX_ME_NUM];
	float	databand[MAX_ME_NUM];
	char	name[MAX_ME_NUM][POINT_NAME_SIZE];
	int	TimeTag[MAX_ME_NUM];
} MEDEF;

typedef	struct
{
	USHORT	number;
	float	scale[MAX_ME_NUM];
	float	lorange[MAX_ME_NUM];
	float	hirange[MAX_ME_NUM];
	float	databand[MAX_ME_NUM];
	char	name[MAX_ME_NUM][POINT_NAME_SIZE];
	int	TimeTag[MAX_ME_NUM];
} ME_SCALE_DEF;

typedef	struct
{
	USHORT	number;
	float	lorange[MAX_ME_NUM];
	float	hirange[MAX_ME_NUM];
	float	databand[MAX_ME_NUM];
	char	name[MAX_ME_NUM][POINT_NAME_SIZE];
	int	TimeTag[MAX_ME_NUM];
} ME_FLOAT_DEF;

typedef	struct
{
	USHORT	number;
	float	lorange[MAX_CYC_NUM];
	float	hirange[MAX_CYC_NUM];
	float	databand[MAX_CYC_NUM];
	char	name[MAX_CYC_NUM][36];
} CYCDEF;

typedef	struct
{
	USHORT	number;
	float	scale[MAX_ME_NUM];
	float	lorange[MAX_ME_NUM];
	float	hirange[MAX_ME_NUM];
	float	databand[MAX_ME_NUM];
	char	name[MAX_ME_NUM][POINT_NAME_SIZE];
} CYC_SCALE_DEF;

typedef	struct
{
	USHORT	number;
	float	lorange[MAX_ME_NUM];
	float	hirange[MAX_ME_NUM];
	float	databand[MAX_ME_NUM];
	char	name[MAX_ME_NUM][POINT_NAME_SIZE];
} CYC_FLOAT_DEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_ST_NUM][POINT_NAME_SIZE];
	int	TimeTag[MAX_ST_NUM];
	POINTER	ptr[MAX_ST_NUM];
} STDEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_IT_NUM][POINT_NAME_SIZE];
	int	TimeTag[MAX_IT_NUM];
} ITDEF;

typedef	struct
{
	USHORT	number;
	unsigned char	cmdtype[MAX_SC_NUM];
	unsigned char	optype[MAX_SC_NUM];
	char	name[MAX_SC_NUM][POINT_NAME_SIZE];
} SCDEF;

typedef	struct
{
	USHORT	number;
	unsigned char	cmdtype[MAX_DC_NUM];
	unsigned char	optype[MAX_DC_NUM];
	char	name[MAX_DC_NUM][POINT_NAME_SIZE];
} DCDEF;

typedef	struct
{
	USHORT	number;
	float	lorange[MAX_SE_NUM];
	float	hirange[MAX_SE_NUM];
	unsigned char	cmdtype[MAX_SE_NUM];
	char	name[MAX_SE_NUM][POINT_NAME_SIZE];
} SEDEF;

typedef	struct
{
	USHORT	number;
	float	lorange[MAX_SE_NUM];
	float	hirange[MAX_SE_NUM];
	float	scale[MAX_SE_NUM];
	unsigned char	cmdtype[MAX_SE_NUM];
	char	name[MAX_SE_NUM][POINT_NAME_SIZE];
} SE_SCALE_DEF;

typedef	struct
{
	USHORT	number;
	float	lorange[MAX_SE_NUM];
	float	hirange[MAX_SE_NUM];
	unsigned char	cmdtype[MAX_SE_NUM];
	char	name[MAX_SE_NUM][POINT_NAME_SIZE];
} SE_FLOAT_DEF;

typedef	struct
{
	USHORT	number;
	unsigned char	cmdtype[MAX_RC_NUM];
	unsigned char	optype[MAX_DC_NUM];
	char	name[MAX_RC_NUM][POINT_NAME_SIZE];
} RCDEF;

typedef	struct
{
	unsigned char	tpyeId;
	unsigned short	infoaddr;
	unsigned char	stat;
	unsigned char	vti;
	unsigned short	stvalue;
	float	fvalue;
	DATE_TIME	time;
} SOEREC;

typedef	struct
{
	unsigned short	bufpnt;
	unsigned short	sendpnt;
	SOEREC	soerec[SOE_NUM];
} SOEBUF;

typedef	struct
{
	unsigned char	tpyeId;
	unsigned short	infoaddr;
	unsigned char	stat;
} SWREC;

typedef	struct
{
	unsigned short	bufpnt;
	unsigned short	sendpnt;
	SWREC	rec[SP_REC_NUM];
} SPBUF;

typedef	struct
{
	unsigned short	bufpnt;
	unsigned short	sendpnt;
	SWREC	rec[DP_REC_NUM];
} DPBUF;

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

typedef	struct
{
	unsigned char	tpyeId;
	unsigned char	cot;
	unsigned short	infoaddr;
	unsigned char	qpm;
	unsigned char	ack_flag;
	unsigned short	setvalue;
	float		fvalue;
} PMBUF;

typedef	struct
{
	unsigned char	tpyeId;
	unsigned char	cot;
	unsigned char	qoi;
	unsigned char	ack_flag;
} ICBUF;

typedef	struct
{
	unsigned char	tpyeId;
	unsigned char	cot;
	unsigned char	qcc;
	unsigned char	ack_flag;
} CIBUF;

typedef	struct
{
	unsigned char	tpyeId;
	unsigned char	cot;
	unsigned short	infoaddr;
	unsigned char	ack_flag;
} RDBUF;

typedef	struct
{
	unsigned char	tpyeId;
	unsigned char	cot;
	unsigned char	qrp;
	unsigned char	ack_flag;
} RPBUF;

typedef	struct
{
	unsigned char	tpyeId;
	unsigned char	cot;
	unsigned char	ack_flag;
} TESTBUF;

typedef	struct
{
	unsigned char	tpyeId;
	unsigned char	cot;
	unsigned char	ack_flag;
	unsigned short	cp16time;
	unsigned short	locp16time;
} CDBUF;

typedef	struct
{
	unsigned char	tpyeId;
	unsigned char	cot;
	unsigned int	pnt;
} MIRBUF;

typedef	struct
{
	unsigned char	tpyeId;
	unsigned char	cot;
	unsigned char	qoi;
	unsigned char	ack_flag;
} ICGROUPBUF;

typedef	struct
{
	unsigned char	tpyeId;
	unsigned char	cot;
	unsigned char	qoi;
	unsigned char	ack_flag;
} CIGROUPBUF;

/*
typedef struct
{
	USHORT		length;
	UCHAR		dp_type;
	UCHAR		dest_id;
	UCHAR 		src_id;
	UCHAR 		dest_stn;
	USHORT		packet_num;
}LAN_HEAD;

typedef	struct
{
	UCHAR		type_id;
	UCHAR		stn_id;
	UCHAR		dev_id;
	UCHAR		data_type;
	USHORT		point_id;
	CMD_STATUS	status;
	union
	{
		int	iValue;
		FLOAT	fValue;
	}data;
	USHORT		ctrl_addr;
	UCHAR		host_id;
	UCHAR		usr_id;
	UCHAR		res;
	UCHAR		second;
	UCHAR		minute;
	UCHAR		hour;
	UCHAR		day;
	UCHAR		month;
	USHORT		year;
	char		usr_login_name[8];
} COMMAND_MSG;
*/

/* static char	*comstatus[]={"GBX_SYS.SYSMSG.TD_TD_STAT","GBX_SYS.SYSMSG.YF_TD2"}; */
static char	*cas_ctrl_priv= "KZQGTD";	/* 梯调控制权 */
static char	*nd_ctrl_priv= "KZQGND";	/* 网调控制权 */

int	Net_Com(int pnum,int sockfd);
int	UpdateChannelFlag(int pnum,unsigned short status);
void	OpenNewTermalWindow ( void );
void	ExitRoutine ( unsigned short signal_type );

int	IEC870_5_Recv(int pnum,unsigned int recvdatapt,int sockfd);
void	IEC104_SOE(void);
int	Get_Cas_Ctrl_Priv(char * ctrl_priv);

int	RespondStartDT(int sock);
int	RespondStopDT(int sock);
int	RespondTestFR(int sock);
int	CmdRespSend(int sock);
int	SERespSend(int sock);
int	PMRespSend(int sock);
int	MirrorNAck(unsigned char cause,unsigned int pnt,int sock);
int	SinglePointCtrlAck(unsigned char cause,unsigned short address,unsigned char stat,int sock);
int	DoublePointCtrlAck(unsigned char cause,unsigned short address,unsigned char stat,int sock);
int	RegulatStepCtrlAck(unsigned char cause,unsigned short address,unsigned char stat,int sock);
int	SetPointCmdNAAck(unsigned char cause,unsigned short address,unsigned short value,unsigned char se,int sock);
/* int	MultiSetPointCmdNAAck(unsigned char cause,unsigned char num,unsigned int pnt,unsigned char se,int sock); */
int	SetPointCmdNBAck(unsigned char cause,unsigned short address,unsigned short value,unsigned char se,int sock);
int	SetPointCmdNCAck(unsigned char cause,unsigned short address,float value,unsigned char se,int sock);
int	SetPointCmdNDAck(unsigned char cause,unsigned char num,unsigned int pnt,unsigned char se,int sock);
int	ParamValueNAAck(unsigned char cause,unsigned char qpm,unsigned short address,unsigned short value,int sock);
int	ParamValueNBAck(unsigned char cause,unsigned char qpm,unsigned short address,unsigned short value,int sock);
int	ParamValueNCAck(unsigned char cause,unsigned char qpm,unsigned short address,float value,int sock);

int	ClockSynchroniseAck(int sock);
int	TimeDelayAck(int sock);
int	GeneralInterrogationAck(unsigned char gi_cause,unsigned char gi_qoi,int sock);
int	CounterInterrogationAck(unsigned char gi_cause,unsigned char gi_qcc,int sock);
int	RespondTestAck(int sock);
int	ResetProcessAck(int sock);
int	ReadCmdAck(unsigned char qoc,unsigned short infoaddr,int sock);
int	ReadCmdResp(unsigned short infoaddr,int sock);
int	SysInform(unsigned short info_addr,int sock);
int	SinglePointInform(unsigned char gi_cause,int sock);
int	CheckSinglePointChange(void);
int	ReadSingleInform(unsigned short infoaddr,int sock);
int	SinglePointInformCyc(unsigned char gi_cause,unsigned short infogroup,int sock);
int	SinglePointInformTA(unsigned char gi_cause,int sock);
int	SinglePointInformTB(unsigned char gi_cause,int sock);
int	SinglePointGroupCyc(unsigned char gi_cause,unsigned short infogroup,int sock);
int	DoublePointInform(unsigned char gi_cause,int sock);
int	CheckDoublePointChange(void);
int	ReadDoubleInform(unsigned short infoaddr,int sock);
int	DoublePointInformCyc(unsigned char gi_cause,unsigned short infogroup,int sock);
int	DoublePointInformTA(unsigned char gi_cause,int sock);
int	DoublePointInformTB(unsigned char gi_cause,int sock);
int	StepPositionInform(unsigned char gi_cause,int sock);
int	CheckStepPositionChange(void);
int	ReadStepPositionInform(unsigned short infoaddr,int sock);
int	StepPositionInformCyc(unsigned char gi_cause,unsigned short infogroup,int sock);
int	StepPositionInformTA(unsigned char gi_cause,int sock);
int	StepPositionInformTB(unsigned char gi_cause,int sock);
int	MeasureValueNA(unsigned char gi_cause,int sock);
int	CheckMeasureValueNA(void);
int	MeasureValueNACyc(unsigned char gi_cause,unsigned short infogroup,int sock);
int	ReadMeasureValueNA(unsigned short infoaddr,int sock);
int	MeasureValueNC(unsigned char gi_cause,int sock);
int	CheckMeasureValueNC(void);
int	MeasureValueNCCyc(unsigned char gi_cause,unsigned short infogroup,int sock);
int	ReadMeasureValueNC(unsigned short infoaddr,int sock);
int	MeasureValueND(unsigned char gi_cause,int sock);
int	MeasureValueNDCyc(unsigned char gi_cause,unsigned short infogroup,int sock);
int	IntegratedTotals(unsigned char gi_cause,unsigned char infogroup,int sock);
int	IntegratedTotalsTA(unsigned char gi_cause,unsigned short infogroup,int sock);
int	IntegratedTotalsTB(unsigned char gi_cause,unsigned short infogroup,int sock);
int	EndofInitialization(int sock);
int	RespondGroupInterrogation(unsigned char gi_qoi,int sock);
int	RespondGeneralInterrogation(int sock);
int	ScanDataTransmit(int sock);
int	RespondParamInterrogation(int sockfd);
int	RespondCountGroupInterrogation(unsigned char gi_qcc,int sock);
int	RespondCountInterrogation(int sock);
int	RespondUserDataClass1(int sock);
int	CheckACD(void);
int	CheckMEACD(void);
int	RespondUserDataClass2(int sock);
int	SequenceofEvent(int sockfd);

void	PrintSendData(unsigned short nbsend);
void	PrintRecvData(unsigned short nbrecv);

void	InitData(char * pName);
ANLG_DATA FloatToAnaData(float data);
float	Datcvt(short status,unsigned short data);

int	ReadStnDef(char * pStnDefFile);
int	GenConfigTable(char * pName,char * pDefFile);

int	GenSPTable(char * pName,char *pDefFile);	
int	GenDPTable(char * pName,char *pDefFile);	
int	GenMETable(char * pName,char *pDefFile);
int	SaveMETable(char * pName,char *pDefFile);
int	GenMEScaleTable(char * pName,char *pDefFile);
int	SaveMEScaleTable(char * pName,char *pDefFile);
int	GenMEFloatTable(char * pName,char *pDefFile);
int	SaveMEFloatTable(char * pName,char *pDefFile);
int	GenSTTable(char * pName,char *pDefFile);
int	GenITTable(char * pName,char *pDefFile);
int	GenSCTable(char * pName,char *pDefFile);
int	GenDCTable(char * pName,char *pDefFile);
int	GenRCTable(char * pName,char *pDefFile);
int	GenSETable(char * pName,char *pDefFile);
int	GenSEScaleTable(char * pName,char *pDefFile);
int	GenSEFloatTable(char * pName,char *pDefFile);
int	GenCYCTable(char * pName,char *pDefFile);
int	GenCYCScaleTable(char * pName,char *pDefFile);
int	GenCYCFloatTable(char * pName,char *pDefFile);

extern	void	Lan_init(void);
extern	char	GetHost_Cmd(void);
extern	MountDB();

