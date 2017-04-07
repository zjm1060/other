/*********************************************************************************
**	This include file  is designed for  struct of data on dl476-92		**
**                                                                          	**
**	History									**
**										**
**	Date		author		comments				**
**	2004.5		Yuan/Hong	First version				**
**										**
*********************************************************************************/
#ifndef DL476STD
#define DL476STD

#define dl476debug

#define HPUX

//#define ALPHA_UNIX
/*#define LINUX*/
/*#define MSWIN32*/


#ifdef ALPHA_UNIX
#include        <sys/types.h>
#include        <sys/socket.h>
#include        <sys/errno.h>
#include        <netinet/in.h>
#include        <netdb.h>
#include        <string.h>
#include        <stdio.h>
#include        <signal.h>
#include        <stdlib.h>
#include        <fcntl.h>
#include        <xti.h>
#include        <unistd.h>
#include        <time.h>
#include	<math.h>

/* #include <sys$common:[syslib]socket.h> */

#define RECV_FLAGS      0
#define SOCKET          short
#define SOCKET_ERROR    (-1)
#define INVALID_SOCKET  (-1)
#define closesocket(x)  shutdown(x,0)
#define fflush(x)       getchar()

#endif

#ifdef LINUX

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include <math.h>

#define RECV_FLAGS      (0)
#define SOCKET_ERROR    (-1)
#define SOCKET          int
#define INVALID_SOCKET  (-1)
#define closesocket(x)  close(x)
#define _toupper(x)     toupper(x)

#endif


#ifdef HPUX

/* known to work under HPUX */
/* compile with cc -Aa for ANSI compatible */

#define _HPUX_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <math.h>

#define RECV_FLAGS      (0)
#define SOCKET_ERROR    (-1)
#define SOCKET          int
#define INVALID_SOCKET  (-1)
#define closesocket(x)  close(x)
#define BIG_ENDIAN      (1)
 
#endif


#ifdef MSWIN32

/* known to work under Windows 95 and Windows NT, MSVC 5.0 */

/*
 * note when using MSVC/C++ 5.0 - create workspace as a win32 console app and
 *  change project/settings/link Object/library modules list to include wsock32.lib
 */

#include <windows.h>
#include <winbase.h>
#include <string.h>
#include <stdlib.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#pragma pack(1)

#define RECV_FLAGS  0

#define	 sleep(sec)		Sleep(sec*1000)
#define	 usleep(sec)	Sleep(sec/1000)

#endif

//#include        "../../../../system/lan/v4.0a/inc/lan_common.h"
#include        "../../../../system/lan/v4.0a/inc/lan_private.h" 
#include        "../../../../system/dps/v4.0a/inc/dps_type_def.h"
#include        "../../../../system/dps/v4.0a/inc/dps_rec_def.h"
#include        "../../../../system/dps/v4.0a/inc/ipc_common.h"


#define		WAN_PORT			51000

#define		MAX_REMOTE_NODE			10
#define		MAX_LOCAL_NODE			2

#define		ANA_TABLE			2
#define		SW_TABLE			3
#define		SWCTRL_TABLE			4

#define		MAX_DATA_SIZE			20000	/* all point number*/

#define		MAX_BLOCK_SIZE			2000 //2048
#define		MAX_TABLE_NUM			35
#define		MAX_APDU_SIZE			MAX_BLOCK_SIZE*2

#define		MAX_SWCTRL_SIZE			600

/*****		控制域 ctr定义			******/

#define		A_ASSOCIATE			0X01
#define		A_ASSOCIATE_ACK			0X02
#define		A_ASSOCIATE_NAK     		0X03
#define		A_RELEASE			0X04
#define		A_RELEASE_ACK			0X05
#define		A_RELEASE_NAK			0X06
#define		A_ABOR				0X07
#define		A_RESET				0X08
#define		A_RESET_ACK			0X09
#define  	A_DATA        			0x0a
#define	 	A_UNLAST_DATA    		0x8a
#define  	A_DATA_ACK         		0x0b    
#define  	A_DATA_NCK         		0x0c 

#define		NO_PARA_SECTION			0
#define		PARA_SECTION			0X80  

/******		运行模式run_mode定义		******/

#define		RULE_0				0X10
#define		RULE_1_SERV			0X20
#define		RULE_1_CLIT			0X30

#define		GROUP_TRAN			0X0A
#define		SINGLE_TRAN			0X05

/******		状态标识status_flag定义	  	******/

#define		HOST_ONLINE			127	
#define		HOST_OFFLINE			0

/******		原因码reason_flag定义	 	******/

#define		OS_SUCCESS			1
#define		PRIORITY_ERR			2
#define		ASKMODE_ERR	 		3
#define		NOT_HOST			4
#define		SOURCEID_ERR			5
#define		DESTID_ERR			6
#define		LOW				7
#define		RUNMODE_ERR			8
#define		DATABASE_ERR			9
#define		DATAINDEX_ERR			10
#define		UNKNOWN_APDU			11
#define		UNKNOWN_DATA			12
#define		PROTOCOL_ERR			13


/******		数据块类型定义 			******/
#define  	B_ALL_INT			1	   	/*全测量量整型块 NO USE*/
#define  	B_ALL_REAL			2	   	/*全测量量实型块*/
#define  	B_ALL_SW       		3	   	/*全状态量块*/

#define		B_GROUP_INT			4		/*成组测量量整型块*/
#define		B_GROUP_REAL		5		/*成组测量量实型块*/
#define		B_GROUP_SW			6		/*成组状态量块*/

#define  	B_CHANG_INT			7  		/*变化测量量整型块*/
#define  	B_CHANG_REAL		8  		/*变化测量量实型块*/
#define  	B_CHANG_SW			9	   	/*变化状态量块*/

#define		B_TIME_REAL			11	   	/*时标测量量实型块*/
#define  	B_TIME_SW       	12	   	/*时标状态量块*/

#define	 	B_CONTROL			22		/*控制命令块*/
#define	 	B_ADJUST			23		/*升降命令块*/
#define		B_CTRL_BACK			25		/*控制命令返回*/
#define	 	B_ADJ_BACK			26		/*升降命令返回*/
#define		B_CTRL_RUN			40		/*控制命令执行*/
#define		B_ADJ_RUN			41		/*升降命令执行*/
#define		B_CURSE				42		/*曲线数据*/
#define		B_CTRL_RET			43		/*控制结果返回*/

#define	 	B_ANASET			50		/*模出命令块*/
#define	 	B_ANASET_BACK		51		/*模出命令返回*/
#define		B_ANASET_RUN		52		/*模出命令执行*/


/******		数据块长度定义 				******/
#define  	ONE_CHANG_SW		3	   	/*变化状态量块*/
#define  	ONE_CHANG_REAL		7  		/*变化测量量实型块 */
#define		ONE_TIME_SW		11		/*时标状态量块	*/
#define		ONE_TIME_REAL		13		/*时标变化测量量实型块 */

/******		操作类型、操作性质			******/
#define		R_APOWER		1
#define		R_RPOWER		2
#define		R_BREAKER		3
#define		R_SWITCH		4
#define		R_UNIT			5
#define		R_GATE			6
#define		R_AGC_SET		7
#define		R_DEVICE		8
#define		R_UNIT_TST		9
#define		R_SYS_MODE_SET	10

#define		S_OPEN			1
#define		S_CLOSE			2
#define		S_ON			3
#define		S_OFF			4

/******		质量码定义				******/

   
#define  	DATA_UNCHANG      	0x40    
#define  	DATA_INVALID      	0x20     
#define  	DATA_ALARM        	0x10     
#define  	DATA_MANSET      	0x08  /*4.0模拟量无手动设置，取数据溢出状态*/   
#define	  	DATA_ESTIMATE   	0x04    
#define		DATA_NEGATIVE		0x8000

/******		状态码定义				******/

#define		CLOSE			0x00		/* 闭合或正常状态*/
#define		OPEN			0x01		/*  断开或异常状态 */

#define		HIGH_PRIORITY		1
#define		LOW_PRIORITY		2


/*****	协议控制的APDU报头			*****/
/*#define	NARI*/

#define	GB_HEAD		0xffff

typedef struct   						   
{
#ifdef NARI
	unsigned	short	nari_head1;
	unsigned	short	nari_head2;
#endif	
	unsigned	char	ctr; 
  	unsigned	char  	run_mode;
  	unsigned	char  	status_flag;
  	unsigned	char  	reason;
  	unsigned	short	length;
} PROTOCOL_HEAD;

typedef	struct
{
	PROTOCOL_HEAD		head;
	unsigned	char	para[10];
}PROTOCOL_APDU;


/*****	基本数据应答的APDU		*****/
typedef struct  
{ 
	unsigned	char  	ctr;
  	unsigned	char  	nr;
  	unsigned	char  	extend;
  	unsigned	char  	reason;
} DATACK_APDU;
                 

/*****	基本数据的APDU报头			*****/


#define	GB_HEAD 0xffff
typedef struct   						   
{
	unsigned	char	ctr; 
  	unsigned	char  	nr;
  	unsigned	char  	ns;
  	unsigned	char  	priority;
  	unsigned	short	length;
} BASEDATA_HEAD;

#define	APDU_HEAD_SIZE		sizeof(BASEDATA_HEAD)

/*****	基本数据块的定义			*****/
typedef struct  				 					
{ 
	unsigned	char 	bid;
	unsigned	char	tableno;
	unsigned	short 	length;
 	unsigned	char	data[MAX_BLOCK_SIZE];

}DATA_BLOCK;

#define	BLOCK_HEAD_SIZE	4

typedef struct  				 					
{ 
	unsigned	char	data[MAX_BLOCK_SIZE];

}COMM_BLOCK;

typedef struct
{	
	BASEDATA_HEAD 	head;
/*	union
	{
		DATA_BLOCK		basedata;	
		COMM_BLOCK		cmd;
	}block;
*/	unsigned	char	block[MAX_APDU_SIZE];
}DATA_APDU;


/******		数据结构定义				******/

typedef struct{
		unsigned short	number;
		char		name[MAX_REMOTE_NODE][60];
		char		ip[MAX_REMOTE_NODE][60];

}NODE_DEF;

typedef struct
{
		unsigned char	no;
		unsigned char	type;
		char		file[60];
		unsigned short	ptnum;
		unsigned short	baseaddr;
}TABLE_DEF;


typedef struct
{
		float			band;
		float			value;
		float			oldvalue;
		unsigned char	quality;
}REAL_DATA;

typedef struct
{
		unsigned char	value;
		unsigned char	oldvalue;
}SW_DATA;

typedef struct
{
		unsigned char	tableno;
		unsigned char	tabletype;
		char		name[60];
		unsigned char	stn_id;/*zwj for v4.0*/
		unsigned char	dev_id;
		unsigned char	data_type;
		unsigned short	point_id;
		union
		{
			REAL_DATA	real;
			SW_DATA		sw;
		}data;		
}DATA_INDEX;


typedef struct
{
		unsigned char	tableno;
		unsigned char	tabletype;
		char		name[60];
		unsigned char	stn_id;/*zwj for v4.0*/
		unsigned char	dev_id;
		unsigned char	data_type;
		unsigned short	point_id;
}SWCTRL_INDEX;

/****************************************/
NODE_DEF		lnode,rnode;
short			hot_rnode;

TABLE_DEF		table[MAX_TABLE_NUM];

DATA_INDEX		dataindex[MAX_DATA_SIZE];
unsigned short	index_num;

SWCTRL_INDEX	swctrl[MAX_SWCTRL_SIZE];
unsigned short	swctrl_num;

DATA_APDU		datapdu,cmdapdu;

DATA_BLOCK		datablock;
extern		SHM_RECORD	*shm_rec_addr;
#define			ALLTIME		2
short			allflag; 

#endif
