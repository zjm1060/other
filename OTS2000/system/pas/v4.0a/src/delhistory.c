#ifdef   WINDOWS_OS
#include   <stdio.h>   
#include   <io.h>
#endif   

#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../pas/v4.0a/inc/pdc_def.h"

void delHisFiles(char *path, int days)
{
#ifdef   WINDOWS_OS
	struct	_finddata_t	filedata;   
	int		first, done;
	time_t	curr_time= time(NULL);	
	char	filename[256];

	sprintf(filename, "%s/*.*", path);
	if((first= _findfirst(filename, &filedata))==-1)
		return;
	while(!(done= _findnext(first, &filedata)))     
	{   
		if(!(_A_SUBDIR & filedata.attrib))
		{
			if((curr_time-filedata.time_write) > 60*60*24*days)
			{
				sprintf(filename, "%s/%s", path, filedata.name);
				remove(filename);
			}
		}
	}
	_findclose(first);

#else /*UNIX_OS*/
  	int		i;
	char 	cmdbuf[256];

	sprintf(cmdbuf, "find %s -ctime +%d -exec rm -f {} \\;", path, days);
	system(cmdbuf);
#endif
}


void *delhistory(PDC_TASK *pTask)
{
	int         run_flag;
	float		bef_year;
	int			bef_days;
	char		timestr[64];
	
		
	if(-1==GetEmsVariable("HYEAR", &bef_year))
		bef_year= 1;

	if(bef_year<1)
		bef_year= 1;
	bef_days= (int)(bef_year * 365);
        
	for(;;)
	{        
		sleep(pTask->check_period);
		if((run_flag = PdcCheck(pTask))!=0) 
			continue;
  
		GetSysTimeStr(timestr);
		printf("\n------CHECK_HIST_FILE------%s\n", timestr);
     

		delHisFiles("/home/ems/h9000/his/event", bef_days);
		delHisFiles("/home/ems/h9000/his/eventtxt", bef_days);
		delHisFiles("/home/ems/h9000/his/rpt", bef_days);
		delHisFiles("/home/ems/h9000/his/sftcp", bef_days);
		delHisFiles("/home/ems/h9000/his/sgzy", bef_days);
		delHisFiles("/home/ems/h9000/his/agc", bef_days);
		delHisFiles("/home/ems/h9000/his/curve", bef_days);
		delHisFiles("/home/ems/h9000/his/trap", 7);
	}     	
}
