
#include	"../../../dms/v4.0a/inc/dms_com.h"
#include    "../../../pas/v4.0a/inc/pdc_def.h"


/*
#define DEBUG
*/
void *safe_day(PDC_TASK *pTask)
{   
	int				run_flag;
	unsigned int    day_num;
	DMS_COMMON		common_ah;
	DATE_TIME       write_time;
	char			timestr[64];
	char			SAFE_DAY_NAME[128]; 
	char			err_str[256];
        
    GetEmsString("SAFE_DAY_NAME", (char *)SAFE_DAY_NAME);
       
	for(;;)
	{
		sleep(pTask->check_period);
		if((run_flag = PdcCheck(pTask))!=0) 
			continue;            
            
		GetSysTimeStr(timestr);
		printf("\n------SAFE_DAY------%s\n", timestr);
	  
		if( -1 == ReadEntryByNameStr (SAFE_DAY_NAME, &common_ah))
		{
			sprintf (err_str,"Safe_day;EC:001 %s No this point in the database file !!!",SAFE_DAY_NAME);		
			Err_log(err_str);
			continue;
		}

		day_num = 1 + (int)(common_ah.var.anlg.fValue + 0.1);
		printf ("\n day_num = %d\n",day_num); 
		GetSysTime( &write_time );
		if( -1 == SetAnaPtfValueByNameStr( SAFE_DAY_NAME,(float)day_num,write_time))
		{
			sprintf (err_str,"Safe_day;EC:002 %s write fail !!!",SAFE_DAY_NAME);		
			Err_log(err_str);
			continue;
		}		
	}
}
