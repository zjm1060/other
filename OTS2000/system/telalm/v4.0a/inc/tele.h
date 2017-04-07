//#pragma once
#ifndef  _TELE_H_
#define  _TELE_H_
#include <windows.h>
#include <winsock.h>    
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <winbase.h>
#include "srllib.h"
#include "dxxxlib.h"
#include "dtilib.h"
#include "sctools.h"
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <fstream.h>
#include	"../../../dms/v4.0a/inc/os_type_def.h"
#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"
//#define  TELASK_FILE			"\\home\\ems\\h9000\\def\\telask.def"
//#define  TELSMS_FILE			"\\home\\ems\\h9000\\def\\telsms.def"
//#define  ALMRECORD_FILE         "\\home\\ems\\h9000\\def\\almrecord.def"
//#define  DEFINE_FILE			"\\home\\ems\\h9000\\def\\define.def"
//#define  SMSDEFINE_FILE			"\\home\\ems\\h9000\\def\\sms_define.def"
//#define	 TELNUMBER_FILE			"\\home\\ems\\h9000\\def\\telnumber.def"
//#define	 MOBILENUMBER_FILE		"\\home\\ems\\h9000\\def\\mobile_number.def"
//#define  SMSDEF_FILE			"\\home\\ems\\h9000\\def\\sms_define.def"
//#define  SMSSEND_FILE			"\\home\\ems\\h9000\\def\\sms_send.def"
//#define  SELDEV_FILE			"\\home\\ems\\h9000\\def\\select_dev.def"
//#define  SMSTIME_FILE			"\\home\\ems\\h9000\\def\\sms_time.def"
//#define  SMSNUM_FILE            "\\home\\ems\\h9000\\def\\sms_num.def"
//#define  MANUAL_FILE            "\\home\\ems\\h9000\\def\\manual_define.def"
#define	 OUTDATA_RECORD_PATH	"\\home\\ems\\h9000\\his\\ONCALL\\OUTDATA\\"
#define	 INDATA_RECORD_PATH		"\\home\\ems\\h9000\\his\\ONCALL\\INDATA\\"
//#define  SW_ALM_FILE			"\\home\\ems\\h9000\\def\\GetAlmClass.def"
//#define  ANA_ALM_FILE			"\\home\\ems\\h9000\\def\\GetAnaAlmClass.def"
#define  MSG_DEFFILE			"\\home\\ems\\h9000\\gen\\MSG_DEF.DEF"
#define  DATABASENAME			"\\home\\ems\\h9000\\dat\\DATABASE.OUT"
#define  SMS_XML_FILE			"\\home\\ems\\h9000\\def\\oncall_define.xls"
//#define  TEL_LINE_FILE			"\\home\\ems\\h9000\\def\\tel_line.def"
//#define  TELDEF_FILE            "\\home\\ems\\h9000\\def\\tel_define.def"
#define  maingladefilepath       "\\home\\ems\\system\\telalm\\v4.0a\\uis\\oncallmain.glade"
#define  nameselectgladefilepath  "\\home\\ems\\system\\telalm\\v4.0a\\uis\\oncallnameselectdig.glade"
#define  nameselectgladepicpath  "\\home\\ems\\system\\telalm\\v4.0a\\uis\\depart.jpg"
#define  nameselectgladepicpath1  "\\home\\ems\\system\\telalm\\v4.0a\\uis\\user.jpg"
//#define  ONCALLMAX_CRT_NUM 1
//#define	 ONCALL_TEMPALMBUF_SIZE 2048
#define	 ONCALL_SMS_AUTOALMSENDBUF_SIZE 512 //自动报警
#define	 ONCALL_SMS_MANUALMSENDBUF_SIZE 512//半自动报警
#define	 ONCALL_SMS_MANUSENDBUF_SIZE    128 //手动发送
#define	 ONCALL_SMS_TIMINGSENDBUF_SIZE  1024 //定时发送
#define	 ONCALL_DIAL_MANUALMBUF_SIZE    1024 //自动
#define	 ONCALL_DIAL_AUTOALMBUF_SIZE    1024 //手动发送
#define	 ONCALL_GtkWidget_INFRO_REDRAW	2000 //界面定时刷新信息时间ms		1
#define	 ONCALL_GtkWidget_INFRO_REDRAW_L 2800 //界面定时刷新信息时间ms		1
#define	 TELPHONE_NUM			        512  
#define	 MAX_LOGNUM_PER_BUF			    1024  //
#define	 ALARM_NUM				        2048  
#define  TIMING_NUM				        256  
#define	 ONCALLINQUIRY_NUM			    1024 
#define	 TELPHONEGROUP_NUM			    256  
 
#define	 TIMEOUT_SECONDS		1

#define  MAXCHAN				20

#define  MAX_sms_dev_num		20

#define  DLDST_DISCTONE         200 
#define  DLDDT_DISCTONE         201 
#define  DLDSTCAD_DISCTONE      202
#define  DLDDTCAD_DISCTONE      203 
#define  HOOKONBUSYTONETIMES    10
#define	 MAX_TEL_NUM			16
//#define	 MAX_BP_NUM				4
//#define	 MAX_MOBILE_NUM			16
#define  BUFSIZE				4000

#define	 MAX_ALM_NUM 			10
#define	 SN_STR_LEN				12
#define	 TIME_STR_LEN			64
#define	 REPEAT_ALM_STR_LEN		12
#define	 STATE_STR_LEN			128
#define  MAX_MESSAGE_SIZE		512
#define	 MAX_CMD_SRC_NAME_SIZE	64
#define	 MAX_ALM             	20

#define  MAXDEV_NUM				200
#define  MAX_WRITEFILEBUFNUM    1024

#define POINT_0		0
#define	POINT_1		1		
#define	POINT_2		2
#define	POINT_3		3
#define	POINT_4		4
#define	POINT_5		5
#define	BitS_TYPE	0
#define	BitN_TYPE	5	//v3.0 modified 4 to 5
#define	BitS_STATUS	2
#define	BitN_STATUS	4
	
#define	DEV_TYPE_RELAY		0
#define	DEV_TYPE_BREAKER	1
#define DEV_TYPE_VALVE		3
#define DEV_TYPE_DEVICE	    4
#define DEV_TYPE_SWITCH	    2
#define	DEV_TYPE_GENSTA	    7
#define DEV_TYPE_DEVMSG		8
#define	DEV_TYPE_STREAM		9
#define	DEV_TYPE_NETMSG		10
#define	DEV_TYPE_HOSTMSG	11
#define	DEV_TYPE_LOGMSG 	12
#define DEV_TYPE_ONOFFMSG	13
#define DEV_TYPE_AGCCMD     14

#define	RELAY_BACK		    0
#define	RELAY_ACTION		1
#define	RELAY_ERR		    2

#define	BREAKER_OPEN		0
#define	BREAKER_CLOSE		1
#define	BREAKER_TRANSIT	    2
#define	BREAKER_ERR		    3

#define	VALVE_CLOSE		    0
#define	VALVE_OPEN		    1
#define	VALVE_TRANSIT		2
#define	VALVE_ERR		    3

#define	DEVICE_OFF		    0
#define	DEVICE_ON		    1
#define	DEVICE_TRANSIT		2
#define	DEVICE_ERR		    3

#define	SWITCH_OPEN		    0
#define	SWITCH_CLOSE		1
#define	SWITCH_TRANSIT	    2
#define	SWITCH_ERR		    3

#define	GENSTA_STANDBY      0
#define	GENSTA_RUN          1
#define	GENSTA_MAINTENANCE  2
#define	GENSTA_COMPENSATION 3
#define	GENSTA_KZH          4
#define	GENSTA_KZ           5
#define	GENSTA_STOP         6

#define DEVMSG_NORMAL		0
#define DEVMSG_FAIL		    1
#define DEVMSG_ERR		    2

#define STREAM_NORMAL		0
#define STREAM_STOP		    1 
#define STREAM_ERR		    2

#define NETMSG_NETA		    0
#define NETMSG_NETB		    1 
#define NETMSG_ERR		    2

#define HOSTMSG_EXIT		0
#define HOSTMSG_HOST		1 
#define HOSTMSG_STANDBY		2
#define HOSTMSG_ERR		    3

#define LOGMSG_LOGOUT       0
#define LOGMSG_LOGIN        1
#define LOGMSG_ERR          2

#define ONOFFMSG_OFFLINE    0
#define ONOFFMSG_ONLINE     1
#define ONOFFMSG_FAILURE    2
#define ONOFFMSG_ERR        3

#define AGCCMD_NOOPR        0
#define AGCCMD_START        1
#define AGCCMD_STOP         2
#define AGCCMD_TG           3
#define AGCCMD_TZF          4
#define AGCCMD_ERR          5

#define	 YES				1
#define	 NO					0
#define	 HI				    9999
#define	 LOW				-9999
#define  NORM_SCAN          0  
#define  OVER_HH_LMT        1  
#define  OVER_H_LMT         2  
#define  OVER_L_LMT         3  
#define  OVER_LL_LMT        4  
#define  BACK_H_LMT         5  
#define  BACK_L_LMT         6  

#define	 SUCCESS			1
#define	 FAILURE			-1
#define	 DIAL_OK			"success"
#define	 DIAL_ERR			"fail"

#define  MAX_CHAR_NO        128 //The max number of the characters


extern	 MountDB (),GetPtId(),LocateKeyword(),InitAudioLibraryJT();

int		tel_playwav_msg(int xh,int chdev,char *msg);   
int		gsmString2Bytes(), gsmDecode7bit(), gsmDecodeUcs2();
char    GetDevNameRemark();
void	TakeApart(), SW_proc(), ANA_proc(),	
		InitFile(), GetDigit(), autoalmTelDial(),manualmTelDial();
void    update_oncallsendbuf(int oncall_alm_def_index, char *cname, char *time);
void    oncallTelDial();
void    oncallTelQuery();
BOOL    XmlCellcheckadgetvalue(char *str, char *strValue,char * ckstartchar,char * ckendchar,char * ckendchar2,int cgtype,int * ended);
void    Getarray();
int		GetPointIdFromRec(REC_ENTRY_UNION *rec, POINTER *pt);
int		CreateMobileNum( char *str );
void	DeCodeMobileNuber(unsigned char *str ,int len ) ;
int		InitIPC(void);
void	UpdateONCALLALMBUF(REC_ENTRY_UNION *rec,char namestr[POINT_NAME_SIZE]);// yujji 090331
int		SMS_SENDRECEIVE_COM_PortInit(int idx);	
void	ClearRecvFault(int idx),	ClearSendCmdFault(int idx);
int		readOncallDefineXML(void);
int		AsciiToUnicode( unsigned char *str );
int		CDMA_AsciiToUnicode( unsigned char *str );
int     UnicodeToAscii( const unsigned char* pSrc, char* pDst, int nSrcLength );// unicode-ascii
int		UTF8ToUnicode ( const unsigned char* str , char* destr );   // utf8-unicode
void	UTF8ToAscii(char *str);
BOOL	GetStrValueFromCellStr(char *str, char *strValue);
BOOL	GetIntValueByIndexStr(char *str, char *index, int *value);
//int     SendMessagetoGSM(int len, int len1, char NumStr[20], unsigned char   strText[BUFSIZE], unsigned char   send_msg[BUFSIZE],unsigned char rTONNPI0,unsigned char rTONNPI1);
//int     SendMessagetoCDMA(int len, int len1, char NumStr[20], unsigned char  strText[BUFSIZE], unsigned char   send_msg[BUFSIZE],unsigned char rTONNPI0,unsigned char rTONNPI1);	// add by yujji 2010-07-07
int     SendMessagetoGSM(int idx);
int     SendMessagetoCDMA(int idx);	

int	GettelalmCbaState(POINTER *point,int type_id,int ind_rt_state,int qlt_alm_flag);


LRESULT WINAPI SMS_SENDRECEIVE(LONG lparam);//半自动短信报警线程
//LRESULT WINAPI SendBytimeShortMsg(LONG lparam) ;
//LRESULT WINAPI CDMA_shortmsg(LONG lparam);  //wavecom cdma_alm
//LRESULT WINAPI CDMA_ReceiveShortMsg(LONG lparam);  //wavecom cdma_rev
//LRESULT WINAPI CDMA_SendBytimeShortMsg(LONG lparam);  //wavecom cdma_tim
DWORD   ReQuery    (LONG IParam) ;//20110226 取消 DWORD WINAPI ReQuery(LONG IParam)
void	initialize_gtd ();
void	PlayAlm();
int		PortInit(),		CreateMobileNum();	
//int	PortInit_cdma();	   
void	ClearRecvFault(),		ClearSendCmdFault();
void	ClearRecvFault_cdma(),	ClearSendCmdFault_cdma();
int read_OncallDefineXML();


 
void sendONCALL_DIAL_AUTOALMBUF();
void sendONCALL_DIAL_MANUALMBUF();
void sendONCALL_SMS_AUTOALMSENDBUF(int idx);
void sendONCALL_SMS_MANUALMSENDBUF(int idx);
void sendONCALL_SMS_MANUSENDBUF(int idx);
void sendONCALL_SMS_TIMINGSENDBUF(int idx);

void receiveONCALL_SMS_GSM(int idx);
void receiveONCALL_SMS_CDMA(int idx);
void Oncall_almsearch();
void Oncall_writefie();
void Oncall_ALM_manuconfirm();
void Oncall_sms_timingsend(int idx);
 
int  write_main_logs_now(char * str,int wtp);
gint uidelete_event( GtkWidget *widget,GdkEvent  *event,gpointer data );
void uidestroy( GtkWidget *widget,gpointer data );

int oncallgtkmain(int argc,char *argv[]);
void set_selecttelalmdevtModel(); //20120322
void set_selectsmsalmdevtModel(); //20120322


void insert_sms_logs(int bufidx,char * strs_in);//20120326
void insert_tel_logs(int bufidx,char * strs_in);//20120326

void insert_almsearch_logs(char * strs_in);//20120326
void insert_doublehost_logs(char * strs_in);//20120326
void insert_gtk_logs(char * strs_in);//20120326
void insert_others_logs(char * strs_in);//20120326
void insert_timingsend_logs(char * strs_in);//20120329
void insert_manusend_logs(char * strs_in);//20120329
void MultiPointPack_ALM(UCHAR type_id,UCHAR stn,UCHAR dev,UCHAR type,unsigned short first,unsigned short end,unsigned short status,int data,int almfag,int smsfalg);
int ind_alm_by_logicalname(char * logicalname,int alm_status,int stand_by,int MAN_SET,int CHAN_FAIL,int alarm_type,int sendsms_falg);
int igc_WSAStartup();
int igc_lan_out_init();

typedef struct
{
	POINTER    	        point; 
	unsigned short	    status;
	float		        value;
	char				year[5];
	char				month[3];
	char				day[3];
	char				hour[3];
	char				minute[3];
	char				second[3];
	char				ms[4];
} DB_DATA;

typedef struct
{	short	day;
	short	hour;
	short	minute;
}TIME;

typedef struct
{	char	l_name[100];
	char	c_name[100];
	char	stn_name[20];
	char	dev_name[20];
	char	short_name[50];
	char	property[20];
	int		class1;
	int		class2;
	int		class3;
	int		class4;
	int		status;
	unsigned short		old_status;
	char	dailout_flag;
	char    audioplay_flag;
	TIME	time;
}REAL_SW;

typedef struct
{	char	l_name[100];
	char	c_name[100];
	char	stn_name[50];
	char	dev_name[50];
	char	short_name[50];
	char	property[20];
	int		class1;
	int		class2;
	int		class3;
	int		class4;
	int		status;
	unsigned short		old_status;
	char	dailout_flag;
	char    audioplay_flag;
	TIME	time;
}REAL_ANA;

typedef struct
{	char	StnCName[50];
	char	StnLName[50];
	int		StartPnt;
	int		EndPnt;
	int		PntNum;
	int		PageNum;
}STN;

typedef struct 
{int vox;
 int dti;
 int hook;
 char voxname[20];
 char dtiname[20];
}CHANNEL;
CHANNEL dev[MAXCHAN];

#define MAX_TONES        	9

#define  OK             	1
#define  FAIL           	-1
#define  TRUE           	1

typedef struct {
   int   freq;          // frequency Hz
   int   deviation;     // deviation in Hz
} Freq_T;

typedef struct {
   int   time;          // time in 10ms
   int   deviation;     // deviation in ms
} State_T;

typedef struct {
   char *   str;        // informational string
   int      tid;        // tone id
   Freq_T   freq1;      // frequency 1
   Freq_T   freq2;      // frequency 2
   State_T  on;         // on time
   State_T  off;        // off time
   int      repcnt;     // repitition count
} Tone_T;

typedef struct 
{
	int real_telphone_def_num;
    char countrynum[10];
	char department[TELPHONE_NUM][20];
	char name[TELPHONE_NUM][20];
	char namecode[TELPHONE_NUM][20];
	char telphone[TELPHONE_NUM][20];
	char Other_telphones[TELPHONE_NUM][5][20];
	char manmsgsel[TELPHONE_NUM];
	char dial_out_add_num[TELPHONE_NUM][5];
}TELPHONE_DEF; 

typedef struct 
{
	int real_telphonegroup_def_num;
	char valid[TELPHONEGROUP_NUM];
	char intit[TELPHONEGROUP_NUM];
	char name[TELPHONEGROUP_NUM][20];
	char grpnamecode[TELPHONEGROUP_NUM][20];
	char weeklimit[TELPHONEGROUP_NUM][7];
	char months[TELPHONEGROUP_NUM][12];
	char days[TELPHONEGROUP_NUM][31];
	char shours[TELPHONEGROUP_NUM];
	char smins[TELPHONEGROUP_NUM];
	char ehours[TELPHONEGROUP_NUM];
	char emins[TELPHONEGROUP_NUM];
	char telphonenums[TELPHONEGROUP_NUM][TELPHONE_NUM][20];
	char namecodes[TELPHONEGROUP_NUM][TELPHONE_NUM][20];
}TELPHONEGROUP_DEF; 

typedef struct 
{
	int  num;
	char namecode[20];
	char telphonenums[TELPHONE_NUM][20];
}GROUP_TO_TELPHONE_DEF; 

typedef struct //20120326
{
	int  a_ptr;
	int  w_ptr;
	int  view_ptr;
	int  flag[MAX_LOGNUM_PER_BUF];
	char str[MAX_LOGNUM_PER_BUF][600];
}ONCALL_LOGS_BUF; 


typedef struct 
{
	int  real_oncallinquiry_def_num;
	char telpassword[20];
	char BftelAlmSpk[200];
	int  almfailtrytimes;
	int  guajiHz;
	int  guajifanwei;
	char smserrcodere[BUFSIZE/2];
	char telerrcodere[BUFSIZE/2];
	char SMS_code[ONCALLINQUIRY_NUM][100];
	char TEL_code[ONCALLINQUIRY_NUM][100];
	char longname[ONCALLINQUIRY_NUM][200];
	char logicnames[ONCALLINQUIRY_NUM][20][100];//20101216
	char namecode[ONCALLINQUIRY_NUM][TELPHONE_NUM][20];
	char groupnamecode[ONCALLINQUIRY_NUM][TELPHONEGROUP_NUM][20];
} ONCALLINQUIRY_DEF; 

typedef struct 
{
	int real_oncall_alm_buf_num;
	char longname[ALARM_NUM][100];
	char logicname[ALARM_NUM][100];
    int  SMS_ALM[ALARM_NUM];
    int  TEL_ALM[ALARM_NUM];
    char property[ALARM_NUM][20];
	int  almtype[ALARM_NUM];
	char namecode[ALARM_NUM][TELPHONE_NUM][20];
	char groupnamecode[ALARM_NUM][TELPHONEGROUP_NUM][20];
} ONCALL_ALM_DEF; 

typedef struct 
{
	int real_oncall_alm_dev_def_num;

	int  replacemanuformset[MAXDEV_NUM];
    char name[MAXDEV_NUM][40];
	char logicname[MAXDEV_NUM][20];
	int  TEL_ALM[MAXDEV_NUM];
    int  SMS_ALM[MAXDEV_NUM];
}ONCALL_ALM_DEV_DEF; 

typedef struct 
{
	int  real_timingsend_def_num;
	char longname[TIMING_NUM][100];
	char logicnames[TIMING_NUM][20][100];
	char hour[TIMING_NUM][48];
	char min[TIMING_NUM][48];
	char sended[TIMING_NUM][48];
	char valid[TIMING_NUM][48];
	char namecode[TIMING_NUM][TELPHONE_NUM][20];
	char groupnamecode[TIMING_NUM][TELPHONEGROUP_NUM][20];
} TIMINGSEND_DEF;


/*typedef struct//手动发送默认人员编号
{
	int real_manusenddefault_def_num;
	char namecode[TELPHONE_NUM][20];
} MANUSENDFAULT_DEF;
*/
/*typedef struct //自动报警缓冲区
{
	 int  add_Ptr;
	 int  send_Ptr;
     char longname[ONCALL_TEMPALMBUF_SIZE][100];
	 char logicname[ONCALL_TEMPALMBUF_SIZE][100];
	 int  SMS_ALM[ONCALL_TEMPALMBUF_SIZE];
	 int  TEL_ALM[ONCALL_TEMPALMBUF_SIZE];
	 int  ALMOPERATE[ONCALL_TEMPALMBUF_SIZE];//报警操作，写程序及读取程序判断，暂时可不用
}ONCALL_Temp_ALMBUF;
*/
typedef struct  
{
	int  add_Ptr;
	int  send_Ptr;
	int  writed_Ptr;
	int  canceled;	
	char telnum[ONCALL_DIAL_AUTOALMBUF_SIZE][TELPHONE_NUM][20];
	char longname[ONCALL_DIAL_AUTOALMBUF_SIZE][100];
	char logicname[ONCALL_DIAL_AUTOALMBUF_SIZE][100];
	char time_str[ONCALL_DIAL_AUTOALMBUF_SIZE][TIME_STR_LEN];
	char message[ONCALL_DIAL_AUTOALMBUF_SIZE][MAX_MESSAGE_SIZE];
	char dial_out_add_num[ONCALL_DIAL_AUTOALMBUF_SIZE][TELPHONE_NUM][5];
}ONCALL_DIAL_AUTOALMSENDBUF;

typedef struct  
{
	int  add_Ptr;
	int  send_Ptr;
	int  writed_Ptr;
	int  confirmPtr;
	int  canceled;	
	int  startdo;
	int  startdoPtr;
	int  confirmed[ONCALL_DIAL_MANUALMBUF_SIZE];
	char telnum  [ONCALL_DIAL_MANUALMBUF_SIZE][TELPHONE_NUM][20];
	char longname  [ONCALL_DIAL_MANUALMBUF_SIZE][100];
	char logicname [ONCALL_DIAL_MANUALMBUF_SIZE][100];
	char time_str[ONCALL_DIAL_MANUALMBUF_SIZE][TIME_STR_LEN];
	char message   [ONCALL_DIAL_MANUALMBUF_SIZE][MAX_MESSAGE_SIZE];
	char dial_out_add_num[ONCALL_DIAL_MANUALMBUF_SIZE][TELPHONE_NUM][5];
}ONCALL_DIAL_MANUALMSENDBUF;

typedef struct  
{
	int  add_Ptr;
	int  send_Ptr;
	int  writed_Ptr;
	int  canceled;	
	char telnum  [ONCALL_SMS_AUTOALMSENDBUF_SIZE][TELPHONE_NUM][20];
	char longname  [ONCALL_SMS_AUTOALMSENDBUF_SIZE][100];
	char logicname [ONCALL_SMS_AUTOALMSENDBUF_SIZE][100];
	char time_str[ONCALL_SMS_AUTOALMSENDBUF_SIZE][TIME_STR_LEN];
	char message   [ONCALL_SMS_AUTOALMSENDBUF_SIZE][MAX_MESSAGE_SIZE];

}ONCALL_SMS_AUTOALMSENDBUF;

typedef struct  
{
	int  add_Ptr;
	int  send_Ptr;
	int  writed_Ptr;
	int  confirmPtr;
	int  canceled;
	int  startdo;
	int  startdoPtr;
	int  confirmed[ONCALL_SMS_MANUALMSENDBUF_SIZE];
	char telnum [ONCALL_SMS_MANUALMSENDBUF_SIZE][TELPHONE_NUM][20];
	char longname [ONCALL_SMS_MANUALMSENDBUF_SIZE][100];
	char logicname[ONCALL_SMS_MANUALMSENDBUF_SIZE][100];
	char time_str[ONCALL_SMS_MANUALMSENDBUF_SIZE][TIME_STR_LEN];
	char message  [ONCALL_SMS_MANUALMSENDBUF_SIZE][MAX_MESSAGE_SIZE];
 
}ONCALL_SMS_MANUALMSENDBUF;

typedef struct 
{
	int  add_Ptr;
	int  send_Ptr;
	int  writed_Ptr;
	int  canceled;
	char telnum[ONCALL_SMS_MANUSENDBUF_SIZE][TELPHONE_NUM][20];
	char time_str[ONCALL_SMS_MANUSENDBUF_SIZE][TIME_STR_LEN];
	char message[ONCALL_SMS_MANUSENDBUF_SIZE][MAX_MESSAGE_SIZE];
}ONCALL_SMS_MANUSENDBUF;

typedef struct 
{
	int  add_Ptr;
	int  send_Ptr;
	int  writed_Ptr;
	int  canceled;
	char telnum[ONCALL_SMS_TIMINGSENDBUF_SIZE][TELPHONE_NUM][20];
	//char longname[ONCALL_SMS_TIMINGSENDBUF_SIZE][100];
	char logicnames[ONCALL_SMS_TIMINGSENDBUF_SIZE][20][100];
	char time_str[ONCALL_SMS_TIMINGSENDBUF_SIZE][20];
	char sendlongnames[ONCALL_SMS_TIMINGSENDBUF_SIZE][20][40];//20110224
	char message[ONCALL_SMS_TIMINGSENDBUF_SIZE][MAX_MESSAGE_SIZE*2];
}ONCALL_SMS_TIMINGSENDBUF;

typedef struct 
{
	int  writed_Ptr;
	int  add_Ptr;
	int  canceled;
	char time_str[MAX_WRITEFILEBUFNUM][20];
	char message[MAX_WRITEFILEBUFNUM][1024];
}ONCALL_SMSQUIRY_WRITEBUF;

typedef struct 
{
	int  writed_Ptr;
	int  add_Ptr;
	int  canceled;
	char time_str[MAX_WRITEFILEBUFNUM][20];
	char message[MAX_WRITEFILEBUFNUM][1024];
}ONCALL_DIALQUIRY_WRITEBUF;


char fault_alm_logicnames[100]; //20120327

typedef struct 
{
	int use_over;
	int use_sc;
	int  len;
	int len1;
	char NumStr[20];
	unsigned char strText[BUFSIZE];
	unsigned char send_msg[BUFSIZE];
	unsigned char rTONNPI0;
	unsigned char rTONNPI1;
}ONCALL_SMS_SEND_TEMPBUF;


TELPHONE_DEF            telphone_def; 
TELPHONEGROUP_DEF       telphonegroup_def;   
TIMINGSEND_DEF          timingsend_def;  
 
ONCALLINQUIRY_DEF       oncallinquiry_def;
ONCALL_ALM_DEF          oncall_alm_def;
ONCALL_ALM_DEV_DEF      oncall_alm_dev_def;  
 

ONCALL_DIAL_AUTOALMSENDBUF oncall_dial_autoalmsendbuf;
ONCALL_DIAL_MANUALMSENDBUF oncall_dial_manualmsendbuf;
ONCALL_SMS_AUTOALMSENDBUF  oncall_sms_autoalmsendbuf;
ONCALL_SMS_MANUALMSENDBUF  oncall_sms_manualmsendbuf;
ONCALL_SMS_MANUSENDBUF     oncall_sms_manusendbuf;
ONCALL_SMS_TIMINGSENDBUF   oncall_sms_timingsendbuf;
//ONCALL_DIALQUIRY_WRITEBUF  oncallL_dialquiry_writebuf;
//ONCALL_SMSQUIRY_WRITEBUF   oncallL_smsquiry_writebuf;//20120329
ONCALL_SMS_SEND_TEMPBUF    _oncallL_sms_send_tempbufs[MAX_sms_dev_num];//20120329



int  oncall_sms_err_times[MAX_sms_dev_num];//20120327
int  lock_oncall_sms_err_times;

ONCALL_LOGS_BUF  oncall_sms_logsbufs[MAX_sms_dev_num];//20120326
ONCALL_LOGS_BUF  oncall_tel_logsbufs[MAXCHAN];//20120326
ONCALL_LOGS_BUF  oncall_almsearch_logsbuf;//20120326
ONCALL_LOGS_BUF  oncall_doublehost_logsbuf;//20120326
ONCALL_LOGS_BUF  oncall_gtk_logsbuf;//20120326
ONCALL_LOGS_BUF  oncall_others_logsbuf;//20120326
ONCALL_LOGS_BUF  oncall_timingsend_logsbuf;//20120329
ONCALL_LOGS_BUF  oncall_manusend_logsbuf;//20120329

int  threadstat_Oncall_almsearch;//20120330
int  threadstat_oncallTelQuery;//20120330
int  threadstat_Oncall_writefie;//20120330
int  threadstat_Oncall_ALM_manuconfirm;//20120330
int  threadstat_Oncall_sms_timingsend;//20120330
int  threadstat_SMS_SENDRECEIVE;//20120330

int            ONCALLSMS_VALID;				
int            ONCALLDIALOGIC_VALID;
char           ONCALLSTATION_NAME[10];	
int	           ONCAL_LOGSRECORD_SAVEDAY;
//int	       ONCALLSMS_RECORD_SAVEDAY;//20120326

//
char           _ONCALLCOM_PORTS[MAX_sms_dev_num][6];
char           _BAK_ONCALLCOM_PORTS[MAX_sms_dev_num][6];//20120327	    	
DWORD	       _ONCALLBAUD_RATES[MAX_sms_dev_num];
int            _ONCALLPARITY_TYPES[MAX_sms_dev_num];
int            _SMS_SENDRECEIVE_COM_STATS[MAX_sms_dev_num];//串口状态
char           _strSmsCenterS[MAX_sms_dev_num][20]; 
int            _ONCALL_SMS_DEV_TYPES[MAX_sms_dev_num];
//

char           ONCALLDIAL_OUT_PORT[20]; 	
char           ONCALLDIAL_IN_PORT[20]; 
unsigned char defaultTONNPI[2];
int printflevel;
int readedxml;
int telusexmlbufoverR,telusexmlbufoverA,smsusexmlbufover;
int first_glag_forgtkui;
 
DWORD ONCALLTelQuery_IDThread,
ONCALLTelDial_IDThread,
SMS_SENDRECEIVE_HANDLE_IDThread,
ONCALLWRITEFILE_HANDLE_IDThread,
ONCALL_ALMSEARCH_HANDLE_IDThread,
Oncall_ALM_manuconfirm_HANDLE_IDThread,
Oncall_sms_timingsend_HANDLE_IDThread,
Oncall_sms_send_HANDLE_IDThreadS[MAX_sms_dev_num],
Oncall_sms_reeive_HANDLE_IDThreadS[MAX_sms_dev_num];

int   ONCALLTelQuery_IDThread_run,
ONCALLTelDial_IDThread_run,
SMS_SENDRECEIVE_HANDLE_IDThread_run,
ONCALLWRITEFILE_HANDLE_IDThread_run,
ONCALL_ALMSEARCH_HANDLE_IDThread_run,
Oncall_ALM_manuconfirm_HANDLE_IDThread_runl,
Oncall_sms_timingsend_HANDLE_IDThread_run,
Oncall_sms_send_HANDLE_IDThreadS_run[MAX_sms_dev_num],
Oncall_sms_reeive_HANDLE_IDThreadS_run[MAX_sms_dev_num];

int CHANSTARTNUM,CHANNUM;
char CHANUSE[MAXCHAN];



GtkWidget *window;
GtkTextView * mansendtexttextview;

 
char *voxchannels[];

 
int gtk_onvall_mainform_created;//20120322
#endif
