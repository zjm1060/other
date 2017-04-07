#ifndef	_PDC_MODULE_DEF_H
#define	_PDC_MODULE_DEF_H

#include	"../../../dms/v4.0a/inc/dms_com.h"


typedef struct
{
	int		month;
	int		day;
	int		hour;
	int		min;
	int		sec;
	int		task_period;
	int		check_period;
	int		type;
	char	taskname[24];
	void*	(*thread)();
	int		param;
} PDC_TASK;


int		CalcuFileLoad(short i);
int		PdcCheck(PDC_TASK *pdc_task);
void*	rptm(PDC_TASK *pTask);
void*	rpth(PDC_TASK *pTask);
void*	tempcha(PDC_TASK *pTask);
void*	safe_day(PDC_TASK *pTask);
void*	CalcuFn(PDC_TASK *pTask);
void*	copy_rpt_data(PDC_TASK *pTask);
void*	delhistory(PDC_TASK *pTask);
void*	SysRtuAllSend(PDC_TASK *pTask);
void*	RdbUpdateCheck(PDC_TASK *pTask);
//////////////////////////////
void*	GetDiskSpace(PDC_TASK *pTask);
///////////////////////////

#ifdef	WINDOWS_OS
#define		pthread_t				HANDLE
#define 	bzero(addr, size)		memset(addr, 0, size)
#define		sleep(sec)				Sleep(sec*1000)
#else
#define		DWORD					pthread_t
#define		closesocket(socket)		close(socket)
#endif




#endif /*_PDC_MODULE_DEF_H*/                                 
				    

