/************************************************************************/
/* TASE.2 SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright BITC                                                 */
/*          2006-2009, All Rights Reserved                              */
/*									*/
/* MODULE NAME : iccp_main.h  						*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*									*/
/* MODULE DESCRIPTION : main header file                                */
/*                                                                      */
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev                     Comments                     */
/* --------  ---  ------   -------------------------------------------  */
/* 12/15/06  zy     1      Created                                      */
/************************************************************************/
#ifndef ICCP_MAIN_INCLUDED
#define ICCP_MAIN_INCLUDED

#include 	<sys/types.h>
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

#include	"../../../dms/v4.0a/inc/dms_com.h"
/*
#include <thread.h>
#include <pthread.h>
*/

#ifdef _AIX               //added by dm
#include <sys/wait.h>
#include <errno.h>
#include <sys/time.h>
#elif defined(SUN_OS)
#include <errno.h>	/*added 2003.4.11 zy */
#include <wait.h>
#include <time.h>
#endif

//#ifdef SUN_OS
//#include <errno.h>	/*added 2003.4.11 zy */
//#include <wait.h>
//#include <time.h>
//#else
//#include	<sys/time.h>
//#endif

#include	"iccp_data.h"
#include	"/DPSINC/ipc_common.h"
#include	"/DPSINC/dps_type_def.h"
#include	"/DPSINC/dps_rec_def.h"
#include	"/LANINC/lan_common.h"


#define	REC_YF_AGC_CMD

#define DEBUG



#define byte   unsigned char
#define FLAGS  unsigned long int

#define	NAME_SIZE	36


/* ---- configuration and program files ---- */
#define ICCP_DATADEF_NAM           "/home/ems/h9000/iccp/ICCPDataDef.dat"
/*
#define CONF_IMSDATASETNAME_NAM    "/home/ems/h9000/iccp/IMSDataSetName.ini"
#define DAT_IMSDATASET_NAM         "/tmp/IMSDataSet.dat"
*/
#define PRC_FIL_GWDC               "/home/ems/h9000/bin/iccp_main.dxe"
/*
#define ICCP_DEF_PATH              "/home/ems/h9000/iccp/"
*/
#define HOMEPATH          	   "/home/ems/h9000/iccp/"
#define ICCP_SCH_PATH              "/home/ems/h9000/iccp/sch/"
#define ICCP_ACK_PATH              "/home/ems/h9000/iccp/ack/"
#define ICCP_TMP_PATH              "/home/ems/h9000/iccp/tmp/"
#define ICCP_STATUS_FILE           "TASE2Status.txt"
#define ICCP_ACK_FILELIST          "ackfilelist.txt"

#define MIT_XML_OSI_CFG_FILE	   "/home/ems/h9000/iccp/osicfg.xml"
#define MIT_XML_MIT_CFG_FILE	   "/home/ems/h9000/iccp/iccpcfg.xml"
#define MIT_XML_MIT_LOG_FILE	   "/home/ems/h9000/iccp/logcfg.xml"

#define MIT_XML_OSI_CFG_FILE_S	   "/home/ems/h9000/iccp/osicfg_s.xml"
#define MIT_XML_MIT_CFG_FILE_S	   "/home/ems/h9000/iccp/iccpcfg_s.xml"
#define MIT_XML_MIT_LOG_FILE_S	   "/home/ems/h9000/iccp/logcfg_s.xml"

#define	DSP_INFO_SIZE	102400


#define M_SP_TB		30	/* Single point with time tag CP56Time2a */
#define M_DP_TB		31	/* Double point with time tag CP56Time2a */

#define MAX_SIGNAL_NUM  32768

#define TYPE_MEASUREMENT    0x01
#define TYPE_INDICATION     0x02
#define TYPE_COMMAND        0x04
#define TYPE_SETPOINT       0x08

#define ID_OFFSET_MEASUREMENT
#define ID_OFFSET_INDICATION
#define ID_OFFSET_COMMAND
#define ID_OFFSET_SETPOINT

#define DATASRC_101_APPC   0x01      /* Application Controller */
#define DATASRC_101_DC0    0x11      /* Dispatch Center 0 (Cascade DC) - not used */
#define DATASRC_101_DC1    0x12      /* Dispatch Center 1 (National DC) */
#define DATASRC_101_DC2    0x13      /* Dispatch Center 2 (Central China DC) */
#define DATASRC_IMS        0x21      /* IMS */
#define DATASRC_APPWS      0x22      /* Application Workstation */
#define DATASRC_TASE2_DC0  0x31      /* Dispatch Center */
#define DATASRC_TASE2_DC1  0x32      /* Dispatch Center */
#define DATASRC_TASE2_DC2  0x33      /* Dispatch Center */

#define DATATYPE_M_DISCRETE     1    /* Measurement */
#define DATATYPE_M_REAL         2    /* Measurement */
#define DATATYPE_M_STATE        3    /* Measurement */
#define DATATYPE_I_SINGLE       4    /* Indication - Single */
#define DATATYPE_I_DOUBLE       5    /* Indication - Double */
#define DATATYPE_CMD                 /* Command */
#define DATATYPE_SP_DISCRETE         /* Setpoint */
#define DATATYPE_SP_REAL             /* Setpoint */
#define DATATYPE_INFO_MSG            /* Information Message */

#define MSG_TYP_ERR     0x01
#define MSG_TYP_WARN    0x02
#define MSG_TYP_INFO    0x03

#define PRC_NUM_GWMAIN      1
#define PRC_NUM_GWCNTRL     2
#define PRC_NUM_GWAPPWS    11
#define PRC_NUM_GWDC0      31
#define PRC_NUM_GWDC1      32
#define PRC_NUM_GWDC2      33
#define PRC_NUM_GWIMS      21
#define PRC_NUM_GWIEC101   41   /* modified 31 to 41 2003.7.16 zy */
#define PRC_NUM_GWPRT      63


#define INFOMSG_REF_SOE			10100	// added by yyp 2011.10.06
#define INFOMSG_REF_SCHEDULE	10101	

#define CHANPRIV_WAIT		0
#define CHANPRIV_TEST		1
#define CHANPRIV_WORK		2
#define CHANPRIV_ONDUTY		3
#define CHANPRIV_REPAIR		6

#define CHANSTAT_FAIL		0
#define CHANSTAT_TEST		1
#define CHANSTAT_WORK		2
#define CHANSTAT_ONDUTY		3



typedef struct sGWAssociation
{
	char            strAssocNam[64];
	char            strRemoteNam[64];
	MIT_ASSOC_INFO *pAssocInf;
	MIT_MIS_VCC    *pMitMisVcc;
	int             nSta;
}sGWAssoc;


/*------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------*/
typedef struct GWTimeStamp {
  long int   nTim;
  short int  nMSec;
} sGWTimeStamp;



/*------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------*/
typedef struct GWSignalValue {
  byte                  nTyp;       /* Measurement / Indication / Command / Setpoint */
  byte                  cVal[33];           /* actual value to transmit */
/*  bool                  bQFlg; */
  int                  bQFlg;
  byte                  cTim[6];    /* Timestamp */
  struct GWSignalValue *pNxt;
} sGWSignalValue;


/*------------------------------------------------------------------------------
// internal communication buffer for interface ICCP/TASE.2 <-> IEC 101 
//-----------------------------------------------------------------------------*/
typedef struct GWSignalTASE2 {
  int          nFct;
  byte         nDst;
  char         strDCNam[33];
  FLAGS        nType;
  byte         nValBits;
  long int     nVal;
  double       rVal;
  FLAGS        nValidity;
  FLAGS        nCurrentSource;
  FLAGS        nNormalValue;
  FLAGS        nTimeStampQuality;
  FLAGS        nOverflow;
  sGWTimeStamp sTimeStamp;
} sGWSigTASE2;

/*------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------*/
#define StartAssociations                      11
#define StopAssociations                       12
#define GetAssociationSummary                  13
  
#define ReadRemoteDataValues                   21
#define WriteRemoteDataValues                  22
#define GetRemoteDataValueNames                23
#define GetRemote DataValuesAttributes         24
#define ReadRemoteDataSet                      25
#define WriteRemoteDataSet                     26
#define CreateRemoteDataSet                    27
#define DeleteRemoteDataSet                    28
#define GetRemoteDataSetElementNames           29
  
#define DisplayLocalDataValue                  31
#define ModifyLocalDataValue                   32
#define ReconfigureDataValueChanedCondition    33
#define GetLocalDataValueNames                 34
#define GetLocalDataSetNames                   35
  
#define StartTransferSets                      41
#define StopTransferSets                       42
  
#define StartInformatonMessages                51
#define StopInformationMessages                52
#define SendInformationMessage                 53
  
#define SelectDevice                           61
#define OperateDevice                          62
#define SetDeviceTag                           63
#define GetDeviceTag                           64
#define GetLocalDevices                        65
  
#define GetConfigurationRemotes                71
#define GetRemoteDateValueNames                72
#define GetRemoteDataSetNames                  73
#define GetRemoteDataSetAttributes             74
#define GetRemoteTransferSetAttributes         75
#define GetRemoteInfoMessages                  76
#define GetRemoteDevices                       77
  
#define DynMemCheck                            81
#define GetStatistics                          82
#define SetDebugLevels                         83
#define ScreenLogOn                            84
#define ScreenLogOff                           85
#define GetSizes                               86
#define ReconfigureChangeMode                  87
#define ModifyAddressMatching                  88



typedef struct GWTASE2Function11 {    /* start association(s) */
  short int  nFctId;
} sGWTase2Fct11;

typedef struct GWTASE2Function12 {    /* stop association(s) */
  short int  nFctId;
}sGWTase2FCT12;

typedef struct GWTASE2Function13 {    /* association summary */
  short int  nFctId;
}sGWTase2FCT13;

typedef struct GWTASE2Function21 {    /* remote variables - get data value(s) */
  short int  nFctId;
}sGWTase2FCT21;

typedef struct GWTASE2Function22 {    /* remote variables - set data value(s) */
  short int  nFctId;
}sGWTase2FCTr22;

typedef struct GWTASE2Function23 {    /* remote variables - get data value name */
  short int  nFctId;
}sGWTase2FCT23;

typedef struct GWTASE2Function24 {    /* remote variables - get data value type */
  short int  nFctId;
}sGWTase2FCT24;

typedef struct GWTASE2Function25 {    /* remote variables - get data set element values */
  short int  nFctId;
}sGWTase2FCT25;

typedef struct GWTASE2Function26 {    /* remote variables - set data set element values */
  short int  nFctId;
}sGWTase2FCT26;

typedef struct GWTASE2Function27 {    /* remote variables - create date set */
  short int  nFctId;
}sGWTase2FCT27;

typedef struct GWTASE2Function28 {    /* remote variables - delete data set */
  short int  nFctId;
}sGWTase2FCT28;

typedef struct GWTASE2Function29 {    /* remote variables - get data set element names */
  short int  nFctId;
}sGWTase2FCT29;

typedef struct GWTASE2Function31 {    /* local variables - display local variable value(s) */
  short int  nFctId;
}sGWTase2FCT31;

typedef struct GWTASE2Function32 {    /* local variables - modify local data values */
  short int  nFctId;
}sGWTase2FCT32;

typedef struct GWTASE2Function33 {    /* local variables -  modify local data value change condition */
  short int  nFctId;
}sGWTase2FCT33;

typedef struct GWTASE2Function34 {    /* local variables - get local data value names */
  short int  nFctId;
}sGWTase2FCT34;

typedef struct GWTASE2Function35 {    /* local variables - get local data set names */
  short int  nFctId;
}sGWTase2FCT35;

typedef struct GWTASE2Function41 {    /* start transfers */
  short int  nFctId;
}sGWTase2FCT41;

typedef struct GWTASE2Function42 {    /* stop transfers */
  short int  nFctId;
}sGWTase2FCT42;

typedef struct GWTASE2Function51 {    /* start information messages (client) */
  short int  nFctId;
}sGWTase2FCT51;

typedef struct GWTASE2Function52 {    /* stop information maessages (client) */
  short int  nFctId;
}sGWTase2FCT52;

typedef struct GWTASE2Function53 {    /* send message (server) */
  short int  nFctId;
}sGWTase2FCT53;

typedef struct GWTASE2Function61 {    /* device control - select operation */
  short int  nFctId;
}sGWTase2FCT61;

typedef struct GWTASE2Function62 {    /* device control - operate operation */
  short int  nFctId;
}sGWTase2FCT62;

typedef struct GWTASE2Function63 {    /* device control - set tag */
  short int  nFctId;
}sGWTase2FCT63;

typedef struct GWTASE2Function64 {    /* device control - get tag */
  short int  nFctId;
}sGWTase2FCT64;

typedef struct GWTASE2Function65 {    /* device control - get local devices */
  short int  nFctId;
}sGWTase2FCT65;

typedef struct GWTASE2Function71 {    /* configuration - get configures remotes */
  short int  nFctId;
}sGWTase2FCT71;

typedef struct GWTASE2Function72 {    /* configuration - get remote data value names */
  short int  nFctId;
}sGWTase2FCT72;

typedef struct GWTASE2Function73 {    /* configuration - get remote data set names */
  short int  nFctId;
}sGWTase2FCT73;

typedef struct GWTASE2Function74 {    /* configuration - get remote data set attributes */
  short int  nFctId;
}sGWTase2FCT74;

typedef struct GWTASE2Function75 {    /* configuration -  ger remote transfer set attributes */
  short int  nFctId;
}sGWTase2FCT75;

typedef struct GWTASE2Function76 {    /* configuration - get remote info messages */
  short int  nFctId;
}sGWTase2FCT76;

typedef struct GWTASE2Function77 {    /* configuration - get remote devices */
  short int  nFctId;
}sGWTase2FCT77;

typedef struct GWTASE2Function81 {    /* misc - check allocated memory pointers */
  short int  nFctId;
}sGWTase2FCT81;

typedef struct GWTASE2Function82 {    /* misc - get iccp statistics */
  short int  nFctId;
}sGWTase2FCTR82;

typedef struct GWTASE2Function83 {    /* misc - set iccp-lite debug levels */
  short int  nFctId;
}sGWTase2FCT83;

typedef struct GWTASE2Function84 {    /* misc - enable log messages to screen */
  short int  nFctId;
}sGWTase2FCT84;

typedef struct GWTASE2Function85 {    /* misc - disable log messages to screen */
  short int  nFctId;
}sGWTase2FCT85;

typedef struct GWTASE2Function86 {    /* misc -  get sizes */
  short int  nFctId;
}sGWTase2FCT86;

typedef struct GWTASE2Function87 {    /* misc - reconfigure data value changes */
  short int  nFctId;
}sGWTase2FCT87;

typedef struct GWTASE2Function88 {    /* misc -  modify address matching */
  short int  nFctId;
}sGWTase2FCT88;


#define	REC_YF_AGC_CMD
#define	LINE_BUF_CHAR	256
#define	O_NAME_SIZE	60	/*36*/

#define	true	(int)1
#define	false	(int)0

#define	cmdrecbuf		"/home/ems/h9000/his/iccp/CMD_"

#define SPDEF_FILE		"ICCP_SP_DATA.DEF"
#define DPDEF_FILE		"ICCP_DP_DATA.DEF"
#define MEDEF_FILE		"ICCP_ME_DATA.DEF"
#define SCDEF_FILE		"ICCP_SC_CTRL.DEF"

#define C_DPDEF_FILE		"ICCP_ClientDP_DATA.DEF"
#define C_MEDEF_FILE		"ICCP_ClientME_DATA.DEF"
/*
#define DCDEF_FILE		"ICCP_DC_CTRL.DEF"
*/
#define SEDEF_FILE		"ICCP_SE_CTRL.DEF"



#define SPDEF_FILE_S		"ICCP_S_SP_DATA.DEF"
#define DPDEF_FILE_S		"ICCP_S_DP_DATA.DEF"
#define MEDEF_FILE_S		"ICCP_S_ME_DATA.DEF"
#define SCDEF_FILE_S		"ICCP_S_SC_CTRL.DEF"

/*
#define DCDEF_FILE_S		"ICCP_S_DC_CTRL.DEF"
*/
#define SEDEF_FILE_S		"ICCP_S_SE_CTRL.DEF"





#define	SPONT_ME_FLOAT_NUM	30

#define	SOE_NUM			1024

#define	CMD_SOURCE		3	/* Cascade dispatch center */

typedef	struct
{
	USHORT	number;
	char	name[MAX_SP_NUM][POINT_NAME_SIZE];
	char	iccp_name[MAX_SP_NUM][POINT_NAME_SIZE];
	int	TimeTag[MAX_SP_NUM];
	POINTER	ptr[MAX_SP_NUM];
} SPDEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_DP_NUM][POINT_NAME_SIZE];
	char	iccp_name[MAX_DP_NUM][POINT_NAME_SIZE];
	char	TimeTag[MAX_DP_NUM];
	char	doublept[MAX_DP_NUM];
	POINTER	ptr[MAX_DP_NUM];
} DPDEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_ME_NUM][POINT_NAME_SIZE];
	char	iccp_name[MAX_ME_NUM][POINT_NAME_SIZE];
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
	unsigned char	cmdtype[MAX_SC_NUM];
	unsigned char	optype1[MAX_SC_NUM];
	unsigned char	optype2[MAX_SC_NUM];
	char	name[MAX_SC_NUM][POINT_NAME_SIZE];
	char	iccp_name[MAX_SC_NUM][POINT_NAME_SIZE];
	char	interlockname[MAX_SC_NUM][O_NAME_SIZE];
} CMDDEF;

typedef	struct
{
	USHORT	number;
	float	lorange[MAX_SE_NUM];
	float	hirange[MAX_SE_NUM];
	float	scale[MAX_ME_NUM];
	unsigned char	cmdtype[MAX_SE_NUM];
	char	name[MAX_SE_NUM][POINT_NAME_SIZE];
	char	iccp_name[MAX_SE_NUM][POINT_NAME_SIZE];
	char	interlockname[MAX_SE_NUM][O_NAME_SIZE];
} SEDEF;

typedef	struct
{
	unsigned char	nDst;
	unsigned short	infoaddr;
	unsigned char	stat;
	unsigned char	vti;
	unsigned short	stvalue;
	float		fvalue;
	DATE_TIME	time;
} SOEREC;

typedef	struct
{
	unsigned short	bufpnt;
	unsigned short	sendpnt;
	SOEREC	soerec[SOE_NUM];
} SOEBUF;

typedef struct
{
	int	header_ptr;
	char	buf[DSP_INFO_SIZE];
}SHM_BUF;



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


int	Net_Com(int pnum,int sockfd);

int Get_Cmd_Ctrl_Priv(char * interlockname,unsigned char stat,DMS_COMMON *pdbentry);
int Get_Setpt_Ctrl_Priv(char * interlockname);
int	UpdateChannelFlag(int pnum,unsigned short status);

int	SendSinglePointInform();
int	SendDoublelePointInform();
int	SendSinglePointInformCyc();
int	SendDoublePointInformCyc();
int	SendStateInformSOE();

int	SendScheduleAckInform(int nDst, char *Ackfile);

int	SendMeasureValueNC();
int	SendMeasureValueNCCyc();
int	SendUserData(void);
int	WriteMeasureValueNC();

int ICCPSendCmd(void);


int	ClientMEDataProc (char * iccp_name, float Value, ST_UCHAR Flags);
int	ClientSWDataProc (char * iccp_name, ST_UCHAR Flags);
int	ClientStateFromSOEProc (int buf_Size, ST_UCHAR *info_buf);
int	ClientDataBD (void);
int	GWRtuAllSend (void);	

int	GetSPStatById(POINTER *ptr,unsigned char *pstat);
int	GetDPStatById(POINTER *ptr,unsigned char *pstat);
int	GetMEValueById(POINTER *ptr,unsigned char *pstat,float *pValue);

int	GetSPStatByName(char *pName,unsigned char *pstat);
int	GetDPStatByName(char *pName,unsigned char *pstat);
int	GetMEValueByName(char *pName,unsigned char *pstat,float *pValue);
short	GetNormalizedValue(float value,float lorange,float hirange);
int	GetIMPValueByName(char *pName,unsigned char *pstat,unsigned int *pValue);

void InitData(void);
void InitData_S(void);
void InitICCPShm(void);
int GenSPTable(char *pDefFile);		/* single point define file */
int GenDPTable(char *pDefFile);		/* double point define file */
int GenMETable(char *pDefFile);		/* measurement point define file */
int GenClientDPTable(char *pDefFile);		/* Client double point define file */
int GenClientMETable(char *pDefFile);		/* Client measurement point define file */
int GenSCTable(char *pDefFile);		/* single point control command define file */
int GenDCTable(char *pDefFile);		/* double point control command define file */
int GenSETable(char *pDefFile);		/* set point control point define file */

void ICCP_SOE_Init(void);
void ICCP_SOE_Rec(void);

void Call_MI_Service( void );

int ICCPDeviceOperateCmd(int nDst,char *iccp_devName, ST_INT16 cmdstat);
int ICCPSetPointCmd(int nDst,char *iccp_devName, float setValue);

void ComdSendToLCU(char type_id,char cmd_stat,DMS_COMMON *pdbcomm);
void SEComdSendToLCU(char type_id, float value, DMS_COMMON *pdbcomm);
int Get_Cas_Ctrl_Priv(char * ctrl_priv);
void CmdInterlockFailBroadcast(char type_id,char cmd_stat,DMS_COMMON *pdbcomm);
void SECmdInterlockFailBroadcast(char type_id, float value, DMS_COMMON *pdbcomm);
void LanBcCmdError(POINTER ptr,short state);
void WriteDspInfo(char *buf);

#endif  /* ICCP_MAIN_INCLUDED*/
