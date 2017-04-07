/*******************************************************************************************************************************
FILENAME:	
FUNCTION:		

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, XP

HISTORY:
2008.4.10	start to modify the code for H9000 V4.0		zyf
********************************************************************************************************************************/
/*********iec102 by zyf for ankong to connect SYBASE**********/
/************************change for all************************/
/*程序只编写接收遥测中的正向总有功，反向总有功，正向总无功，反向总无功。
  (积分周期/负荷曲线周期=15分钟)*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory.h>
#include <time.h>
#include <windows.h>
#include <winsock.h>     
#include <math.h>
#include <stdlib.h>
#include "string.h"
#include "SQL.H"
#include "SQLEXT.H"

#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include    "../../../cmd/v4.0a/inc/cmd_def.h"
#include    "../../../dms/v4.0a/inc/dms_std.h"
#include    "../../../dms/v4.0a/inc/dms_structure_def.h"
#include    "../../../dms/v4.0a/inc/os_type_def.h"


#define		TRUE	1
#define		FALSE	0
//==================用户定义========================
#define		DEBUG
#define		GetPulse

#define		SlaveTotal		11
#define		AnaNum			10
int         Slavenum;

#ifdef		GetPulse
#define		PulseNum		4
#endif

#define		Com_Baud_Rate   9600  
#define		COMM_PORT		"COM4"
#define		BAUD_RATE		9600    //波特率
#define		PARITY_TYPE		NOPARITY  /* ODDPARITY（奇校验）,  EVENPARITY（偶），NOPARITY */

#define		YC_TABLE		"\\home\\ems\\h9000\\iec102\\def\\iec102_yc_define.txt"

/*******************用于广播的LCU号和数据库起始地址***********************/
#define		H9000_DEV_ID			1	 
#define		H9000_IMP_START_PID		0  

//================================================ 


#define		SEND_PERIOD		2    //秒

#define		TIMEOUT_SECONDS		8
//#define		BUFSIZE				2048
#define		BUFSIZE				256
#define		MAX_YC_NUM			300
#define		MAX_YX_NUM			300
//#define		MAX_BUFFILE_NUM     2048
#define		MAX_BUFFILE_NUM     256

#define		FAILURE				-1
#define		SUCCESS				0

#define LONGNAME_LEN	30


int			ReadDefineFile();	
void		iecHostStation(),DltHostGetTime(),iecHostSetTime(),HostSendReadCmd();
BOOL		HostReadInfoBack(unsigned char *,unsigned char *);

void		HostAnaDataUnpack(int),HostPulseDataUnpack(int,int),TimeDataUnpack(int),HostSwDataUnpack(int id);
void		HostSendAskCmd(int,int);
 			   
unsigned char CrcCheckforfix();
unsigned char CrcCheckforval();
unsigned char DtoH(int), DtoH(int);

void		HostSendReadCmd(int,int,int,int,int,int,int,int,int);
void        Hour_Data_rcv(),Day_Data_rcv0(),Day_Data_rcv8();
extern		GetHost_Cmd(),	lan_out(),	Lan_init(),	Lan_finish();
int			HostRcvDataAnalyse(int,char *)	;		
void		HostRcvDataCul(int,char *,char *);
int			HostReadInfoEnd(int ,unsigned char *,int ,int ,int ,int );

BOOL		Read_HDB(char *, int , int );
RETCODE		Insert_HDB( int ,int ,char * ,double);
//====================== for H9000 ====================
#define		LCU_ANA_NUM		MAX_YC_NUM
#define		LCU_NUM_NUM		0
#define		LCU_IMP_NUM		0


typedef struct
{ 		unsigned short	status;
		unsigned short	data;
}ANA;

struct 
{ 
	char	dev_id;
	char	data_type;
	short	point_id;
	short	n_ana;
	short	n_num;
	short	n_imp;
	ANA		ana[LCU_ANA_NUM+LCU_NUM_NUM+LCU_IMP_NUM];	
}ana_all;


typedef struct 
{ char           c_name[100];  //中文名 
  char           l_name[100];  //逻辑名
  char			 dev_id;       
  char			 data_type;    
  short			 point_id;	   
  short          status;        
  int			 data;       
  float			 range;		   //量程
  float		     HI_limit;	   //报警上限
  float          fValueNew;    
  float          fValueOld;
  char			 instrument_id;
  char			 gather_id;
} YC;


/********************************ODBC for AK******************************************/

#define Ex_dbname    "aksd_syb"
#define Ex_server    EX_SERVER
#define Ex_username  "telcom"
#define Ex_password  "telcom"
//#define INSERT       "insert into t0600_on_off values ( %d, %d, '%s', %d )"
#define SELECT_LATE_TIME    "select max(c0600_datetime) from %s where c0400_station_num = %d and c0411_point_id = %d"
#define SELECT_INIT_DATA    "select c0600_status from t0600_on_off where c0400_station_num = %d and c0411_point_id = %d and c0600_datetime = '%s'"
#define UPDATE_NEW_DATA     "update t0411_dp_property set c0411_machine_status = %d where c0400_station_num = %d and c0411_point_id = %d"
#define INSERT       "insert into t0600_on_off values ( %d, %d, '%s', %d, %d, %d, %d )"   //add

#define command_get_ana_table_name  "SELECT c0402_table,c0402_column,c0402_order FROM t0402_type_name WHERE c0402_dp_type =(SELECT c0402_dp_type FROM t0410_dp_property WHERE c0400_station_num=%d AND c0410_point_id=%d)"

#define command_put_data        "INSERT INTO %s VALUES( %d, %d, '%s', %s)"


RETCODE	DatabaseConnect(),  DatabaseDisconnect();
HENV    henv;
HDBC    hdbc;
HSTMT   hstmt;


int	   HostSendLLZTTCmd();
int    ControlLog; 

HANDLE	ComInit(char *COM,DWORD Baud_Rate,BYTE ByteSize,BYTE Parity,BYTE StopBits);
HANDLE	hRecvSemaphore;

#define RD_INTERVAL_TIMEOUT		(DWORD)(132000/Com_Baud_Rate)
//#define RD_TOTAL_TIMEOUT_MUL	(DWORD)(22000/Com_Baud_Rate+1)
#define RD_TOTAL_TIMEOUT_MUL	(DWORD)(11000/Com_Baud_Rate+1)
//#define RD_TOTAL_TIMEOUT_CNT	(DWORD)(396000/Com_Baud_Rate+1)
#define RD_TOTAL_TIMEOUT_CNT	(DWORD)(40)
//#define WT_TOTAL_TIMEOUT_MUL	(DWORD)(11000/Com_Baud_Rate+1)
#define WT_TOTAL_TIMEOUT_MUL	(DWORD)(2)
//#define WT_TOTAL_TIMEOUT_CNT	(DWORD)(33000/Com_Baud_Rate+1)
#define WT_TOTAL_TIMEOUT_CNT	(DWORD)(10)
#define CBINQU	1024
#define CBOUTQU	1024