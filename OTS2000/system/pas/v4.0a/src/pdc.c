
#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../pas/v4.0a/inc/pdc_def.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"

#ifndef	WINDOWS_OS
#include	<pthread.h>
#endif


#define	MAX_PDC_TASK_NUM	20
int			pdc_tasks_num;
PDC_TASK	pdc_tasks[MAX_PDC_TASK_NUM];



/***  check  period  time  ***/
int  PdcCheck(PDC_TASK *pdc_task)
{
    struct  tm timeptr;
    time_t  tt;
       
	tt = time(NULL);                          
    timeptr=*localtime(&tt);
	if(pdc_task->type==0) /*P*/
	{
		if((tt%pdc_task->task_period)<pdc_task->check_period)
			return 0;
		else
			return 1;
	}
	else /*T 最小执行单位为1天*/
	{
		if(pdc_task->month!=0 && (timeptr.tm_mon+1)!=pdc_task->month)
			return 1;
		if(pdc_task->day!=0 && (timeptr.tm_mday!=pdc_task->day))
			return 1;
		if(timeptr.tm_hour!=pdc_task->hour)
			return 1;
		if(timeptr.tm_min!=pdc_task->min)
			return 1;
		if(timeptr.tm_sec<pdc_task->sec)
			return 1;
		if((timeptr.tm_sec-pdc_task->sec)<pdc_task->check_period)
			return 0;
		else
			return 1;
	}
}


int PeriodFileLoad(void)
{
	char	filename[256];    
	unsigned int		j;    	  	
	FILE	*fpp;
	char	period_str[64];
	char	type_str[12];
	char	err_str[128];
	PDC_TASK	task;
	int		task_num;
    
	sprintf(filename, "%s%s", SYS_DEFINE_PATH, PDC_PERIOD_DEFINE_FILE);
    if((fpp= fopen( filename,"r" )) == NULL )
    {
		sprintf(err_str, "Fail to open %s", filename);
		Err_log(err_str);
		return -1;
    }

	task_num= 0;
    while(fscanf(fpp, "%s %s %s", task.taskname, type_str, period_str ) == 3 )
    {
		if(task.taskname[0]=='#' || task.taskname[0]=='!')
			continue;

		if(type_str[0]=='P')
			task.type= 0;
		else if(type_str[0]=='T')
			task.type= 1;
		else
		{
			sprintf(err_str, "%s period define format error\n", task.taskname);
			Err_log(err_str);
			continue;	     
		}

		for(j=0; j<strlen(period_str); j++)
		{
			if(period_str[j]<'0' || period_str[j]>'9')
				period_str[j]= ' ';
		}

		if(sscanf(period_str, "%d %d %d %d %d", &task.month, &task.day, 
			&task.hour, &task.min, &task.sec)!=5)
		{
			sprintf(err_str, "%s period define format error\n", task.taskname);
			Err_log(err_str);
			continue;	     
		}
		task.task_period= task.sec+task.min*60+task.hour*3600;

		if(!strcmp(task.taskname, "CALCU"))
		{
			task.thread= CalcuFn;
			task.check_period= 1;
			task.type= 0;
			task.param= 0;
		}
		else if(!strcmp(task.taskname, "CALCU1"))
		{
			task.check_period= 10;
			if(task.task_period<task.check_period)
			{
				sprintf(err_str, "CALCU1 need period equal or large than %d sec", task.check_period);
				Err_log(err_str);
				return -1;	     
			}
			task.thread= CalcuFn;
			task.type= 0;
			task.param= 1;
		}
		else if(!strcmp(task.taskname, "RPTM"))
		{
			task.check_period= 5;
			if(task.task_period<task.check_period)
			{
				sprintf(err_str, "RPTM need period equal or large than %d sec", task.check_period);
				Err_log(err_str);
				return -1;	     
			}
			task.thread= rptm;
			task.type= 0;
			task.param= 0;
		}
		else if(!strcmp(task.taskname, "RPTH"))
		{
			task.check_period= 5;
			if(task.task_period<task.check_period)
			{
				sprintf(err_str, "RPTH need period equal or large than %d sec", task.check_period);
				Err_log(err_str);
				return -1;	     
			}
			task.thread= rpth;
			task.type= 0;
			task.param= 0;
		}
		else if(!strcmp(task.taskname, "TEMPCHA"))
		{
			task.check_period= 5;
			if(task.task_period<task.check_period)
			{
				sprintf(err_str, "TEMPCHA need period equal or large than %d sec", task.check_period);
				Err_log(err_str);
				return -1;	     
			}
			task.thread= tempcha;
			task.type= 0;
			task.param= 0;
		}
		else if(!strcmp(task.taskname, "SAFE_DAY"))
		{
			task.check_period= 10;
			if(task.task_period<task.check_period)
			{
				sprintf(err_str, "SAFE_DAY need period equal or large than %d sec", task.check_period);
				Err_log(err_str);
				return -1;	     
			}
			task.thread= safe_day;
			task.type= 1;
			task.param= 0;
		}
		else if(!strcmp(task.taskname, "RCPDATA"))
		{
			task.check_period= 10;
			if(task.task_period<task.check_period)
			{
				sprintf(err_str, "RCPDATA need period equal or large than %d sec", task.check_period);
				Err_log(err_str);
				return -1;	     
			}
			task.thread= copy_rpt_data;
			task.type= 1;
			task.param= 0;
		}
		else if(!strcmp(task.taskname, "DELHISFILE"))
		{
			task.check_period= 10;
			if(task.task_period<task.check_period)
			{
				sprintf(err_str, "DELHISFILE need period equal or large than %d sec", task.check_period);
				Err_log(err_str);
				return -1;	     
			}
			task.thread= delhistory;
			task.type= 1;
			task.param= 0;
		}
		else if(!strcmp(task.taskname, "SYSALLSEND"))
		{
			if(task.task_period<60)
			{
				sprintf(err_str, "SYSALLSEND need period equal or large than 60 sec");
				Err_log(err_str);
				return -1;	     
			}
			task.check_period= task.task_period;
			task.thread= SysRtuAllSend;
			task.type= 0;
			task.param= 0;
		}
		else if(!strcmp(task.taskname, "RDBUPDATE"))
		{
			task.check_period= 60;
			if(task.task_period<240)
			{
				sprintf(err_str, "RDBUPDATE need period equal or large than 240 sec");
				Err_log(err_str);
				return -1;	     
			}
			task.thread= RdbUpdateCheck;
			task.type= 0;
			task.param= 0;
		}
		/////////////////////////////////////////////////////////
		else if(!strcmp(task.taskname, "GETDISKSPACE"))
		{
			task.check_period= 10;
			if(task.task_period<task.check_period)
			{
				sprintf(err_str, "GETDISKSPACE need period equal or large than %d sec", task.check_period);
				Err_log(err_str);
				return -1;	     
			}
			task.thread= GetDiskSpace;
			task.type= 0;
			task.param= 0;
		}
		/////////////////////////////////////////////////////////
		else
		{
			sprintf(err_str, "Unsupported pdc task: %s\n",task.taskname);
			Err_log(err_str);
			continue;	     
		}

		if(task_num>=MAX_PDC_TASK_NUM)
		{
			sprintf(err_str, "Arrive max pdc task num: %d", MAX_PDC_TASK_NUM);
			Err_log(err_str);
			break;
		}
		pdc_tasks[task_num]= task;
		task_num++;
	}    
	pdc_tasks_num= task_num;
	fclose ( fpp );
	return 0;
}



int main(int argc, char *argv[])
{  	
    int	        i;     
	char		err_str[128];
	DWORD		h_terminated;
	pthread_t	h_thread[MAX_PDC_TASK_NUM];
    
#ifdef	WINDOWS_OS
	WSADATA		WSAData;
#endif
	
	CreateLockFile(".pdc.lock");
	if(-1==InitSysLog(H9000_MODULE_PDC, "PDC"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	

#ifdef	WINDOWS_OS
	if(WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
	{
		Err_log("Fail to initialize net!\n");
		exit(-1);
	}
#endif

	if(-1==MountDB(NULL))
	{
		Err_log("Fail to Mount RDB");
		exit(-1);
	}

	if(!lan_out_init())
	{
		Err_log("Fail to init lan_out");
		exit(-1);
	}

	if(-1==PeriodFileLoad())
	{
		Err_log("Fail to load period define file");
		exit(-1);
	}

#ifdef	WINDOWS_OS
	for(i=0; i<pdc_tasks_num; i++)
	{
		h_thread[i]=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pdc_tasks[i].thread, 
			&pdc_tasks[i], 0, NULL);
		if(!h_thread[i])
  		{
			sprintf(err_str, "Fail to create thread %s!", pdc_tasks[i].taskname);
			Err_log(err_str);
			exit(-1);
  		} 
	}
  	
	while( h_terminated= WaitForMultipleObjects(pdc_tasks_num, h_thread, FALSE, INFINITE) )
	{
		sleep(1);
		if(h_terminated<WAIT_OBJECT_0 || h_terminated>(WAIT_OBJECT_0+pdc_tasks_num-1))
		{
			Err_log("WaitForMultipleObjects return error");
			continue;
		}

		i= h_terminated-WAIT_OBJECT_0;
		sprintf(err_str, "restart thread %d", i);
		Err_log(err_str);

		h_thread[i]=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pdc_tasks[i].thread, 
			&pdc_tasks[i], 0, NULL);
		if(!h_thread[i])
  		{
			sprintf(err_str, "Fail to create thread %s", pdc_tasks[i].taskname);
			Err_log(err_str);
			exit(-1);
  		} 
  	}
#else
	for(i=0; i<pdc_tasks_num; i++)
	{
		sleep(1);//cxs add for aix
 		if( pthread_create(&h_thread[i], NULL, pdc_tasks[i].thread, &pdc_tasks[i]) )
 		{
			sprintf(err_str, "Fail to create thread %s!", pdc_tasks[i].taskname);
			Err_log(err_str);
			exit(-1);
  		} 
	}
  	
/*
	while( !thr_join(0, &h_terminated, NULL) )
	{
		sleep(1);
		for(i=0; i<pdc_tasks_num; i++)
		{
 			if( h_terminated==h_thread[i] )
				break;
		}
		if(i>=pdc_tasks_num)
		{
			Err_log("Error terminated thread ID");
			continue;
		}

		sprintf(err_str, "restart thread %s", pdc_tasks[i].taskname);
		Err_log(err_str);

		if( thr_create(NULL, 0, pdc_tasks[i].thread, &pdc_tasks[i], 0, &h_thread[i]) )	  
  		{
			sprintf(err_str, "Fail to create thread %s!", pdc_tasks[i].taskname);
			Err_log(err_str);
			exit(-1);
		}
	}
*/
	for(;;)
		sleep(100000);
#endif

}


