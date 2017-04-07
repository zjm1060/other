#include 	<sys/types.h>
#include 	<string.h>                      
#include 	<stdio.h>
/*#include	<termios.h>*/
#include 	<signal.h>
#include	<stdlib.h>
#include 	<fcntl.h>
/*#include 	<xti.h>
#include	<unistd.h>
#include 	<sys/shm.h>
#include 	<sys/sem.h>

#include <sys/errno.h>*/
/* #include	"/DBINC/dms_com.h" */
#include	"../../../dms/v4.0a/inc/dms_com.h"

/*#include <thread.h>
#include <pthread.h>*/

#ifdef SUN_OS
#include <errno.h>	/*added 2003.4.11 zy */
#include <termios.h>
#include <xti.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/errno.h>
#include <thread.h>
#include <pthread.h>

#include <wait.h>
/*#include <time.h>*/
#include	<sys/time.h>
#else
/*#include	<sys/time.h>*/
#include <time.h>
#include <windows.h>
#define	 sleep(sec)		Sleep(sec*1000)
#endif

#include	"../../../dps/v4.0a/inc/ipc_common.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"

#define Com1_Baud_Rate  1200
#define Com2_Baud_Rate  1200
#define Com3_Baud_Rate  1200
#define Com4_Baud_Rate  1200
#define Com5_Baud_Rate  1200
#define Com6_Baud_Rate  1200
#define Com7_Baud_Rate  1200
#define Com8_Baud_Rate  1200
#define Com9_Baud_Rate  1200
#define Com10_Baud_Rate  9600 

#ifdef SUN_OS
#define COM_PORT1	"/dev/ttya"
#define COM_PORT2	"/dev/ttyb"
#else
#define COM_PORT1	"Com1"
#define COM_PORT2	"Com2"
#endif

#define Com_Baud_Rate	Com1_Baud_Rate
#define Com_Baud_Rate2	Com2_Baud_Rate
#define	NONE	0
#define	EVEN	1
#define	ODD		2

#define RD_INTERVAL_TIMEOUT		(DWORD)(240000/Com_Baud_Rate)
#define RD_TOTAL_TIMEOUT_MUL	(DWORD)(22000/Com_Baud_Rate+1)
#define RD_TOTAL_TIMEOUT_CNT	(DWORD)(720000/Com_Baud_Rate+1)
#define WT_TOTAL_TIMEOUT_MUL	(DWORD)(11000/Com_Baud_Rate+1)
#define WT_TOTAL_TIMEOUT_CNT	(DWORD)(33000/Com_Baud_Rate+1)

#define CBINQU	1024
#define CBOUTQU	1024

#define	HOST_DIR	0		/*Host to RTU*/
#define	RTU_DIR		0x40	/*RTU to Host*/

//Command Code Mnemonic Define:

#define NAK		00		//NOT ACKNOWLEDGED
#define RRC		0x01		//Report RTU Configuration
#define DRF		0x02		//DATA Request Full
#define XRF		0x03		//Exception Request Full
#define SOE		0x04		//Sequence of Events
#define PAR		0x05		//Pulse Accumulator Report
#define PAZ		0x06		//Pulse Accumulator ZERO (And Freeze)
#define PAF		0x07		//Pulse Accumulator Freeze
#define PAT		0x08		//Pulse Accumulator THAW

#define SDB		0x09		//Set Deadband
#define RDB		0x0A		//Read Deadband
#define COA		0x0D		//Control Output ARM
#define COD		0x0E		//Control Output Direct
#define COE		0x11		//Control Output Execute

#define COL		0x15		//Control Output Latched
#define DRL		0x18		//DATA Request Latched
#define SST		0x19		//Synchronize System Time
#define RST		0x1A		//Report System Time
#define RIM		0x1B		//Report Interface Mode
#define SIM		0x1C		//Set Interface Mode

#define VER		0x1D		//Read RTU And RMF Version Identifiers
#define RES		0x1E		//Perform RTU Cold Start
#define ERR		0x1F		//Report Error DATA

//Board Define
#define DI		0x11	//24 pt Digital Input
#define AI		0x32	//12 pt Analog Input
#define DO		0x14	//32 pt Digital Output
#define TRD		0x05	//16 pt Timed Relay Driver
#define AO		0x26	//8 pt Analog Output
#define PA		0x3B	//8 pt Pulse Accumulator
#define CCO		0x30	//8 pt Controlled Contact Output

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
#define	INTRO1		21		//respond interrogation group 1 M direction

#define STATION_ID		1
#define RTU_CONFIG_LEN	23
#define SST_LEN	12
#define VARFRAMELEN	256
#define MINFRAMELEN	15
#define NAME_SIZE	36

#define MAX_BUF_SIZE    SHM_DATA_SEG_SIZE
#define MAX_RECV_NUM    SHM_DATA_SEG_COUNTER

#define CMD_ENTRY_NUMBER	10
#define CMD_DATA_SIZE		16
#define ASDU_ENTRY_NUMBER	100
#define ASDU_DATA_SIZE		256

#define	MAX_SSW_NUM			600
#define	MAX_DSW_NUM			300
#define	MAX_ANA_NUM			600

#define	MAX_SSW_CTRL		100
#define	MAX_DSW_CTRL		100
#define	MAX_ANA_CTRL		100

#define	SC_CMD_BASE_ADDR	0xB01
#define	DC_CMD_BASE_ADDR	0xB31
#define	RC_CMD_BASE_ADDR	0xB61
#define	SETP_CMD_BASE_ADDR	0xB81

#define	MEASURE_BASE_ADDR	0x701
#define	SW_POINT_BASE_ADDR	1
#define	IT_TOTAL_BASE_ADDR	0xC01
#define	ST_POINT_BASE_ADDR	0xC81

#define	SYS_RTU		12


#define	MIN_RMT_DEV_ID		3
#define	MAX_RMT_DEV_ID		4


#define	CMDLINKERR			88

#define	RESOLUTE		0x0FFF

#define	DRFT_SEC		(365*30+7)*24*3600
//946684800

//static char	*DEVNAME[]={"","F1","F2","F3","F4","F5","F6","F7","F8","F9","SWY","CMM","SYS","RCOM"};
//static char	*DATATYPE[]={"",".ANA.",".NUM.",".IMP.",".INT.",".POL.",".SYSMSG.",".MAPS.",".BHMSG.",".CALC."};	 

static char	*slotdefname[16]={"/home/ems/h9000/sc1801/SLOT0.DEF",
								"/home/ems/h9000/sc1801/SLOT1.DEF",
								"/home/ems/h9000/sc1801/SLOT2.DEF",
								"/home/ems/h9000/sc1801/SLOT3.DEF",
								"/home/ems/h9000/sc1801/SLOT4.DEF",
								"/home/ems/h9000/sc1801/SLOT5.DEF",
								"/home/ems/h9000/sc1801/SLOT6.DEF",
								"/home/ems/h9000/sc1801/SLOT7.DEF",
								"/home/ems/h9000/sc1801/SLOT8.DEF",
								"/home/ems/h9000/sc1801/SLOT9.DEF",
								"/home/ems/h9000/sc1801/SLOT10.DEF",
								"/home/ems/h9000/sc1801/SLOT11.DEF",
								"/home/ems/h9000/sc1801/SLOT12.DEF",
								"/home/ems/h9000/sc1801/SLOT13.DEF",
								"/home/ems/h9000/sc1801/SLOT14.DEF",
								"/home/ems/h9000/sc1801/SLOT15.DEF"};
						

/*struct shm_recv_seg {
        LANH_PTR	lan_head;
        char		data[MAX_BUF_SIZE];
};

struct shm_recv {
		short	head;
		short   end;
		short   flag;
        struct shm_recv_seg shm_recv_seg[MAX_RECV_NUM];
};

struct shm_recv data_recv_addr,*shm_recv_addr;*/

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
	POINTER	ptr;
	USHORT	status;
	USHORT	data;
	char	second;
	char	minute;
}  LANBCDATA;

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
	float	param[MAX_ANA_NUM];
	char	name[MAX_ANA_NUM][36];
} ANADEF;

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
	float	param[MAX_ANA_NUM];
	char	name[MAX_ANA_CTRL][36];
} SETPTCTRL;

typedef	struct
{
	USHORT	number;
	char	name[MAX_DSW_CTRL][36];
} STEPCTRL;

/*
typedef struct
{
	short	length;
	char	dp_type;
	char	dest_id;
	char	src_id;
	char	tmp;
}LANHD;
*/
typedef struct
{
	POINTER		ptr;
	ANLG_DATA	value;
}ANA_FMT;

typedef struct
{
	POINTER		ptr;
	unsigned short	status;
	unsigned short	ms;
	unsigned char	second;
	unsigned char	minute;
}IND_FMT;

void	WriteDspInfo(char *buf);
