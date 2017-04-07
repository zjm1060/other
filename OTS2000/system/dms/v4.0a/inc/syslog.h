#ifndef _SYS_LOG_H
#define _SYS_LOG_H

#include	"../inc/os_type_def.h"
#include	"../inc/dms_com.h"


#ifdef __cplusplus
extern "C"{
#endif 


#define	MAX_H9000_MODULE_NUM	256
#define	MAX_H9000_MODULE_NAME_SIZE	36
enum	H9000_MODULE_ID	{
						H9000_MODULE_OIX=1,
						H9000_MODULE_IPM=2,
						H9000_MODULE_AUDALM=3,
						H9000_MODULE_DMSMON=4,
						H9000_MODULE_DBREAD=5,
						H9000_MODULE_SYSMGR=6,
						H9000_MODULE_DBWRITE=7,
						H9000_MODULE_FAULTTABLE=8,
						H9000_MODULE_PDC=9,
						H9000_MODULE_DPP=10,
						H9000_MODULE_PLCSCAN=11,
						H9000_MODULE_PLCCMD=12,
						H9000_MODULE_PLCMON=13,
						H9000_MODULE_LANFORWARD=14,
						H9000_MODULE_LANINB=15,
						H9000_MODULE_LANOUTB=16,
						H9000_MODULE_EVENTSEARCH=17,
						H9000_MODULE_SYSHINT=18,
						H9000_MODULE_SYSSTOP=19,
						H9000_MODULE_HMGR=20,
						H9000_MODULE_CURVESCAN=21,
						H9000_MODULE_ALARMSCAN=22,
						H9000_MODULE_REVERTSCAN=23,
						H9000_MODULE_HDBWRITE=24,
						H9000_MODULE_CURVEWRITE=25,
						H9000_MODULE_WEBWRITE=26,
						H9000_MODULE_LOGWRITE=27,
						H9000_MODULE_PERIODWRITE=28,
						H9000_MODULE_SGZY=31,
						H9000_MODULE_TRENDDATAREC=32,
						H9000_MODULE_INTERLOCKNAME=36,
						H9000_MODULE_CALCUNAME=37,
						H9000_MODULE_TSERVICE=39,
						H9000_MODULE_AGC=41,
						H9000_MODULE_AVC=42,
						H9000_MODULE_IEC104=51,
						H9000_MODULE_IEC104S=52,
						H9000_MODULE_IEC101S=53,
						H9000_MODULE_ICCPMAIN=55,
						H9000_MODULE_MBM=56,
						H9000_MODULE_MBS=57,
						H9000_MODULE_OBJCALC=58,
						H9000_MODULE_READGPS=59,
						H9000_MODULE_WEBSCAN=60,
						H9000_MODULE_NETMON=61,
						H9000_MODULE_LOGMGR=62,
						H9000_MODULE_TELALM=81,
						H9000_MODULE_61850_CLIENT=82,
						H9000_MODULE_IGC=91,  //cxs
						H9000_MODULE_FAULT_AUDALM=92,  //cxs
						H9000_MODULE_SENDALL=93,//dm
						H9000_MODULE_SIMUSERVICE=94//dm
					};


enum	H9000_LOG_TYPE  {
		H9000_LOG_ERROR=0x00000001,
		H9000_LOG_WARN=	0x00000002, 
		H9000_LOG_INFO=	0x00000004,
		H9000_LOG_SEND=	0x00000008,
		H9000_LOG_RECV=	0x00000010,
};

enum	app_code //reserved for function log_printf
{
	app_code
};
/******************* constant and struct definition of log module *******************************************************/

#define MAX_LOG_NUM	20000			/* Max log record number saved in log shared memory */	
#define MAX_LOG_INFO_SIZE   256		/* Max char number of a log record info*/
#define SHM_LOG_SIZE    sizeof(struct shm_log)

/******************** log info struct for LogWrite Function as Parameter **************************************************/
typedef struct log_info
{
	enum H9000_MODULE_ID module_id;		/* app id */
	DATE_TIME datetime;		/* occurring time of error */
//	short errLevel;			/* error code id */
	enum H9000_LOG_TYPE logType;			
	char info[MAX_LOG_INFO_SIZE]; 	/* log information */
}LOG_INFO;  

typedef struct _module_info
{
	int	filter;
	int fileFlag;
	char name[MAX_H9000_MODULE_NAME_SIZE];
}MODULE_INFO;  

typedef struct shm_log 
{
	unsigned int	header;				/* head pointer of log shared memory */
	unsigned int	count;
	MODULE_INFO		module[MAX_H9000_MODULE_NUM];
	struct log_info log[MAX_LOG_NUM];
} SHM_LOG; 


/****************************************************************************************************************************
write error log to file api.
parameter:
	errtype:		error type, ERR (1) : Error  WRN (2): Warning  INF (3): Infomation 
	errcode:		error level id, 1: the most important, 2: the second ...
	appid:			app thread code,defined in enum app_code
	info: 			error log information
note:	
	progammer needn't to specify the time when error occurs. it will be automatically added by
	Log_Write() function. 
	
	Log_init() function should be called first.
****************************************************************************************************************************/


void	SetModuleName( char *module );
int	GetModuleName(int moduleId, char *moduleName);
void	SetModuleId(enum H9000_MODULE_ID moduleId);
UCHAR	GetModuleId(void);
int		InitSysLog(enum H9000_MODULE_ID moduleId, char *moduleName);
BOOL GetLogOutputFilter(enum H9000_MODULE_ID moduleId, enum H9000_LOG_TYPE logType);
void SetLogOutputFilter(enum H9000_MODULE_ID moduleId, enum H9000_LOG_TYPE logType, BOOL isTrue);
BOOL GetLogOutputFileFlag(enum H9000_MODULE_ID moduleId);
void SetLogOutputFileFlag(enum H9000_MODULE_ID moduleId, BOOL isTrue);

void	SendTraceMsg(int logType, int reserve, const char* format, ...);
void	Err_log( char *info_buf );
int log_printf(int logType,short errlevel,enum app_code appid,char *info);
int log_write(int logType,short errlevel,char *info);
int err_write(char *info);
int warn_write(char *info);
int info_write(char *info);
void SendErrMsg(int reserve, const char* format, ... );
void SendWarnMsg(int reserve, const char* format, ... );
void SendInfoMsg(int reserve, const char* format, ... );
void SendSendMsg(int reserve, const char* format, ... );
void SendRecvMsg(int reserve, const char* format, ... );

int GetSysLogCount(void);
int GetSysLogHeader(int *header, int *count);
int GetSysLogTail(int *tail, int *count);
int GetSysLogInfo(int index, LOG_INFO *pLogInfo);


#ifdef __cplusplus
}
#endif


#endif //_SYS_LOG_H
