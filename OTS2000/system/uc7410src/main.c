/****************************************************************************************
    File name:	main.c
	
    History: Ö÷³ÌÐò   
    Version	µËÐ¡¸Õ 	Date	05.8	Comment 
****************************************************************************************/

#include        <stdio.h>
#include        <stdlib.h>
#include        <fcntl.h>
#include        <errno.h>
#include        <termios.h>
#include        <time.h>
#include        <sys/types.h>
#include        <sys/socket.h>
#include        <sys/ioctl.h>
#include        <linux/if.h>
#include        <netinet/in.h>
#include        <arpa/inet.h>
#include		<moxadevice.h>
#include	    <stddef.h>
#include	    <unistd.h>
#include        <pthread.h>
#include		<semaphore.h>
#include		<linux/sem.h>
#include		<stdarg.h>
#include 		<signal.h>

#include "main.h"


void abplc(void);


TASK_CONF_INFO	config_info;


int GetCommTasks(void)
{
	char	*filename=CONFIG_DEFINE_FILE;
	char	*sect="COMM_TASK";
	char	buff[1024];
	char	seps[]   = "\n";
	char	*token;
	char	strbuf[MAX_STR_NUM_IN_LINE][MAX_CHAR_NUM_IN_LINE];
	int		str_num;
	
	if(!GetProfSection(filename, sect, buff, sizeof(buff)))
		return FALSE;
	
	share_table->dev_num= 0;

	token= (char*)strtok( buff, seps );
	while( token != NULL )
	{
		if(token[0]=='#' || token[0]=='!')
		{
			token = (char*)strtok( NULL, seps );
			continue;
		}
			
		if(-1==(str_num=GetStringsFromStr(token, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			SendTraceMsg("Format error in [%s]: %s", sect, token);
			return FALSE;
		}
		if(str_num!=2)
		{
			SendTraceMsg("Format error in [%s]: %s", sect, token);
			return FALSE;
		}
		
		if(strlen(strbuf[0])>=MAX_DEF_SECTION_NAME_SIZE)
		{
			SendTraceMsg("Task define section name %s too long: max %d", strbuf[0], MAX_DEF_SECTION_NAME_SIZE);
			return FALSE;
		}
		if(strlen(strbuf[1])>=MAX_COMM_TASK_NAME_SIZE)
		{
			SendTraceMsg("Task name %s too long: max %d", strbuf[1], MAX_COMM_TASK_NAME_SIZE);
			return FALSE;
		}
		if(config_info.task_num>=MAX_COMM_TASK_NUM)
		{
			SendTraceMsg("Defined comm task too much: max %d", MAX_COMM_TASK_NUM);
			break;
		}
		strcpy(share_table->dev[config_info.task_num].sect_name, strbuf[0]);
		strcpy(config_info.task[config_info.task_num++], strbuf[1]);
		SendTraceMsg("comm_task: %s	program: %s", strbuf[0], strbuf[1]);
		token = (char*)strtok( NULL, seps );
	}
	share_table->dev_num= config_info.task_num;
	SendTraceMsg("total comm task num: %d", config_info.task_num);
	return TRUE;	
}


pid_t	StartTaskProcess(char *process_name, int task_id)
{
	char	task[256];
	pid_t	process_id;
	
	
	if(err_output_mode!=0)
		sprintf(task, "/root/%s %d -errlog", process_name, task_id);
	else
		sprintf(task, "/root/%s %d", process_name, task_id);
	
	process_id= fork();
	if(process_id==0)  /*child*/
	{
#define MAX_PROC_PARAM_NUM	5
		char	strList[MAX_PROC_PARAM_NUM][MAX_CHAR_NUM_IN_LINE];
		char	*argv[MAX_PROC_PARAM_NUM+1];
		int		str_num;
		
		str_num= GetStringsFromStr(task, MAX_PROC_PARAM_NUM, strList);
		if(str_num==-1 || str_num==0)
			SendTraceMsg("Task string format error: %s!", task);
		else
		{
			int	i;
			for(i=0; i<str_num; i++)
				argv[i]= strList[i];
			argv[i]= 0;
			
			if(execv(argv[0], argv)<0)
				SendTraceMsg("Fail to start task: %s", task);
		}
		exit(0);
	}
	/*father*/
	return process_id;
}

pid_t	run_process[MAX_COMM_TASK_NUM];

static void ExitSystem( int signo )
{
	int		i;
	
	for(i=0; i<config_info.task_num; i++)
	{
		if(run_process[i]<=0)
			continue;
		
		kill(run_process[i], SIGTERM);
		SendTraceMsg("stop task %s", config_info.task[i]);
	}
	exit(0);
}


int main(int argc, char *argv[])
{   
	int		i;
	
	SetModuleName("MAIN");
	SetErrLogMode(argc, argv);
	
	SendTraceMsg("start to run uc7410 main program");

	if(!InitShareCommTable())
	{
		SendTraceMsg("Fail to mount share comm table");
		exit(-1);
	}	
	SendTraceMsg("Success to mount share comm table");
	
	if(!GetCommTasks())
	{
		SendTraceMsg("Fail to read comm define file");
		exit(-1);
	}	
	SendTraceMsg("Success to read comm define file");
	
	if(SIG_ERR==signal(SIGTERM, ExitSystem))
	{
		SendTraceMsg("Fail to signal SIGTERM");
		exit(-1);
	}
	if(SIG_ERR==signal(SIGINT, ExitSystem))
	{
		SendTraceMsg("Fail to signal SIGINT");
		exit(-1);
	}
	if(SIG_ERR==signal(SIGQUIT, ExitSystem))
	{
		SendTraceMsg("Fail to signal SIGQUIT");
		exit(-1);
	}
	


	for(i=0; i<config_info.task_num; i++)
	{
		run_process[i]= StartTaskProcess(config_info.task[i], i);
		if(run_process[i]>0)
			SendTraceMsg("Success to start task: %s", config_info.task[i]);
	}

	for(;;)
	{
		pid_t	ret_pid;

		if(config_info.task_num==0)
		{
			for(;;)
				sleep(1000000);
		}
		ret_pid= wait((int*)NULL);
		if(-1==ret_pid)
		{
			SendTraceMsg("Fail to waitpid");
			continue;
		}
		for(i=0; i<config_info.task_num; i++)
		{
			if(ret_pid==run_process[i])
				break;
		}
		
		sleep(1);
		if(i>=config_info.task_num)
			continue;
		
		run_process[i]= (pid_t)-1;
		SendTraceMsg("Task %s exit", config_info.task[i]);
		
		run_process[i]= StartTaskProcess(config_info.task[i], i);
		if(run_process[i]>0)
			SendTraceMsg("Success to restart task: %s", config_info.task[i]);
	}
	
}
























