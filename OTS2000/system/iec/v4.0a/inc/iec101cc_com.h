//////////////////////////////////////////////////////////////////////
//
// iec101cc_com.h: Define, structure and function for CIEC101Slave Class
//					of IEC870-5-101 Slave Station.
//
//////////////////////////////////////////////////////////////////////
//#pragma once

#include <sys/types.h>  
#include <errno.h>  
#include <signal.h>  
#include <fcntl.h>  
#include <termios.h>
/*
#include "/usr/gwaysw/gateway/v1.0a/inc/stdafx.h"
#include "/usr/gwaysw/gateway/v1.0a/inc/PAU_LogFile.h"
#include "/usr/gwaysw/gateway/v1.0a/inc/PAU_SharedMemory.h"
#include "/usr/gwaysw/gateway/v1.0a/inc/PAU_CWatchDog.h"
#include "/usr/gwaysw/gateway/v1.0a/inc/PAU_RealTimeValues.h"
#include "/usr/gwaysw/gateway/v1.0a/inc/PAU_EventFifo.h"
#include "/usr/gwaysw/gateway/v1.0a/inc/PAU_Command.h"
*/
#define NONE  0
#define EVEN  1
#define ODD  2

/*channel number COMM_FAIL_CHN_NUM  0x1FF0(8176) for save the communication link status with remote system*/  
#define COMM_CHN_NUM_INDEX  2
#define CHN_NAME		"IEC101 Link Status With CCDC"

#define EXPALRM  2   /* alarm time to read expect string */  
static int  expalarm = EXPALRM; 
 
void sig_alrm(int signo);
//static volatile sig_atomic_t caught_alrm; 
//static int	g_iAlarmFlag; 

extern CLogFile gLogFile;
extern char   gMsgString[256];
/*
#define	BOOL int
#define	DWORD int
#define	HANDLE int
#define	BYTE char
*/
#define	REC_YF_AGC_CMD

#define Com1_Baud_Rate  2400
#define Com2_Baud_Rate  19200
#define Com3_Baud_Rate  1200
#define Com4_Baud_Rate  600
#define Com5_Baud_Rate  1200
#define Com6_Baud_Rate  1200
#define Com7_Baud_Rate  1200
#define Com8_Baud_Rate  1200
#define Com9_Baud_Rate  1200
#define Com10_Baud_Rate  9600 

//#define COM_PORT	"COM2"
//#define COM_PORT	"COM5"
#define COM_PORT	"ttya03"
#define Com_Baud_Rate	Com3_Baud_Rate

#define	LINKADRESS	1
#define	DATABAND	2
#define	RESOLUTE	0x7FFF
#define COM_CHN		0

#define CONFIG_FILE		"/usr/gwaysw/iec101/v1.0a/def/IEC101CC_CONFIG.DEF"

#define SPDEF_FILE		"/usr/gwaysw/iec101/v1.0a/def/SP_DATA_CC.DEF"
#define DPDEF_FILE		"/usr/gwaysw/iec101/v1.0a/def/DP_DATA_CC.DEF"
#define STDEF_FILE		"/usr/gwaysw/iec101/v1.0a/def/ST_DATA_CC.DEF"
#define BODEF_FILE		"/usr/gwaysw/iec101/v1.0a/def/BO_DATA_CC.DEF"
#define MEDEF_FILE		"/usr/gwaysw/iec101/v1.0a/def/ME_DATA_CC.DEF"
#define ITDEF_FILE		"/usr/gwaysw/iec101/v1.0a/def/IT_DATA_CC.DEF"
#define SCDEF_FILE		"/usr/gwaysw/iec101/v1.0a/def/SC_CTRL_CC.DEF"
#define DCDEF_FILE		"/usr/gwaysw/iec101/v1.0a/def/DC_CTRL_CC.DEF"
#define RCDEF_FILE		"/usr/gwaysw/iec101/v1.0a/def/SC_CTRL_CC.DEF"
#define SEDEF_FILE		"/usr/gwaysw/iec101/v1.0a/def/SE_CTRL_CC.DEF"

//TYPE IDENTIFICATION define:
#define M_SP_NA		1		//Single point without time tag (class 1, IC)
#define M_SP_TA		2		//Single point with time tag (SOE)
#define M_DP_NA		3		//Double point without time tag (class 1, IC)
#define M_DP_TA		4		//double point with time tag (SOE)
#define M_ST_NA		5		//Step point without time tag (class 2, IC)
#define M_ST_TA		6		//Step point with time tag
#define M_BO_NA		7		//Slave station remote terminal status
#define M_BO_TA		8		//Bitstring of 32 bit with time tag

#define M_ME_NA		9		//Measure value without time tag (class 2, IC)
#define M_ME_TA		10		//measure value with time tag
#define M_ME_NB		11		//-measure value without time tag,scaled
#define M_ME_TB		12		//measure value with time tag,scaled

#define M_IT_NA		15		//-integrated totals without time tag
#define M_IT_TA		16		//Integrated totals with time tag

#define M_PS_NA		20		//-status detected block sp information (class 1, IC)
#define M_ME_ND		21		//measure value, normalized without quality descriptor (class 2, IC)

#define M_SP_TB		30		//Single point with time tag CP56Time2a (SOE)
#define M_DP_TB		31		//Doule point with time tag CP56Time2a (SOE)
#define M_ST_TB		32		//Step point with time tag CP56Time2a
#define M_ME_TD		34		//Measure value with time tag CP56Time2a
#define M_IT_TB		37		//Integrated totals with time tag CP56Time2a

#define M_EI_NA		70		//End of initialization

#define M_BD_NA		232		//-BCD code water level (class 2, IC)

#define C_SC_NA		45		//Single point control command
#define C_DC_NA		46		//Double point control command
#define C_RC_NA		47		//Regulating step command
#define C_SE_NA		48		//Set point command normalized value
#define C_SE_NB		49		//Set point command scaled value
#define C_SE_NC		50		//-set point command floating point value
#define C_BO_NA		51		//Bitstring of 32 bit

#define C_IC_NA		100		//Interrogation command
#define C_CI_NA		101		//Counter interrogation command
#define C_RD_NA		102		//-read command
#define C_CS_NA		103		//Clock synchronization command
#define C_TS_NA		104		//Test command
#define C_RP_NA		105		//-reset process command

#define P_ME_NA		110		//Parameter for measurements,normalized
#define P_ME_NB		111		//parameter for measurements,scaled
#define P_AC_NA		113		//-parameter activation

//cause of transmit
#define	PER_CYC		1		//cyclic transmission
#define	BG_SCAN		2
#define	SPONT		3		//spontaneous transmission
#define	INIT		4
#define	REQ			5
#define	ACT			6		//activation C direction
#define	ACTCON		7		//activation confirmation M direction
#define	DEACT		8		//deactivation C direction
#define	DEACTCON	9		//deactivation confirmation M direction
#define	ACTTERM		10		//activation terminal C direction
#define	INTROGEN	20		//interrogation C direction
#define	INTRO1		21		//respond interrogation group 1 SP/DP
#define	INTRO2		22		//respond interrogation group 2 SP/DP
#define	INTRO3		23		//respond interrogation group 3 SP/DP
#define	INTRO4		24		//respond interrogation group 4 SP/DP
#define	INTRO5		25		//respond interrogation group 5 SP/DP
#define	INTRO6		26		//respond interrogation group 6 SP/DP
#define	INTRO7		27		//respond interrogation group 7 SP/DP
#define	INTRO8		28		//respond interrogation group 8 SP/DP
#define	INTRO9		29		//respond interrogation group 9 ME
#define	INTRO10		30		//respond interrogation group 10 ME
#define	INTRO11		31		//respond interrogation group 11 ME
#define	INTRO12		32		//respond interrogation group 12 ME

#define	REQCOGCN	37		//respond integrated Total
#define	REQCO1		38		//respond integrated Total group 1
#define	REQCO2		39		//respond integrated Total group 2
#define	REQCO3		40		//respond integrated Total group 3
#define	REQCO4		41		//respond integrated Total group 4

#define	ACK_CMD		0		//ACK the Command
#define	NACK_CMD	1		//NACK theCommand

#define FIXFRAMELEN	5
#define VARFRAMELEN	256
#define MINFRAMELEN	15

#define CMD_ENTRY_NUMBER	10
#define CMD_DATA_SIZE		36
#define ASDU_ENTRY_NUMBER	100
#define ASDU_DATA_SIZE		256

#define	MAX_SP_NUM		512
#define	MAX_DP_NUM		256
#define	MAX_ME_NUM		256
#define	MAX_ST_NUM		32
#define	MAX_BO_NUM		32
#define	MAX_IT_NUM		64

#define	MAX_SC_NUM		100
#define	MAX_DC_NUM		100
#define	MAX_RC_NUM		100
#define	MAX_SE_NUM		100
#define	MAX_BC_NUM		255
#define	MAX_PM_NUM		256

#define	SC_CMD_BASE_ADDR	0xB01
#define	DC_CMD_BASE_ADDR	0xB01
#define	RC_CMD_BASE_ADDR	0xB01
#define	SE_CMD_BASE_ADDR	0xB81
#define	BC_CMD_BASE_ADDR	0xCA1

#define	SP_BASE_ADDR	0x0001
#define	DP_BASE_ADDR	0x1001
#define	ME_BASE_ADDR	0x4000
#define	PM_BASE_ADDR	0x901
#define	IT_BASE_ADDR	0xC01
#define	ST_BASE_ADDR	0xC81
#define	BO_BASE_ADDR	0xCA1

#define	ME_GROUP_NUM	64
#define	SP_GROUP_NUM	64
#define	DP_GROUP_NUM	64
#define	ST_GROUP_NUM	32
#define	IT_GROUP_NUM	32
#define	BO_GROUP_NUM	32

#define	NAME_SIZE		36

typedef struct         // command data structure ASDU
{
	unsigned short	nPnt;
	unsigned short	nLength[CMD_ENTRY_NUMBER];
	unsigned char	nData[CMD_ENTRY_NUMBER][CMD_DATA_SIZE];
}ASDUCOMDDATA;
/*
typedef struct         // received data structure ASDU
{
	unsigned short	nPnt;
	unsigned short	nLength[ASDU_ENTRY_NUMBER];
	unsigned char	nData[ASDU_ENTRY_NUMBER][ASDU_DATA_SIZE];
}ASDUDATA;
*/
typedef	struct
{
	unsigned short nNumber;
	unsigned short nChannelNo[MAX_SP_NUM];
} IEC101_SPDEF;

typedef	struct
{
	unsigned short nNumber;
	unsigned short nChannelNo[MAX_DP_NUM];
	unsigned short nChannelNo1[MAX_DP_NUM];
} IEC101_DPDEF;

typedef	struct
{
	unsigned short nNumber;
	float	fLoRange[MAX_ME_NUM];
	float	fHiRange[MAX_ME_NUM];
	unsigned short nChannelNo[MAX_ME_NUM];
} IEC101_MEDEF;

typedef	struct
{
	unsigned short nNumber;
	unsigned short nChannelNo[MAX_ST_NUM];
} IEC101_STDEF;

typedef	struct
{
	unsigned short nNumber;
	unsigned short nChannelNo[MAX_BO_NUM];
} IEC101_BODEF;

typedef	struct
{
	unsigned short nNumber;
	unsigned short nChannelNo[MAX_IT_NUM];
} IEC101_ITDEF;

typedef	struct
{
	unsigned short nNumber;
	unsigned short nChannelNo[MAX_SC_NUM];
} IEC101_SCDEF;

typedef	struct
{
	unsigned short nNumber;
	unsigned short nChannelNo[MAX_DC_NUM];
} IEC101_DCDEF;

typedef	struct
{
	unsigned short nNumber;
	float	fLoRange[MAX_SE_NUM];
	float	fHiRange[MAX_SE_NUM];
	unsigned short nChannelNo[MAX_SE_NUM];
} IEC101_SEDEF;

typedef	struct
{
	unsigned short nNumber;
	float	fLoRange[MAX_PM_NUM];
	float	fHiRange[MAX_PM_NUM];
	unsigned short nChannelNo[MAX_PM_NUM];
} IEC101_PMDEF;

typedef	struct
{
	unsigned short nNumber;
	unsigned short nChannelNo[MAX_RC_NUM];
} IEC101_RCDEF;

typedef	struct
{
	unsigned short nNumber;
	unsigned short nChannelNo[MAX_BC_NUM];
} IEC101_BCDEF;

class CIEC101Slave
{
private:

	unsigned char	m_nRecvBuf[1024];
	unsigned char	m_nSendBuf[1024];
	unsigned char	m_nCmdSendBuf[CMD_ENTRY_NUMBER];

	ASDUCOMDDATA	m_sRecvCmdData;

	short		m_iOrgAnaBuf[MAX_ME_NUM];
	unsigned char	m_nOrgAnaQlt[MAX_ME_NUM];
	
	unsigned char	m_nOrgSPbuf[MAX_SP_NUM];
	unsigned char	m_nOrgDPbuf[MAX_DP_NUM];
	unsigned char	m_nOrgDPbuf1[MAX_DP_NUM];
	
	int		m_iOrgITBuf[MAX_IT_NUM];
	unsigned char	m_nOrgITQlt[MAX_IT_NUM];


	int	m_dwByteToSend;	
	int	m_dwSendByte;

	bool	m_bFCB;
	bool	m_bACD;
	bool	m_bDFC;
	
//	int	m_iAlarmFlag;

	int	m_iCmdTermFlag;
	int	m_iCounterFlag;
	int	m_iInterrogationFlag;
	int	m_iNoChangeData;
	int	m_iLocalInitFlag;
	int	m_iCmdAckFlag;

	unsigned short	m_nDataband;

	unsigned short	m_nCmdSelAddr;

	unsigned char	m_cRespGenInterroGrpNum;
	unsigned char	m_cRespCountGrpNum;

	unsigned int	m_nEventPtr;
	SEVENT		m_sEvent;

	unsigned char	m_nStnId;
	char			m_strDev[100];
	int				m_nBaudRate;

	IEC101_SPDEF	m_sSPDef;
	IEC101_DPDEF	m_sDPDef;
	IEC101_MEDEF	m_sMEDef;
	IEC101_ITDEF	m_sITDef;
	IEC101_STDEF	m_sSTDef;
	IEC101_BODEF	m_sBODef;
	IEC101_SEDEF	m_sSEDef;
	IEC101_PMDEF	m_sPMDef;
	IEC101_DCDEF	m_sDCDef;
	IEC101_SCDEF	m_sSCDef;
	IEC101_RCDEF	m_sRCDef;
	IEC101_BCDEF	m_sBCDef;

public:

	unsigned short m_nPID;
	int	m_fd_com;

#ifdef REC_YF_AGC_CMD
	unsigned short	m_nDaySaved;
#endif

//Pointer to log file class CLogFile

//	CLogFile	*m_pCLogFile;

//Pointer to Real-time value shared memory class CRealTimeValues

	CRealTimeValues	*m_pCRTValue;

//Pointer to Event shared memory class CEventFifo;

	CEventFifo *m_pCEvent;

//Pointer to Command shared memory class CCommandFifo

	CCommandFifo *m_pCCommand;

//Pointer to Watchdog shared memory class CWatchDog

	CWatchDog *m_pCWatchdog;

	CIEC101Slave(unsigned short nPid)
	{
		m_nPID = nPid;
		InitIEC101ComData();
	}

	CIEC101Slave(void)
	{
		m_nPID = 0;
		InitIEC101ComData();
	}

	~CIEC101Slave(void)
	{
		delete m_pCRTValue;
		delete m_pCEvent;
		delete m_pCCommand;
		delete m_pCWatchdog;
	}

	void InitIEC101ComData(void);

	int ComInit(char *Com, int Baud_Rate,int ByteSize,int Parity,int modem);

	bool	IEC101SlaveCom(void);

private:

	void	IEC870_5_Recv(unsigned int recvdatapt);

	bool	RespondLinkStatus(void);
	bool	ResetLinkStatus(void);
	bool	AckCommand(unsigned char nAck);
	bool	NoRequestData(void);
	bool	SingleRespondByte(void);
	bool	RepeatTransmit(void);

	bool	SinglePointCtrlAck(void);
	bool	DoublePointCtrlAck(void);
	bool	RegulatStepCtrlAck(void);
	bool	SetPointCmdNAAck(void);
	bool	SetPointCmdNBAck(void);
	bool	BitStringCtrlAck(void);
	bool	ParamValueNAAck(void);
	bool	ParamValueNBAck(void);

	bool	ClockSynchroniseAck(void);
	bool	ClockSynchroniseAckResp(unsigned int nPnt);
	bool	GeneralInterrogationAck(void);
	bool	GeneralInterrogationAckResp(unsigned char gi_cause);
	bool	CounterInterrogationAck(void);
	bool	CounterInterrogationAckResp(unsigned char gi_cause);

	bool	GetUserDataClass1(SEVENT &sEvent);
	void	RespondUserDataClass1(void);
	void	RespondUserDataClass2(void);

	bool	RespondTestAck();

	bool	RespondGroupInterrogation(unsigned char	gi_cause);
	void	RespondGeneralInterrogation(void);
	bool	RespondCountGroupInterrogation(unsigned char gi_cause);
	void	RespondCountInterrogation(void);

	bool	SinglePointInformCyc(unsigned char gi_cause, unsigned char infogroup);
	bool	SinglePointInform(void);
	bool	SinglePointInformTA(void);
	bool	SinglePointInformTB(void);
	bool	DoublePointInformCyc(unsigned char gi_cause, unsigned char infogroup);
	bool	DoublePointInform(void);
	bool	DoublePointInformTA(void);
	bool	DoublePointInformTB(void);
	bool	StepPositionInformCyc(unsigned char gi_cause, unsigned char infogroup);
	bool	StepPositionInform(void);
	bool	StepPositionInformTA(void);
	bool	StepPositionInformTB(void);
	bool	Bitstringof32Bit(void);
	bool	Bitstringof32BitTA(void);
	bool	IntegratedTotalsCyc(unsigned char gi_cause, unsigned char infogroup);
	bool	IntegratedTotals(void);
	bool	IntegratedTotalsTA(void);
	bool	IntegratedTotalsTB(void);
	bool	MeasureValueNACyc(unsigned char gi_cause,unsigned char infogroup);
	bool	MeasureValueNA(void);
	bool	MeasureValueNBCyc(unsigned char gi_cause,unsigned char infogroup);
	bool	MeasureValueNB(void);
	bool	MeasureValueNDCyc(unsigned char gi_cause,unsigned char infogroup);
	bool	MeasureValueND(void);
	bool	MeasureValueTA(void);
	bool	MeasureValueTB(void);
	bool	MeasureValueTD(void);

	bool	EndofInitialization(void);

	void	ClearRecvFault(void);
	void	ClearSendRespFault(void);
	void	PrintRecvData(unsigned int nbsend);
	void	PrintSendData(unsigned int nbsend);
	void	PrintSendCmdData(unsigned int nbsend);
	float	Datcvt(unsigned short status, unsigned short data);

	bool	GenConfigTable(char *pDefFile);
	bool	GenSPTable(char *pDefFile);		
	bool	GenDPTable(char *pDefFile);		
	bool	GenMETable(char *pDefFile);
	bool	GenStepTable(char *pDefFile);		
	bool	GenITTable(char *pDefFile);
	bool	GenSCTable(char *pDefFile);
	bool	GenDCTable(char *pDefFile);
	bool	GenRCTable(char *pDefFile);
	bool	GenBCTable(char *pDefFile);
	bool	GenSetPCtrlTable(char *pDefFile);
	bool	GenParamTable(char *pDefFile);
	void	CrossCopy(void *strDest, const void *strSrc, size_t nNum);

};


