#define DEBUG
#include "\home\ems\system\lan\v4.0a\inc\lan.h"
#include "iec101_hdb.h"

#define Com1_Baud_Rate  9600
#define Com2_Baud_Rate  4800
#define Com3_Baud_Rate  9600
#define Com4_Baud_Rate  1200
#define Com5_Baud_Rate  1200
#define Com6_Baud_Rate  1200
#define Com7_Baud_Rate  1200
#define Com8_Baud_Rate  1200
#define Com9_Baud_Rate  1200
#define Com10_Baud_Rate  9600 

#define COM_PORT	"COM1"
#define Com_Baud_Rate	Com2_Baud_Rate

//#define RD_INTERVAL_TIMEOUT	(DWORD)(33000/Com_Baud_Rate+1)
#define RD_INTERVAL_TIMEOUT		(DWORD)(396000/Com_Baud_Rate+1)
#define RD_TOTAL_TIMEOUT_MUL	(DWORD)(22000/Com_Baud_Rate+1)
#define RD_TOTAL_TIMEOUT_CNT	(DWORD)(396000/Com_Baud_Rate+1)
#define WT_TOTAL_TIMEOUT_MUL	(DWORD)(11000/Com_Baud_Rate+1)
#define WT_TOTAL_TIMEOUT_CNT	(DWORD)(33000/Com_Baud_Rate+1)

#define CBINQU	1024
#define CBOUTQU	1024

#define	STN_NUM		1
#define	CYCLE_NUM	200	//number of cycle for general interrogation
#define	TIMEOUT_NUM	10	//time out number for reset RTU
#define	REPEAT_NUM	3	//repeat number for a frame to discard
#define	SES_NUM	1		//ses request number for a station 1 class data

#define	LINKADRESS	0
#define	RESOLUTE	0x7FFF

//TYPE IDENTIFICATION define:
#define M_SP_NA		1		//single point without time tag (class 1, IC)
#define M_SP_TA		2		//single point with time tag (SOE)
#define M_DP_NA		3		//double point without time tag (class 1, IC)
#define M_DP_TA		4		//double point with time tag (SOE)
#define M_ST_NA		5		//step point without time tag (class 2, IC)
#define M_ST_TA		6		//step point with time tag
#define M_BO_NA		7		//slave station remote terminal status

#define M_ME_NA		9		//measure value without time tag (class 2, IC)
#define M_ME_TA		10		//measure value with time tag
#define M_ME_NB		11		//measure value without time tag,scaled
#define M_ME_TB		12		//measure value with time tag,scaled

#define M_IT_NA		15		//integrated totals without time tag
#define M_IT_TA		16		//integrated totals with time tag

#define M_PS_NA		20		//status detected block sp information (class 1, IC)
#define M_ME_ND		21		//measure value, normalized without quality descriptor (class 2, IC)

#define M_SP_TB		30		/*Single point with time tag CP56Time2a (SOE) */
#define M_DP_TB		31		/*Doule point with time tag CP56Time2a (SOE) */

#define M_EI_NA		70		//end of initialization

#define M_BD_NA		232		//BCD code water level (class 2, IC)

#define C_SC_NA		45		//single point control command
#define C_DC_NA		46		//double point control command
#define C_RC_NA		47		//regulating step command
#define C_SE_NA		48		//set point command
#define C_SE_NB		49		//set point command

#define C_IC_NA		100		//interrogation command
#define C_CI_NA		101		//counter interrogation command
#define C_RD_NA		102		//read command
#define C_CS_NA		103		//clock synchronization command

#define P_ME_NA		110		//parameter for measurements,normalized
#define P_ME_NB		111		//parameter for measurements,scaled
#define P_AC_NA		113		//parameter activation

//cause of transmit
#define	PER_CYC		1		//cyclic transmission
#define	BACK		2
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

#define FIXFRAMELEN	5
#define VARFRAMELEN	256
#define MINFRAMELEN	15

#define MAX_BUF_SIZE    SHM_DATA_SEG_SIZE

#define CMD_ENTRY_NUMBER	10
#define CMD_DATA_SIZE		16
#define ASDU_ENTRY_NUMBER	100
#define ASDU_DATA_SIZE		256

#define	MAX_SSW_NUM			768
#define	MAX_DSW_NUM			1000
#define	MAX_ANA_NUM			600
#define	MAX_IT_NUM			128

#define	MAX_SSW_CTRL		100
#define	MAX_DSW_CTRL		100
#define	MAX_STEP_CTRL		100
#define	MAX_ANA_CTRL		100

#define	SC_CMD_BASE_ADDR	0xB01
#define	DC_CMD_BASE_ADDR	0xB31
#define	RC_CMD_BASE_ADDR	0xB61
#define	SETP_CMD_BASE_ADDR	0xB81

#define	SP_BASE_ADDR	0x001
#define	DP_BASE_ADDR	0x301
#define	ME_BASE_ADDR	0x701
#define	PE_BASE_ADDR	0x901
#define	IT_BASE_ADDR	0xC01
#define	ST_BASE_ADDR	0xC81

#define	ANA_GROUP_NUM	64
#define	SDP_GROUP_NUM	128
#define	IT_GROUP_NUM	32


#define	MIN_RMT_DEV_ID		3
#define	MAX_RMT_DEV_ID		4

#define	NAME_SIZE			36
#define	CMDLINKERR			88	//Channel fail database point address

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


typedef struct
{
	POINTER		ptr;
	ANLG_DATA	value;
}ANA_FMT;

typedef	struct
{
	USHORT	number;
	char	name[MAX_SSW_NUM][36];
} SSWDEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_DSW_NUM][36];
} DSWDEF;

typedef	struct
{
	USHORT	number;
	float	lorange[MAX_ANA_NUM];
	float	hirange[MAX_ANA_NUM];
	char	name[MAX_ANA_NUM][36];
} ANADEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_DSW_NUM][36];
} ITDEF;

typedef	struct
{
	USHORT	number;
	char	name[MAX_SSW_CTRL][36];
} SSWCTRL;

typedef	struct
{
	USHORT	number;
	char	name[MAX_DSW_CTRL][36];
} DSWCTRL;

typedef	struct
{
	USHORT	number;
	float	lorange[MAX_ANA_NUM];
	float	hirange[MAX_ANA_NUM];
	char	name[MAX_ANA_CTRL][36];
} SETPTCTRL;

typedef	struct
{
	USHORT	number;
	char	name[MAX_STEP_CTRL][36];
} STEPCTRL;

