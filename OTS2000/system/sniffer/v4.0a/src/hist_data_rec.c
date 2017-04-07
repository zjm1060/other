#include	<time.h>
#include	<stdlib.h>
#include	<stdio.h>


#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"
#include	"../../../dps/v4.0a/inc/dps_functions_def.h"



#ifndef  WINDOWS_OS
#define	Sleep(x)	usleep(1000*x)
#endif



int		GetDataConf(void);



#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "../../../dps/v4.0a/lib/dpslib.lib")
#pragma comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma comment(lib, "../../../lan/v4.0a/lib/lan.lib")


#define CONFIG_DEFINE_FILE	"/home/ems/h9000/def/hist_data_rec.def"

#define MAX_HIST_DATA_REC_NUM	10
char data_rec_ptname_list[MAX_HIST_DATA_REC_NUM][POINT_NAME_SIZE];
int	data_num;



void WriteDataToTxtFile(int mon, int day, int hour, int isHourStart, char *ptname)
{
	FILE	*fp;
	float	value;
	char	filename[256];
	
	sprintf(filename, "%s%s_%02d%02d.txt", ERROR_LOG_PATH, ptname, mon, day);
	fp= fopen(filename, "a");
	if(fp)
	{
		if(isHourStart)
			fprintf(fp, "\nhour%02d: ", hour);
		if(-1!=GetAnaPtfValueByNameStr(ptname, &value))
			fprintf(fp, "%.2f;", value);
		else
			fprintf(fp, "xxxx;");
		fclose(fp);
	}
}



int	main(int argc, char *argv[])
{
	int		i;
	time_t	tt;
	struct tm	time_tm;

	SetModuleName("HIST_DATA_REC");
	SetErrLogMode(argc, argv);
	CreateLockFile(".hist_data_rec.lock");

	if(-1==MountDB(NULL))
	{
		Err_log("Fail to MountDB!\n");
		exit(-1);
	}
	
	if(-1==GetDataConf())
	{
		Err_log("Fail to init hist data config!\n");
		exit(-1);
	}
	else
		Err_log("Success to init hist data config!\n");

	Err_log("Start to hist data rec process\n");

	for(;;)
	{
		int isHourStart;

		tt= time(NULL);
		time_tm= *localtime(&tt);

		if(time_tm.tm_min==0 && time_tm.tm_sec==0)
			isHourStart= 1;
		else
			isHourStart= 0;

		for(i=0; i<data_num; i++)
			WriteDataToTxtFile(time_tm.tm_mon+1, time_tm.tm_mday, 
				time_tm.tm_hour, isHourStart, data_rec_ptname_list[i]);

		Sleep(1000);

	} //for(;;)
}



int GetDataConf(void)
{
	char	*filename=CONFIG_DEFINE_FILE;
	char	string[256], ptname[256];
	POINTER	pt;
	FILE	*fp;

	data_num=0;
	fp= fopen(filename, "r");
	if(!fp)
	{
		SendTraceMsg(0,0,"Fail to open %s", filename);
		return -1;
	}

	while(fgets(string, sizeof(string), fp))
	{
		if(1!=sscanf(string, "%s", ptname))
			continue;

		if(-1==GetPtIdByNameStr(ptname, &pt))
		{
			SendTraceMsg(0,0,"Error: pt %s is not exist", ptname);
			fclose(fp);
			return -1;
		}

		if(data_num>=MAX_HIST_DATA_REC_NUM)
		{
			SendTraceMsg(0,0,"max hist data rec pt num: %d", MAX_HIST_DATA_REC_NUM);
			break;
		}

		strcpy(data_rec_ptname_list[data_num], ptname);
		data_num++;
		
	}
	fclose(fp);
	return 0;
}
