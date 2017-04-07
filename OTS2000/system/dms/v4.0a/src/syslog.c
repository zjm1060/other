#include	<string.h>
#include	<stdarg.h>
#include "../../../dps/v4.0a/inc/ipc_common.h"
#include "../inc/syslog.h"



static SHM_LOG	*shm_log_addr= (SHM_LOG*)-1;
static	UCHAR	h9000_module_id;
static char h9000_module_name[MAX_H9000_MODULE_NAME_SIZE];


void	SetModuleName(char *moduleName)
{
	if(shm_log_addr==(SHM_LOG*)-1)
		return;
	if(strlen(moduleName)>=MAX_H9000_MODULE_NAME_SIZE)
	{
		strncpy(shm_log_addr->module[h9000_module_id].name, moduleName, MAX_H9000_MODULE_NAME_SIZE-1);
		shm_log_addr->module[h9000_module_id].name[MAX_H9000_MODULE_NAME_SIZE-1]= 0;
	}
	else
		strcpy(shm_log_addr->module[h9000_module_id].name, moduleName);
}

int	GetModuleName(int moduleId, char *moduleName)
{
	if(shm_log_addr==(SHM_LOG*)-1)
		return -1;
	if(moduleId==0)
	{
		strcpy(moduleName, "ALL");
		return 0;
	}
	if(moduleId<1 || moduleId>=MAX_H9000_MODULE_NUM)
	{
		SendErrMsg(1, "module id %d error: range 1~%d", moduleId, MAX_H9000_MODULE_NUM-1);
		return -1;
	}
	else
	{
		strcpy(moduleName, shm_log_addr->module[moduleId].name);
		return 0;
	}
}

void	SetModuleId(enum H9000_MODULE_ID moduleId)
{
	if(moduleId<1 || moduleId>=MAX_H9000_MODULE_NUM)
	{
		moduleId= 0;
		SendErrMsg(1, "set module id %d error: range 1~%d, will set 0", moduleId, MAX_H9000_MODULE_NUM-1);
	}
	h9000_module_id= moduleId;
}

UCHAR	GetModuleId(void)
{
	return	h9000_module_id;
}

int InitSysLog(enum H9000_MODULE_ID moduleId, char *moduleName)
{
	strncpy(h9000_module_name, moduleName, MAX_H9000_MODULE_NAME_SIZE-1);

	shm_log_addr= (SHM_LOG*) LinktoShareMem( SHM_SYSLOG_KEY_CHAR );
	if( shm_log_addr == (SHM_LOG*)-1 )
	{
		Err_log("Fail to link log record shm");
		return -1;
	}	

	if( LinktoSemSet(SEM_WR_SYSLOG) == -1 )
	{
		Err_log("Fail to link log record semaphore\n");
		return -1;
	}

	SetModuleId(moduleId);
	SetModuleName(moduleName);
	if(moduleId<1 || moduleId>=MAX_H9000_MODULE_NUM)
		moduleId= 0;
	shm_log_addr->module[moduleId].filter= H9000_LOG_ERROR|H9000_LOG_WARN;
	shm_log_addr->module[moduleId].fileFlag= 1;
	
#ifdef WINDOWS_OS
	//if(strcmp(moduleName, "LogMgr"))
	//	FreeConsole();
#endif

	return 0;
}

int GetSysLogCount(void)
{
	if(shm_log_addr==(SHM_LOG*)-1)
		return -1;
	return shm_log_addr->count;
}
int GetSysLogHeader(int *header, int *count)
{
	if(shm_log_addr==(SHM_LOG*)-1)
	{
		*header= 0;
		*count= 0;
		return -1;
	}
	WaitforSem(SEM_WR_SYSLOG, -1);
	*header= shm_log_addr->header;
	*count= shm_log_addr->count;
	ReleaseWriteSem(SEM_WR_SYSLOG);
	return 0;
}
int GetSysLogTail(int *tail, int *count)
{
	if(shm_log_addr==(SHM_LOG*)-1)
	{
		*tail= 0;
		*count= 0;
		return -1;
	}
	WaitforSem(SEM_WR_SYSLOG, -1);
	*tail= ((shm_log_addr->header+MAX_LOG_NUM)-shm_log_addr->count)%MAX_LOG_NUM;
	*count= shm_log_addr->count;
	ReleaseWriteSem(SEM_WR_SYSLOG);
	return 0;
}
int GetSysLogInfo(int index, LOG_INFO *pLogInfo)
{
	if(shm_log_addr==(SHM_LOG*)-1)
		return -1;

	*pLogInfo= shm_log_addr->log[index%MAX_LOG_NUM];
	return 0;
}
static int InsertLogInfo(LOG_INFO *pLogInfo, int fileFlag)
{
	if(shm_log_addr==(SHM_LOG*)-1)
		return -1;

	WaitforSem(SEM_WR_SYSLOG, -1);
	
	shm_log_addr->log[shm_log_addr->header]= *pLogInfo;
	
	shm_log_addr->header= (shm_log_addr->header+1)%MAX_LOG_NUM;
	if(shm_log_addr->count<MAX_LOG_NUM)
		shm_log_addr->count++;
	
	if(fileFlag)
	{
		char filename[256];
		FILE	*fp;

		sprintf(filename, "%s%s%04d%02d%02d_err_log.txt", ERROR_LOG_PATH, h9000_module_name,
			pLogInfo->datetime.year, pLogInfo->datetime.month, pLogInfo->datetime.day);
		if(fp= fopen(filename, "a+"))
		{
			fprintf(fp, "%02d:%02d:%02d:%03d	%s\n", pLogInfo->datetime.hour,
				pLogInfo->datetime.minute, pLogInfo->datetime.second, pLogInfo->datetime.ms,
				pLogInfo->info);
			fclose(fp);
		}
	}
	ReleaseWriteSem(SEM_WR_SYSLOG);
	return 0;
}

BOOL GetLogOutputFilter(enum H9000_MODULE_ID moduleId, enum H9000_LOG_TYPE logType)
{
	if(shm_log_addr==(SHM_LOG*)-1)
		return TRUE;
	
	if(moduleId<0 || moduleId>=MAX_H9000_MODULE_NUM)
	{
		SendErrMsg(1, "module id %d error: range 0~%d", moduleId, MAX_H9000_MODULE_NUM-1);
		return FALSE;
	}
	return shm_log_addr->module[moduleId].filter & logType;
}


void SetLogOutputFilter(enum H9000_MODULE_ID moduleId, enum H9000_LOG_TYPE logType, BOOL isTrue)
{
	if(shm_log_addr==(SHM_LOG*)-1)
		return;

	if(moduleId<0 || moduleId>=MAX_H9000_MODULE_NUM)
	{
		SendErrMsg(1, "module id %d error: range 0~%d", moduleId, MAX_H9000_MODULE_NUM-1);
		return;
	}
	if(isTrue)
		shm_log_addr->module[moduleId].filter |= logType;
	else
		shm_log_addr->module[moduleId].filter &= ~logType;

	if(moduleId==0)
	{
		int id;

		for(id=1; id<MAX_H9000_MODULE_NUM; id++)
			SetLogOutputFilter(id, logType, isTrue);
	}
}

BOOL GetLogOutputFileFlag(enum H9000_MODULE_ID moduleId)
{
	if(shm_log_addr==(SHM_LOG*)-1)
		return TRUE;
	
	if(moduleId<0 || moduleId>=MAX_H9000_MODULE_NUM)
	{
		SendErrMsg(1, "module id %d error: range 0~%d", moduleId, MAX_H9000_MODULE_NUM-1);
		return FALSE;
	}
	return shm_log_addr->module[moduleId].fileFlag;
}


void SetLogOutputFileFlag(enum H9000_MODULE_ID moduleId, BOOL isTrue)
{
	if(shm_log_addr==(SHM_LOG*)-1)
		return;
	
	if(moduleId<0 || moduleId>=MAX_H9000_MODULE_NUM)
	{
		SendErrMsg(1, "module id %d error: range 0~%d", moduleId, MAX_H9000_MODULE_NUM-1);
		return;
	}
	shm_log_addr->module[moduleId].fileFlag = isTrue;

	if(moduleId==0)
	{
		int id;

		for(id=1; id<MAX_H9000_MODULE_NUM; id++)
			SetLogOutputFileFlag(id, isTrue);
	}
}



void	SendTraceMsg(                                       
				int logType,		// log type
				int reserve,	
				const char* format,// from here the function uses printf syntax
				...
				)
{
    va_list ap;
	LOG_INFO logInfo;
	char	buff[1024];
	char timeStr[256];

		
		va_start(ap, format);
		vsprintf(buff, format, ap);
		va_end(ap);

		GetSysTimeStr(timeStr);
		if(reserve<2)
		{
			printf("\n%s %s %s", timeStr, h9000_module_name, buff);
		}
		return;

	if(!(logType & shm_log_addr->module[h9000_module_id].filter))
		return;

	GetSysTime(&logInfo.datetime);
	logInfo.module_id= h9000_module_id;
	logInfo.logType= logType;
	
    va_start(ap, format);
    vsprintf(buff, format, ap);
	va_end(ap);

	strncpy(logInfo.info, buff, MAX_LOG_INFO_SIZE-1);
	logInfo.info[MAX_LOG_INFO_SIZE-1]= 0;

	InsertLogInfo(&logInfo, shm_log_addr->module[h9000_module_id].fileFlag);	
}

void Err_log( char *info_buf )
{
	SendTraceMsg(H9000_LOG_ERROR,1,"%s",info_buf);
} 

int log_printf(int logType,short errlevel,enum app_code appid,char *info)
{
	SendTraceMsg(logType,errlevel,"%s",info);
	return TRUE;
}

int log_write(int logType,short errlevel,char *info)
{
	SendTraceMsg(logType,errlevel,"%s",info);
	return TRUE;
}

int err_write(char *info)
{
	SendTraceMsg(H9000_LOG_ERROR,1,"%s",info);
	return TRUE;
}

int warn_write(char *info)
{
	SendTraceMsg(H9000_LOG_WARN,1,"%s",info);
	return TRUE;
}

int info_write(char *info)
{
	SendTraceMsg(H9000_LOG_INFO,1,"%s",info);
	return TRUE;
}

void SendErrMsg(int reserve, const char* format, ... )
{
    va_list ap;
	char buff[1024];

    va_start(ap, format);
    vsprintf(buff, format, ap);
	va_end(ap);

	SendTraceMsg(H9000_LOG_ERROR,reserve,"%s", buff);
}

void SendWarnMsg(int reserve, const char* format, ... )
{
    va_list ap;
	char buff[1024];
	
    va_start(ap, format);
    vsprintf(buff, format, ap);
	va_end(ap);
	
	SendTraceMsg(H9000_LOG_WARN,reserve,"%s", buff);
}

void SendInfoMsg(int reserve, const char* format, ... )
{
    va_list ap;
	char buff[1024];
	
    va_start(ap, format);
    vsprintf(buff, format, ap);
	va_end(ap);
	
	SendTraceMsg(H9000_LOG_INFO,reserve,"%s", buff);
}

void SendSendMsg(int reserve, const char* format, ... )
{
    va_list ap;
	char buff[1024];
	
    va_start(ap, format);
    vsprintf(buff, format, ap);
	va_end(ap);
	
	SendTraceMsg(H9000_LOG_SEND,reserve,"%s", buff);
}

void SendRecvMsg(int reserve, const char* format, ... )
{
    va_list ap;
	char buff[1024];
	
    va_start(ap, format);
    vsprintf(buff, format, ap);
	va_end(ap);
	
	SendTraceMsg(H9000_LOG_RECV,reserve,"%s", buff);
}

