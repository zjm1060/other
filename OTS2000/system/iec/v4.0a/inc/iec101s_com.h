#include 	<sys/types.h>
#include 	<string.h>                      
#include 	<stdio.h>
#include 	<signal.h>
#include	<stdlib.h>
#include 	<fcntl.h>


#include	"../../../dms/v4.0a/inc/dms_com.h"


#include	"/home/ems/system/dps/v4.0a/inc/ipc_common.h"
#include	"/home/ems/system/dps/v4.0a/inc/dps_type_def.h"
#include	"/home/ems/system/dps/v4.0a/inc/dps_rec_def.h"
#include	"/home/ems/system/lan/v4.0a/inc/lan_common.h"

#include "./iec_cmm_def.h"

#define	DEBUG

#define	REC_YF_AGC_CMD
#define	LINE_BUF_CHAR		256

#define	DSP_INFO_SIZE		102400
#define SHM_BUF_KEY_CHAR	'0'


#define sleep(x)	Sleep(x*1000)


#define Com_Baud_Rate	1200

#define NONE	0
#define EVEN	1
#define ODD	2

#define CBINQU	1024
#define CBOUTQU	1024

#define	MAX_PROC_NUM	8

#define SEND_TIMEOUT_VALUE	1000
#define RECV_TIMEOUT_VALUE	500

#define RD_INTERVAL_TIMEOUT		(DWORD)(396000/Com_Baud_Rate+1)
#define RD_TOTAL_TIMEOUT_MUL	(DWORD)(22000/Com_Baud_Rate+1)
#define RD_TOTAL_TIMEOUT_CNT	(DWORD)(396000/Com_Baud_Rate+1)
#define WT_TOTAL_TIMEOUT_MUL	(DWORD)(11000/Com_Baud_Rate+1)
#define WT_TOTAL_TIMEOUT_CNT	(DWORD)(33000/Com_Baud_Rate+1)

#define	BROADCAST_LINKADRESS1	0xFF
#define	BROADCAST_CMMADRESS1	0xFF
#define	BROADCAST_LINKADRESS2	0xFFFF
#define	BROADCAST_CMMADRESS2	0xFFFF
#define	LINKADRESS	1
#define	CMM_ADRESS	1
#define	DATABAND	20

#define	SCAN_PERIOD	300		/*300 minutes */
#define	CYCLE_PERIOD	20		/*20 seconds */
#define	CMD_TIMEOUT	20		/*20 seconds */
#define	CMD_DELAY	200		/* 20 seconds */
#define	DIFF_TIME	3000		/*3 seconds */

/*RESOLUTE	0xFFFF: 65535.0 /*0x7FFF: 32767.0 */
#define	HI_CODE	32767.0f /*ming091109*/
#define	LO_CODE	-32768.0f

#define COM_CHN		0


#define	IEC101_HOMEPATH		"/home/ems/h9000/iec101/"
#define	IEC101_LOGPATH		"/home/ems/h9000/his/iec101/"
#define	cmdrecbuf			"/home/ems/h9000/his/iec101/CMD_"

#define	STN_DEF_FILE	"IEC101S_STATION.DEF"	
#define CONFIG_FILE		"CONFIG.DEF"
#define SPDEF_FILE		"SP_DATA.DEF"
#define DPDEF_FILE		"DP_DATA.DEF"
#define STDEF_FILE		"ST_DATA.DEF"
#define BODEF_FILE		"BO_DATA.DEF"
#define MEDEF_NORM		"ME_DATA.DEF"
#define CYCDEF_NORM		"CYC_DATA.DEF"
#define ITDEF_FILE		"IT_DATA.DEF"
#define SCDEF_FILE		"SC_CTRL.DEF"
#define DCDEF_FILE		"DC_CTRL.DEF"
#define RCDEF_FILE		"RC_CTRL.DEF"
#define SEDEF_NORM		"SE_CTRL.DEF"

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
#define M_ME_TD		34		/*Measure value with time tag CP56Time2a */
#define M_ME_TE		35		/*Measure value with time tag CP56Time2a,scaled */
#define M_ME_TF		36		/*Measure value with time tag CP56Time2a,float value */
#define M_IT_TB		37		/*Integrated totals with time tag CP56Time2a */

#define M_EI_NA		70		/*end of initialization */

#define M_BD_NA		232		/*BCD code water level (class 2, IC) */

#define C_SC_NA		45		/*single point control command */
#define C_DC_NA		46		/*double point control command */
#define C_RC_NA		47		/*regulating step command */
#define C_SE_NA		48		/*set point command normalized value */
#define C_SE_NB		49		/*set point command scaled value */
#define C_SE_NC		50		/*set point command floating point value */
#define C_BO_NA		51		/*Bitstring of 32 bit */

#define C_SC_TA		58		/* single point control command with time tag CP56Time2a*/
#define C_DC_TA		59		/* double point control command with time tag CP56Time2a*/
#define C_RC_TA		60		/* regulating step command with time tag CP56Time2a*/
#define C_SE_TA		61		/* set point command normalized value with time tag CP56Time2a*/
#define C_SE_TB		62		/* set point command scaled value with time tag CP56Time2a*/
#define C_SE_TC		63		/* set point command floating point value with time tag CP56Time2a*/
#define C_BO_TA		64		/* Bitstring of 32 bit with time tag CP56Time2a*/

#define C_SE_ND		136		/*multi set point command normalized value */

#define C_IC_NA		100		/*interrogation command */
#define C_CI_NA		101		/*counter interrogation command */
#define C_RD_NA		102		/*read command */
#define C_CS_NA		103		/*clock synchronization command */
#define C_TS_NA		104		/*test command */
#define C_RP_NA		105		/*Reset process command */
#define C_CD_NA		106		/*Delay acquire command */

#define C_TS_TA		107		/* test command time tag CP56Time2a*/

#define P_ME_NA		110		/*parameter for measurements,normalized */
#define P_ME_NB		111		/*parameter for measurements,scaled */
#define P_ME_NC		112		/*parameter for measurements,float */
#define P_AC_NA		113		/*parameter activation */

/*cause of transmit */
#define	PER_CYC		1		/*cyclic transmission */
#define	BACK		2
#define	SPONT		3		/*spontaneous transmission */
#define	INIT		4
#define	REQ		5
#define	ACT		6		/*activation C direction */
#define	ACTCON		7		/*activation confirmation M direction */
#define	NACTCON		0x47		/*activation confirmation M direction */
#define	DEACT		8		/*deactivation C direction */
#define	DEACTCON	9		/*deactivation confirmation M direction */
#define	ACTTERM		10		/*activation terminal M direction */
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
#define	INTRO13		33		/*respond interrogation group 13 STEP POSITION */
#define	INTRO14		34		/*respond interrogation group 14 BCD code */
#define	INTRO15		35		/*respond interrogation group 15 remote terminal state */
#define	INTRO16		36		/*respond interrogation group 16 res */

#define	REQCOGCN	37		/*respond integrated Total */
#define	REQCO1		38		/*respond integrated Total group 1 */
#define	REQCO2		39		/*respond integrated Total group 2 */
#define	REQCO3		40		/*respond integrated Total group 3 */
#define	REQCO4		41		/*respond integrated Total group 4 */

#define	UKTYPE		44		/*unknown type id */
#define	UKCOT		45		/*unknown cause of transmit */
#define	UKASDUAD	46		/*unknown ASDU common address */
#define	UKINFOAD	47		/*unknown information address */

#define VARFRAMELEN	288

#define CMD_ENTRY_NUMBER	100
#define CMD_DATA_SIZE		288		/*16 */
#define ASDU_ENTRY_NUMBER	100
#define ASDU_DATA_SIZE		288

#define	O_NAME_SIZE	60	/*36*/

#define	MAX_SP_NUM		1024
#define	MAX_DP_NUM		1024
#define	MAX_ME_NUM		512
#define	MAX_CYC_NUM		512
#define	MAX_ST_NUM		128
#define	MAX_BO_NUM		128
#define	MAX_IT_NUM		128

#define	MAX_SC_NUM		512
#define	MAX_DC_NUM		512
#define	MAX_RC_NUM		256
#define	MAX_SE_NUM		256

/*control 6001H--6100H 24577-24832(256) */
#define	SC_CMD_BASE_ADDR	0xB01
#define	DC_CMD_BASE_ADDR	0xB01

/*RC control 6101H--6200H 24833-25088(256) */
#define	RC_CMD_BASE_ADDR	0x6101

/*setpoint 6201H--6400H 25089-25600(512) */
#define	SE_CMD_BASE_ADDR	0x6201
#define	SE_CMD_SCALE_ADDR	0x6201
#define	SE_CMD_FLOAT_ADDR	0x6401

#define	BC_CMD_BASE_ADDR	0xCA1

/*system status 0001H--0020H 1-32(32) */
#define	SYS_BASE_ADDR		0x0001

/*indication status 0021H--4000H 33-16384(16352) */
/*#define	SP_BASE_ADDR	0x0021 */
#define	SP_BASE_ADDR		0x0001
#define	DP_BASE_ADDR		0x1000

/*measurement 4001H--5000H 16385-20480(4096) */
#define	ME_BASE_ADDR		0x4001
#define	CYC_BASE_ADDR		0x4801

/*parameter 5001H--6000H 20481-24576(4096) */
#define	PM_BASE_ADDR		0x5001

/*integrated total 6401H--6600H 25601-26112(512) */
#define	IT_BASE_ADDR		0xC01

/*step position 6601H--6700H 26113-26368(256) */
#define	ST_BASE_ADDR		0xC81

#define	BO_BASE_ADDR		0xCA1

#define	IC_SP_GROUP_NUM	64
#define	IC_ME_GROUP_NUM	32
#define	CI_GROUP_NUM		32

#define	SPONT_SP_NUM		60
#define	SPONT_DP_NUM		60
#define	TA_SP_NUM		32
#define	TA_DP_NUM		32
#define	TB_SP_NUM		20
#define	TB_DP_NUM		20

#define	SPONT_ME_NUM		40
#define	SPONT_ME_TD_NUM		18
#define	SPONT_ME_TE_NUM		18
#define	SPONT_ME_FLOAT_NUM	30
#define	SPONT_ME_FLOAT_TF_NUM	16

#define	ME_GROUP_NUM		32
#define	ME_FLOAT_GROUP_NUM	32
#define	SP_GROUP_NUM		64
#define	PS_GROUP_NUM		480
#define	DP_GROUP_NUM		64
#define	IT_GROUP_NUM		16
#define	ST_GROUP_NUM		32
#define	CI_GROUP_NUM		32

#define	SOE_NUM			1024
#define	SP_REC_NUM		128
#define	DP_REC_NUM		128

#define	CMD_SOURCE		3	/* Cascade dispatch center */

#define	NAME_SIZE		(POINT_NAME_SIZE+1)
#define	CMDLINKERR		88	/*Channel fail database point address */


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
	char	ctrl_priv[POINT_NAME_SIZE];
	int		Pid;
	unsigned char	cmd_src;
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
	char	name[MAX_ME_NUM][POINT_NAME_SIZE];
	float	lorange[MAX_ME_NUM];
	float	hirange[MAX_ME_NUM];
	float	scale[MAX_ME_NUM];
	float	databand[MAX_ME_NUM];
	char	ASDUType[MAX_ME_NUM];
	char	TimeTag[MAX_ME_NUM];
	POINTER	ptr[MAX_ME_NUM];
} MEDEF;

typedef	struct
{
	USHORT	number;
	POINTER	ptr[MAX_ST_NUM];
	char	name[MAX_ST_NUM][POINT_NAME_SIZE];
	int		TimeTag[MAX_ST_NUM];
} STDEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_IT_NUM][POINT_NAME_SIZE];
	int		TimeTag[MAX_IT_NUM];
	POINTER	ptr[MAX_IT_NUM];
} ITDEF;

typedef	struct
{
	USHORT	number;
	unsigned char	cmdtype[MAX_SC_NUM];
	unsigned char	optype1[MAX_SC_NUM];
	unsigned char	optype2[MAX_SC_NUM];
	char	name[MAX_SC_NUM][POINT_NAME_SIZE];
	char	interlockname[MAX_SC_NUM][O_NAME_SIZE];
} SCDEF;

typedef	struct
{
	USHORT	number;
	unsigned char	cmdtype[MAX_DC_NUM];
	unsigned char	optype1[MAX_DC_NUM];
	unsigned char	optype2[MAX_DC_NUM];
	char	name[MAX_DC_NUM][POINT_NAME_SIZE];
	char	interlockname[MAX_DC_NUM][O_NAME_SIZE];
} DCDEF;

typedef	struct
{
	USHORT	number;
	float	lorange[MAX_SE_NUM];
	float	hirange[MAX_SE_NUM];
	float	scale[MAX_ME_NUM];
	unsigned char	cmdtype[MAX_SE_NUM];
	char	name[MAX_SE_NUM][POINT_NAME_SIZE];
	char	interlockname[MAX_SE_NUM][O_NAME_SIZE];
} SEDEF;

typedef	struct
{
	USHORT	number;
	unsigned char	cmdtype[MAX_RC_NUM];
	unsigned char	optype1[MAX_DC_NUM];
	unsigned char	optype2[MAX_DC_NUM];
	char	name[MAX_RC_NUM][POINT_NAME_SIZE];
	char	interlockname[MAX_RC_NUM][O_NAME_SIZE];
} RCDEF;

typedef	struct
{
	USHORT	number;
	POINTER	ptr[MAX_CYC_NUM];
	char	name[MAX_CYC_NUM][36];
	float	lorange[MAX_CYC_NUM];
	float	hirange[MAX_CYC_NUM];
	float	scale[MAX_CYC_NUM];
	char	ASDUType[MAX_CYC_NUM];
} CYCDEF;

typedef	struct
{
	unsigned char	typeId;
	unsigned short	infoaddr;
	unsigned char	stat;
	unsigned char	vti;
	unsigned short	stvalue;
	float		fvalue;
	DATE_TIME	time;
} SOEREC;

typedef	struct
{
	unsigned short	count;
	unsigned short	ackpnt;
	unsigned short	bufpnt;
	unsigned short	sendpnt;
	SOEREC	soerec[SOE_NUM];
} SOEBUF;

typedef	struct
{
	unsigned char	typeId;
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
	unsigned short	opstat;
	unsigned char	typeId;
	unsigned char	cot;
	unsigned short	infoaddr;
	unsigned char	stat;
	unsigned char	ack_flag;
	unsigned char	selected;
	unsigned char	term_flag;
	time_t			sel_tim;
	unsigned char	ackdata[CMD_DATA_SIZE];
} CMDBUF;

typedef	struct
{
	unsigned char	typeId;
	unsigned char	cot;
	unsigned short	infoaddr;
	unsigned char	qoc;
	unsigned char	ack_flag;
	unsigned char	term_flag;
	unsigned char	infonum;
	unsigned short	setvalue;
	float			fvalue;
	unsigned int	rcvpnt;
} SEBUF;

typedef	struct
{
	unsigned char	typeId;
	unsigned char	cot;
	unsigned short	infoaddr;
	unsigned char	qpm;
	unsigned char	ack_flag;
	unsigned short	setvalue;
	float			fvalue;
} PMBUF;

typedef	struct
{
	unsigned char	typeId;
	unsigned char	cot;
	unsigned char	qoi;
	unsigned char	ack_flag;
} ICBUF;

typedef	struct
{
	unsigned char	typeId;
	unsigned char	cot;
	unsigned char	qcc;
	unsigned char	ack_flag;
} CIBUF;

typedef	struct
{
	unsigned char	typeId;
	unsigned char	cot;
	unsigned short	infoaddr;
	unsigned char	ack_flag;
} RDBUF;

typedef	struct
{
	unsigned char	typeId;
	unsigned char	cot;
	unsigned char	qrp;
	unsigned char	ack_flag;
} RPBUF;

typedef	struct
{
	unsigned char	typeId;
	unsigned char	cot;
	unsigned char	ack_flag;
} TESTBUF;

typedef	struct
{
	unsigned char	typeId;
	unsigned char	cot;
	unsigned char	ack_flag;
	unsigned short	cp16time;
	time_t			recv_time_sec;
	int				recv_time_ms;
} CDBUF;

typedef	struct
{
	unsigned char	typeId;
	unsigned char	cot;
	unsigned int	pnt;
} MIRBUF;

typedef	struct
{
	unsigned char	typeId;
	unsigned char	cot;
	unsigned char	qoi;
	unsigned char	ack_flag;
} ICGROUPBUF; 

typedef	struct
{
	unsigned char	typeId;
	unsigned char	cot;
	unsigned char	qoi;
	unsigned char	ack_flag;
} CIGROUPBUF; //counter interrogation

typedef	struct
{
	unsigned char	typeId;
	unsigned char	cot;
	unsigned char	qoi;
	unsigned char	ack_flag;
} CSBUF; //clock sync



typedef struct
{
	int	header_ptr;
	char	buf[DSP_INFO_SIZE];
}IEC101S_DSP_BUF;



HANDLE ComInit(char *COM,DWORD Baud_Rate,BYTE ByteSize,BYTE Parity,BYTE StopBits); 
void	IEC870_5_Recv(int, unsigned int recvdatapt, USHORT recv_cmmaddr);
void *	IEC101_SOE(void *arg);
int		Com_Link(int stn_id);
/*void	LanBcCmdError(unsigned char dev,unsigned char type,unsigned short p,unsigned short stat);
*/
int Get_Cmd_Ctrl_Priv(char * interlockname,unsigned char stat,DMS_COMMON *pdbentry);
int Get_Setpt_Ctrl_Priv(char * interlockname);
int	UpdateChannelFlag(int pnum,unsigned short status);
BOOL GetCtrlPrvg(int stn_id);

int	RespondLinkStatus(void);
int	AckCmd(void);
int	NAckCmd(void);
int	NoRequestData(void);
int	SingleRespondByte(void);
int	RepeatTransmit(void);
int	CmdRespSend(void);
int	SERespSend(void);
int	PMRespSend(void);
int	MirrorNAck(unsigned char cause,unsigned int pnt);
int	SinglePointCtrlAck(unsigned char cause,unsigned short address,unsigned char stat);
int	DoublePointCtrlAck(unsigned char cause,unsigned short address,unsigned char stat);
int	RegulatStepCtrlAck(unsigned char cause,unsigned short address,unsigned char stat);
int	SetPointCmdNAAck(unsigned char cause,unsigned short address,unsigned short value,unsigned char se);
int	SetPointCmdNBAck(unsigned char cause,unsigned short address,unsigned short value,unsigned char se);
int	SetPointCmdNCAck(unsigned char cause,unsigned short address,float value,unsigned char se);
int	SetPointCmdNDAck(unsigned char cause,unsigned char num,unsigned int pnt,unsigned char se);
int	ParamValueNAAck(unsigned char cause,unsigned char qpm,unsigned short address,unsigned short value);
int	ParamValueNBAck(unsigned char cause,unsigned char qpm,unsigned short address,unsigned short value);
int	ParamValueNCAck(unsigned char cause,unsigned char qpm,unsigned short address,float value);

int	ClockSynchroniseAck(void);
int	TimeDelayAck(void);
int	GeneralInterrogationAck(unsigned char gi_cause,unsigned char gi_qoi);
int	CounterInterrogationAck(unsigned char gi_cause,unsigned char gi_qcc);
int	RespondTestAck(void);
int	ResetProcessAck(void);
int	ReadCmdAck(unsigned char qoc,unsigned short infoaddr);
int	ReadCmdResp(unsigned short infoaddr);

int	SysInform(unsigned short info_addr);
int	SinglePointInform(unsigned char gi_cause);
int	CheckSinglePointChange(void);
int	ReadSingleInform(unsigned short infoaddr);
int	SinglePointInformCyc(unsigned char gi_cause,unsigned short infogroup);
int	SinglePointInformTA(unsigned char gi_cause);
int	SinglePointInformTB(unsigned char gi_cause);
int	SinglePointGroupCyc(unsigned char gi_cause,unsigned short infogroup);
int	DoublePointInform(unsigned char gi_cause);
int	CheckDoublePointChange(void);
int	ReadDoubleInform(unsigned short infoaddr);
int	DoublePointInformCyc(unsigned char gi_cause,unsigned short infogroup);
int	DoublePointInformTA(unsigned char gi_cause);
int	DoublePointInformTB(unsigned char gi_cause);
int	StepPositionInform(unsigned char gi_cause);
int	CheckStepPositionChange(void);
int	ReadStepPositionInform(unsigned short infoaddr);
int	StepPositionInformCyc(unsigned char gi_cause,unsigned short infogroup);
int	StepPositionInformTA(unsigned char gi_cause);
int	StepPositionInformTB(unsigned char gi_cause);
int	MeasureValueNA(unsigned char gi_cause);
int	CheckMeasureValueNA(void);
int	MeasureValueNACyc(unsigned char gi_cause,unsigned short infogroup);
int	CyclicSendNA(unsigned short infogroup);
int	ReadMeasureValueNA(unsigned short infoaddr);
int	MeasureValueNB(unsigned char gi_cause);
int	CheckMeasureValueNB(void);
int	MeasureValueNBCyc(unsigned char gi_cause,unsigned short infogroup);
int	CyclicSendNB(unsigned short infogroup);
int	ReadMeasureValueNB(unsigned short infoaddr);
int	MeasureValueNC(unsigned char gi_cause);
int	CheckMeasureValueNC(void);
int	MeasureValueNCCyc(unsigned char gi_cause,unsigned short infogroup);
int	CyclicSendNC(unsigned short infogroup);
int	ReadMeasureValueNC(unsigned short infoaddr);
int	MeasureValueND(unsigned char gi_cause);
int	MeasureValueNDCyc(unsigned char gi_cause,unsigned short infogroup);
int	CyclicSendND(unsigned short infogroup);
int	MeasureValueTA(unsigned char gi_cause);
int	MeasureValueTD(unsigned char gi_cause);
int	MeasureValueTB(unsigned char gi_cause);
int	MeasureValueTE(unsigned char gi_cause);
int	MeasureValueTC(unsigned char gi_cause);
int	MeasureValueTF(unsigned char gi_cause);
int	IntegratedTotals(unsigned char gi_cause,unsigned short infogroup);
int	IntegratedTotalsTA(unsigned char gi_cause,unsigned short infogroup);
int	IntegratedTotalsTB(unsigned char gi_cause,unsigned short infogroup);
int	EndofInitialization(void);
int	RespondGroupInterrogation(unsigned char gi_qoi);
void	RespondGeneralInterrogation(void);
void	ScanDataTransmit(void);
int	RespondParamInterrogation(void);
int	RespondCountGroupInterrogation(unsigned char gi_qcc);
void	RespondCountInterrogation(void);
void	RespondUserDataClass1(void);
int	CheckACD(void);
void	RespondUserDataClass2(void);
int	SequenceofEvent(void);

int		SendData(void);
void	ClearRecvFault(void);
void	ClearSendRespFault(void);
void	PrintSendData(unsigned int nbsend);
void	PrintRecvData(unsigned int nbrecv);

void	InitConfig(char *);

int	ReadStnDef(char * pStnDefFile);
int	GenConfigTable(char * pName,char * pDefFile);

int	GenSPTable(char * pName,char *pDefFile);	
int	GenDPTable(char * pName,char *pDefFile);	
int	GenMETable(char * pName,char *pDefFile);
int	SaveMETable(char * pName,char *pDefFile);
int	GenSTTable(char * pName,char *pDefFile);
int	GenITTable(char * pName,char *pDefFile);
int	GenSCTable(char * pName,char *pDefFile);
int	GenDCTable(char * pName,char *pDefFile);
int	GenRCTable(char * pName,char *pDefFile);
int	GenSETable(char * pName,char *pDefFile);
int	GenCYCTable(char * pName,char *pDefFile);
void	ComdSendToLCU(char type_id,char cmd_stat,DMS_COMMON *pdbcomm);
void	SEComdSendToLCU(char type_id, float value, DMS_COMMON *pdbcomm);
void CmdInterlockFailBroadcast(char type_id,char cmd_stat,DMS_COMMON *pdbcomm);
void SECmdInterlockFailBroadcast(char type_id, float value, DMS_COMMON *pdbcomm);

int	GetSPStatById(POINTER *ptr,unsigned char *pstat);
int	GetDPStatById(POINTER *ptr,unsigned char *pstat);
int	GetSTStatById(POINTER *ptr,unsigned char *pvti,unsigned char *pstat);
int	GetMEValueById(POINTER *ptr,unsigned char *pstat,float *pValue);
int	GetIMPValueById(POINTER *ptr,unsigned char *pstat,unsigned int *pValue);

int	GetSPStatByName(char *pName,unsigned char *pstat);
int	GetDPStatByName(char *pName,unsigned char *pstat);
int	GetSTStatByName(char *pName,unsigned char *pvti,unsigned char *pstat);
int	GetMEValueByName(char *pName,unsigned char *pstat,float *pvalue);
int	GetIMPValueByName(char *pName,unsigned char *pstat,unsigned int *pvalue);
short	GetNormalizedValue(float value,float lorange,float hirange,float scale);



