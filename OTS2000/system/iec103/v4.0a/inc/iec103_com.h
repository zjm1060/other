/*******************************************************************************************************************************
FILENAME:	
FUNCTION:		

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, XP

HISTORY:
2008.4.10	start to modify the code for H9000 V4.0		zyf
********************************************************************************************************************************/
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory.h>
#include <time.h>
#include <windows.h>
#include <winsock.h>     

#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include    "../../../cmd/v4.0a/inc/cmd_def.h"
#include    "../../../dms/v4.0a/inc/dms_std.h"
#include    "../../../dms/v4.0a/inc/dms_structure_def.h"
#include    "../../../dms/v4.0a/inc/os_type_def.h"



//============================ 用 户 定 义 ===================================//

#define	MAX_STN_NUM103		10			//定义最大可以连接的保护装置的个数
#define STN_NUM			1			//实际连接的保护装置的数量 必须<=MAX_STN_NUM103
#define COM_PORT		"COM4"		//定义通信端口
#define Com_Baud_Rate	9600		//定义波特率：600, 1200, 4800, 9600
#define Parity_Type		EVENPARITY	//定义校验：EVENPARITY, ODDPARITY, NOPARITY

 
//注意：数组0位置放-1, 1位置放1#站地址，2位置放2#站地址，3位置放3#站地址，以此类推。
//		最多可定义MAX_STN_NUM103个地址。iec103_com.exe依次与之通讯(共STN_NUM个站)。 
static unsigned char station_addr[MAX_STN_NUM103+1]={ -1, 0x1, 0x55, 0x04, 0x31 };
		
static char	yc_file[MAX_STN_NUM103][200]=
	{
		"\\home\\ems\\h9000\\iec103\\def\\rtu1_yc_def.txt.", //1#站模拟量
		"\\home\\ems\\h9000\\iec103\\def\\rtu2_yc_def.txt.", //2#站模拟量
		"",
		"",
		"",
		"",
		"",
		""	
	};

static char	yx_file[MAX_STN_NUM103][200]=
	{
		"\\home\\ems\\h9000\\iec103\\def\\rtu1_yx_def.txt.", //1#站开关量
		"\\home\\ems\\h9000\\iec103\\def\\rtu2_yx_def.txt.", //2#站开关量
		"",
		"",
		"",
		"",
		"",
		""
	};
//======================================================================//



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


#define	CYCLE_NUM	50	//number of cycle for general intrrrogation
#define	TIMEOUT_NUM	10	//time out number for reset RTU
#define	REPEAT_NUM	3	//repeat number for a frame to discard
#define	SES_NUM	1		//ses request number for a station 1 class data

//TYPE IDENTIFICATION define:
#define M_TTM_TA_3	1		//double point  time tag
#define M_TMR_TA_3	2		//double point with relative time tag
#define M_MEI_NA_3	3		//measure value I
#define M_TME_TA_3	4		//measure value with relative time tag
#define M_IRC_NA_3	5		//idential flag
#define M_SYN_TA_3	6		//time Synchronization

#define M_TGI_NA_3		8		//integrated totals
#define M_MEII_NA_3		9		//measure value II with time tag
#define M_GD_N(T)_A_3	10		//general value ???
#define M_GI_N(T)_A_3	11		//general flag ???

#define M_MEIII_NA_3	15		//(NARI)measure value III without time tag

#define M_LRD_TA_3	23		//disturbance data with time tag ???
#define M_RTD_TA_3	26		//disturbance data ready to transmit ???
#define M_RTC_NA_3	27		//recorded channel ready to transmit ???
#define M_RTT_NA_3	28		//status change with flag ready to transmit ???
#define M_TDT_TA_3	29		//transmit status change with flag ???
#define M_TDN_NA_3	30		//transmit disturbance data ???
#define M_EOT_NA_3	31		//transmit end???

#define M_MEIII_TA_3	32		//measure value III with time tag
#define M_MEIV_TA_3		33		//measure value IV with time tag
#define M_MEV_TA_3		34		//measure value V with time tag
#define M_MEVI_TA_3		35		//measure value VI with time tag

#define M_IT_NA_3	36		//pulse point
#define M_IT_TA_3	37		//pulse point with time tag
#define M_ST_NA_3	38		//step point
#define M_ST_TA_3	39		//step point with time tag
#define M_SP_NA_3	40		//single point GI
#define M_SP_TA_3	41		//single point GI with time tag
#define M_DP_NA_3	42		//double point
#define M_DP_TA_3	43		//double point with time tag

#define M_SS_NA_3	44		//single point GI status
#define M_SS_TA_3	45		//single point GI status with time tag
#define M_DS_NA_3	46		//double point GI status
#define M_DS_TA_3	47		//double point GI status with time tag
#define M_WL_TA_3	48		//water level with time tag

#define M_MEVII_NA_3	50		//measure value VII with time tag

#define C_SYN_TA_3	6		//time Synchronization
#define C_IGI_NA_3	7		//general integrated
#define C_GD_NA_3	10		//general data
#define C_GRC_NA_3	20		//normal command
#define C_GC_NA_3	21		//general command 调定值
#define C_ODT_NA_3	24		//disturbance data transmit command ???
#define C_ADT_NA_3	25		//disturbance data transmit acknowledge ???

#define C_DC_NA_3	64		//circuit breaker control command
#define C_RC_NA_3	65		//regulating step command
#define C_SE_NA_3	66		//set point command,normal value
#define C_CC_NA_3	67		//control command

#define C_CI_NA_3	88		//pulse counter interrogation command
/*
#define M_SP_NA		1		//single point without time tag
#define M_SP_TA		2		//single point with time tag
#define M_DP_NA		3		//double point without time tag
#define M_DP_TA		4		//double point with time tag
#define M_ST_NA		5		//step point without time tag
#define M_ST_TA		6		//step point with time tag

#define M_ME_NA		9		//measure value without time tag
#define M_ME_TA		10		//measure value with time tag

#define M_IT_NA		15		//integrated totals without time tag
#define M_IT_TA		16		//integrated totals with time tag
#define M_BD_NA		232		//BCD code water level

#define M_EI_NA		70		//end of initialization
#define C_SC_NA		45		//single point control command
#define C_DC_NA		46		//double point control command
#define C_RC_NA		47		//regulating step command
#define C_SE_NA		48		//set point command
#define C_CS_NA		103		//clock synchronization command
#define P_ME_NA		110		//parameter for measurements,normalized
#define P_ME_NB		111		//parameter for measurements,scaled value
#define P_AC_NA		113		//parameter activation
*/
//cause of transmit
#define	SPONT		1		//spontaneous transmission
#define	CYC			2		//cyclic transmission
#define	RESET_FCB	3		//reset FCB bit
#define	RESET_CU	4		//reset communication unit
#define	START		5		//start or restart
#define	POWERON		6		//power on
#define	TEST		7		//test mode
#define	SYN_CLK		8		//synchronize clock
#define	GI_ACK		9		//general interrogation
#define	GI_TEM		10		//general interrogation terminal
#define	LOC_OP		11		//local operater
#define	RMT_OP		12		//remote operater
#define	CMD_ACK		20		//command ack
#define	CMD_NACK	21		//command negative ack

#define FIXFRAMELEN	5
#define VARFRAMELEN	256
#define MINFRAMELEN	15

#define MAX_BUF_SIZE    SHM_DATA_SEG_SIZE
#define MAX_RECV_NUM103    SHM_DATA_SEG_COUNTER

#define CMD_ENTRY_NUMBER	10
#define CMD_DATA_SIZE		16
#define ASDU_ENTRY_NUMBER	100
#define ASDU_DATA_SIZE		256

#define	MAX_SSW_NUM			600
#define	MAX_DSW_NUM			300
#define	MAX_ANA_NUM			600
#define	MAX_IT_NUM			128

#define	MAX_SSW_CTRL		100
#define	MAX_DSW_CTRL		100
#define	MAX_ANA_CTRL		100
#define	MAX_STEP_CTRL		100

#define	SC_CMD_BASE_ADDR	0xB01
#define	DC_CMD_BASE_ADDR	0xB31
#define	RC_CMD_BASE_ADDR	0xB61
#define	SETP_CMD_BASE_ADDR	0xB81

#define	MEASURE_BASE_ADDR	0x701
#define	SW_POINT_BASE_ADDR	1
#define	IT_TOTAL_BASE_ADDR	0xC01
#define	ST_POINT_BASE_ADDR	0xC81

//#define	SYS_RTU		12


#define	MIN_RMT_DEV_ID		7
#define	MAX_RMT_DEV_ID		7

#define	NAME_SIZE			36
#define	CMDLINKERR			88

//static char	*DEVNAME[]={"","F1","F2","F3","F4","F5","F6","F7","F8","F9","SWY","CMM","SYS","RCOM"};
static char	*DATATYPE[]={"",".ANA.",".NUM.",".IMP.",".INT.",".POL.",".SYSMSG.",".MAPS.",".BHMSG.",".CALC."};	 


typedef struct         // command data structure ASDU 
{
	unsigned short	pnt;
	unsigned short	length[CMD_ENTRY_NUMBER];
	unsigned char	data[CMD_ENTRY_NUMBER][CMD_DATA_SIZE];
}ASDUCOMDDATA;

typedef struct         // received data structure ASDU
{
	unsigned short	pnt;
	unsigned short	length[ASDU_ENTRY_NUMBER];
	unsigned char	data[ASDU_ENTRY_NUMBER][ASDU_DATA_SIZE];
}ASDUDATA;

typedef struct
{
	POINTER	ptr;
	USHORT	status;
	USHORT	data;
	char	second;
	char	minute;
}  LANBCDATA;



//--------------- added by zm --------------

#define		MAX_YC_NUM			600
#define		MAX_YX_NUM			300

#define		FAILURE				-1
#define		SUCCESS				0
int         tmptmp;
typedef struct
{
  char           c_name[100]; 
  char           l_name[100]; 
  char			 dev_id;       
  char			 data_type;    
  short			 point_id;	   
  short          status;        
  GLONG          value;       
  float			 range;		   
  float		     HI_limit;	  
  float          fValueNew;    
  float          fValueOld;
  int			 InfoID;
  float			 PntNum;
} YC;


typedef struct 
{ char          c_name[100];
  char          l_name[100]; 
  char			dev_id;
  char			data_type;
  short			point_id;
  short         StatusNew;
  short			StatusOld;
  int			InfoID;
  int			FunID;
} YX;	

typedef struct
{	YX		yx[MAX_YX_NUM];
	YC		yc[MAX_YC_NUM];
}RTU;

RTU	rtu[STN_NUM];

typedef struct
{ char				dev_id;
  char				data_type;
  short				point_id;
  unsigned short    status;
  unsigned short    value;
  float		        HI_limit;	   //报警上限
  char				day;
  char				hour;
  char				minute;
  char				second;
  short				ms;
} DB_DATA;


int		ReadDefineFile();
void	Broadcast_ana(),	Broadcast_sw();
void	ProcYx(),			ProcYc(int, int, short);
										
float   AnaDataToFloat_1( DB_DATA );
DB_DATA FloatToAnaData_1( float );

int				GetStnId();
unsigned char	GetStnAddr();

HANDLE			ComInit();
extern			MountDB();