/************************************************************************************************************************
FILENAME:	logwrite.c

FUNCTION:	
	
	source code of  logwrite module,include:
	(1) read data from log shared memory continually 
	(2) write every data record to log file

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	start to update the file from H9000 V3.0 to V4.0		yyp

*************************************************************************************************************************/

#include "../../../dms/v4.0a/inc/syslog.h"

#pragma	comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma	comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma	comment(lib, "wsock32.lib")

#define MAX_SHOW_LOG_NUM	30

int setting_mode;
MODULE_INFO		module_view[MAX_H9000_MODULE_NUM];

#ifdef WINDOWS_OS
int SignalRoutine ( DWORD signal )
{
	switch ( signal )
	{
	case CTRL_C_EVENT:
		setting_mode= 1;
		return TRUE;

	default:
		return FALSE;
	}
}

#else //UNIX
#include <signal.h>
void SignalRoutine( int signal )
{
	setting_mode= 1;
}	
#endif

int GetInput(char *prompt, char input[][256], int max_num)
{
	char buff[256];
	int num;

input1:
	printf("%s", prompt);
	if(!gets(buff))
		goto input1;

	num= GetStringsFromStr(buff, 5, input);
	if(num<1)
		goto input1;
	return num;
}

int get_module_id(char *moduleStr)
{
	int id;

	if(moduleStr[0]>='0' && moduleStr[0]<='9')
	{
		if(1==sscanf(moduleStr, "%d", &id))
			return id;
		else
			return -1;
	}

	if(!strcmp(moduleStr, "ALL"))
		return 0;

	for(id=1; id<MAX_H9000_MODULE_NUM; id++)
	{
		char name[256];

		GetModuleName(id, name);
		if(!strcmp(name, moduleStr))
			return id;
	}
	return -1;
}

void log_module_flag_list(UCHAR module_id)
{
	printf("error: %s\n", GetLogOutputFilter(module_id, H9000_LOG_ERROR)? "on":"off");
	printf("warn: %s\n", GetLogOutputFilter(module_id, H9000_LOG_WARN)? "on":"off");
	printf("info: %s\n", GetLogOutputFilter(module_id, H9000_LOG_INFO)? "on":"off");
	printf("recv: %s\n", GetLogOutputFilter(module_id, H9000_LOG_RECV)? "on":"off");
	printf("send: %s\n", GetLogOutputFilter(module_id, H9000_LOG_SEND)? "on":"off");
	printf("file: %s\n", GetLogOutputFileFlag(module_id)? "on":"off");
}

void view_module_flag_list(UCHAR module_id)
{
	printf("error: %s\n", (module_view[module_id].filter & H9000_LOG_ERROR)? "on":"off");
	printf("warn: %s\n", (module_view[module_id].filter & H9000_LOG_WARN)? "on":"off");
	printf("info: %s\n", (module_view[module_id].filter & H9000_LOG_INFO)? "on":"off");
	printf("recv: %s\n", (module_view[module_id].filter & H9000_LOG_RECV)? "on":"off");
	printf("send: %s\n", (module_view[module_id].filter & H9000_LOG_SEND)? "on":"off");
}


void log_module_flag_setting(UCHAR module_id, char *flag, char *state)
{
	int set;
	enum H9000_LOG_TYPE log_type;

	if(!strcmp(state, "on"))
		set= 1;
	else if(!strcmp(state, "off"))
		set= 0;
	else
	{
		printf("Usage: set flag/all on/off\n");
		return;
	}

	if(!strcmp(flag, "all"))
		log_type= H9000_LOG_ERROR|H9000_LOG_WARN|H9000_LOG_INFO|H9000_LOG_RECV|H9000_LOG_SEND;
	else if(!strcmp(flag, "error"))
		log_type= H9000_LOG_ERROR;
	else if(!strcmp(flag, "warn"))
		log_type= H9000_LOG_WARN;
	else if(!strcmp(flag, "info"))
		log_type= H9000_LOG_INFO;
	else if(!strcmp(flag, "recv"))
		log_type= H9000_LOG_RECV;
	else if(!strcmp(flag, "send"))
		log_type= H9000_LOG_SEND;
	else if(!strcmp(flag, "file"))
	{
		SetLogOutputFileFlag(module_id, set);
		return;
	}
	else
	{
		printf("Usage: set flag/all on/off\n");
		return;
	}

	SetLogOutputFilter(module_id, log_type, set);
}

void view_module_flag_setting(UCHAR module_id, char *flag, char *state)
{
	int set;
	enum H9000_LOG_TYPE log_type;

	if(!strcmp(state, "on"))
		set= 1;
	else if(!strcmp(state, "off"))
		set= 0;
	else
	{
		printf("Usage: set flag/all on/off\n");
		return;
	}

	if(!strcmp(flag, "all"))
		log_type= H9000_LOG_ERROR|H9000_LOG_WARN|H9000_LOG_INFO|H9000_LOG_RECV|H9000_LOG_SEND;
	else if(!strcmp(flag, "error"))
		log_type= H9000_LOG_ERROR;
	else if(!strcmp(flag, "warn"))
		log_type= H9000_LOG_WARN;
	else if(!strcmp(flag, "info"))
		log_type= H9000_LOG_INFO;
	else if(!strcmp(flag, "recv"))
		log_type= H9000_LOG_RECV;
	else if(!strcmp(flag, "send"))
		log_type= H9000_LOG_SEND;
	else
	{
		printf("Usage: set flag/all on/off\n");
		return;
	}

	if(set)
		module_view[module_id].filter |= log_type;
	else
		module_view[module_id].filter &= ~log_type;

	if(module_id==0)
	{
		int id;
		for(id=1; id<MAX_H9000_MODULE_NUM; id++)
		{
			if(set)
				module_view[id].filter |= log_type;
			else
				module_view[id].filter &= ~log_type;
		}
	}
}

void log_setting_module(UCHAR module_id)
{
	int num;
	char input[5][256];
	char prompt[256], module_name[256];

	GetModuleName((UCHAR)module_id, module_name);
	sprintf(prompt, "log_setting:/%s>", module_name);

input1:
	num= GetInput(prompt, input, 5);
	if(num<1)
		goto input1;

	if(!strcmp(input[0], "ls"))
	{
		log_module_flag_list(module_id);
	}
	else if(!strcmp(input[0], "set"))
	{
		if(num!=3)
		{
			printf("Usage: set flag/all on/off\n");
			goto input1;
		}
		log_module_flag_setting(module_id, input[1], input[2]);
	}
	else if(!strcmp(input[0], "back"))
	{
		return;
	}
	else if(!strcmp(input[0], "quit") || !strcmp(input[0], "exit"))
	{
		exit(0);
	}
	else if(!strcmp(input[0], "help"))
	{
		printf("ls: list module log flags\n");
		printf("set: set module log flag\n");
		printf("back: return\n");
	}
	else
		printf("Unknown command \"%s\".\n", input[0]);
		
	goto input1;	
}

void view_setting_module(UCHAR module_id)
{
	int num;
	char input[5][256];
	char prompt[256], module_name[256];

	GetModuleName((UCHAR)module_id, module_name);
	sprintf(prompt, "view_setting:/%s>", module_name);

input1:
	num= GetInput(prompt, input, 5);
	if(num<1)
		goto input1;

	if(!strcmp(input[0], "ls"))
	{
		view_module_flag_list(module_id);
	}
	else if(!strcmp(input[0], "set"))
	{
		if(num!=3)
		{
			printf("Usage: set flag/all on/off\n");
			goto input1;
		}
		view_module_flag_setting(module_id, input[1], input[2]);
	}
	else if(!strcmp(input[0], "back"))
	{
		return;
	}
	else if(!strcmp(input[0], "quit") || !strcmp(input[0], "exit"))
	{
		exit(0);
	}
	else if(!strcmp(input[0], "help"))
	{
		printf("ls: list module view flags\n");
		printf("set: set module view flag\n");
		printf("back: return\n");
	}
	else
		printf("Unknown command \"%s\".\n", input[0]);
		
	goto input1;	
}

void log_modules_list(void)
{
	int id;

	printf("0: all\n");
	for(id=1; id<MAX_H9000_MODULE_NUM; id++)
	{
		char name[256];

		name[0]= 0;
		GetModuleName(id, name);
		if(name[0])
			printf("%d: %s\n", id, name);
	}
}
void view_modules_list(void)
{
	int id;

	printf("0: all\n");
	for(id=1; id<MAX_H9000_MODULE_NUM; id++)
	{
		char name[256];

		name[0]= 0;
		GetModuleName(id, name);
		if(name[0])
			printf("%d: %s\n", id, name);
	}
}

void log_setting(void)
{
	int num, module_id;
	char input[5][256];
	char prompt[256];

	strcpy(prompt, "log_setting:>");

input1:
	num= GetInput(prompt, input, 5);
	if(num<1)
	{
		printf("input error, \"help\" for help\n");
		goto input1;
	}

	if(!strcmp(input[0], "cd"))
	{
		if(num<2)
		{
			printf("Usage: cd module_name/id\n");
			goto input1;
		}

		module_id=  get_module_id(input[1]);
		if(-1==module_id)
		{
			printf("\"%s\" doesn't match any modules\n", input[1]);
			goto input1;
		}

		log_setting_module((UCHAR)module_id);
	}

	else if(!strcmp(input[0], "ls"))
	{
		log_modules_list();
	}
	else if(!strcmp(input[0], "back"))
	{
		return;
	}
	else if(!strcmp(input[0], "quit") || !strcmp(input[0], "exit"))
	{
		exit(0);
	}
	else if(!strcmp(input[0], "help"))
	{
		printf("cd: enter module\n");
		printf("ls: list modules\n");
		printf("back: return\n");
	}
	else
		printf("Unknown command \"%s\".\n", input[0]);
	goto input1;
}

void view_setting(void)
{
	int num, module_id;
	char input[5][256];
	char prompt[256];

	strcpy(prompt, "view_setting:>");

input1:
	num= GetInput(prompt, input, 5);
	if(num<1)
	{
		printf("input error, \"help\" for help\n");
		goto input1;
	}

	if(!strcmp(input[0], "cd"))
	{
		if(num<2)
		{
			printf("Usage: cd module_name/id\n");
			goto input1;
		}

		module_id=  get_module_id(input[1]);
		if(-1==module_id)
		{
			printf("\"%s\" doesn't match any modules\n", input[1]);
			goto input1;
		}

		view_setting_module((UCHAR)module_id);
	}

	else if(!strcmp(input[0], "ls"))
	{
		view_modules_list();
	}
	else if(!strcmp(input[0], "back"))
	{
		return;
	}
	else if(!strcmp(input[0], "quit") || !strcmp(input[0], "exit"))
	{
		exit(0);
	}
	else if(!strcmp(input[0], "help"))
	{
		printf("cd: enter module\n");
		printf("ls: list modules\n");
		printf("back: return\n");
	}
	else
		printf("Unknown command \"%s\".\n", input[0]);
	goto input1;
}



int input_handler(void)
{
	char	input[256];

	printf("\n\nenter interactive mode");

	while(1)
	{
		printf("\n1) log setting");
		printf("\n2) view setting");
		printf("\n3) return to the oldest");
		printf("\n4) return to the latest");
		printf("\n5) quit");
		printf("\nplease input: ");
		gets(input);
		
		if(input[0]== '1')
			log_setting();
		else if(input[0]== '2')
			view_setting();
		else if(input[0]== '3' || input[0]== '4')
			return input[0];
		else if(input[0]== '5')
			exit(0);
	}
}

void initModuleView(void)
{
	int id;

	for(id=0; id<MAX_H9000_MODULE_NUM; id++) //from 0
		module_view[id].filter= -1;
}

void outputLogInfo(LOG_INFO *log_info, int pos)
{
	if(log_info->module_id<1 || log_info->module_id >=MAX_H9000_MODULE_NUM)
	{
		printf("\nmodule id %d error: range 1~%d", log_info->module_id, MAX_H9000_MODULE_NUM-1);
		return;
	}
	if(module_view[log_info->module_id].filter & log_info->logType)
	{
		char moduleName[MAX_H9000_MODULE_NAME_SIZE];

		GetModuleName(log_info->module_id, moduleName);
		printf("\n%05d %04d-%02d-%02d %02d:%02d:%02d:%03d %s: %s", pos+1, log_info->datetime.year,
			log_info->datetime.month, log_info->datetime.day, log_info->datetime.hour,
			log_info->datetime.minute, log_info->datetime.second, log_info->datetime.ms,
			moduleName, log_info->info);
	}
}
int main(int argc, char *argv[])
{   
	int i, header, count;
	LOG_INFO	log_info;

#ifdef WINDOWS_OS
	COORD console_size= {256,10000};
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), console_size);
	if ( SetConsoleCtrlHandler ((void *)SignalRoutine,TRUE ) == 0 )
	{
		Err_log("Fail to SetConsoleCtrlHandler!\n");
		exit(-1);
	}
#else //UNIX
	signal(SIGKILL,SignalRoutine); 
	signal(SIGTERM,SignalRoutine);
	signal(SIGINT,SignalRoutine);
#endif

	if(-1==InitSysLog(H9000_MODULE_LOGMGR, "LogMgr"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}

	initModuleView();
	
	GetSysLogHeader(&header, &count);
	if(count>MAX_SHOW_LOG_NUM)
		count= MAX_SHOW_LOG_NUM;

	for(i=0; i<count; i++)
	{
		if(-1==GetSysLogInfo(header-count+i, &log_info))
			continue;
		outputLogInfo(&log_info, header-count+i);
	}

	for(;;)
	{
		int new_header;

		Msleep(50);

		GetSysLogHeader(&new_header, &count);
		while(header!=new_header)
		{
			GetSysLogInfo(header, &log_info);
			outputLogInfo(&log_info, header);

			header= (header+1)%MAX_LOG_NUM;
			
			if(setting_mode)
			{
#ifndef WINDOWS_OS
				signal(SIGKILL,SignalRoutine); 
				signal(SIGTERM,SignalRoutine);
				signal(SIGINT,SignalRoutine);
#endif

				if('4'==input_handler())
				{
					GetSysLogHeader(&new_header, &count);
					header=new_header;
				}
				setting_mode= 0;
			}
			
		}

		if(setting_mode)
		{
#ifndef WINDOWS_OS
			signal(SIGKILL,SignalRoutine); 
			signal(SIGTERM,SignalRoutine);
			signal(SIGINT,SignalRoutine);
#endif

			if('4'==input_handler())
			{
				GetSysLogHeader(&new_header, &count);
				header=new_header;
			}
			setting_mode= 0;
		}

	}
}
