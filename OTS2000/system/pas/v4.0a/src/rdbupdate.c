

#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include	"../../../plc/v4.0a/inc/lcu_data.h"
#include	"../../../dpp/v4.0a/inc/dppdef.h"
#include	"../../../dpp/v4.0a/inc/dpp_functions.h"
#include	"../../../pas/v4.0a/inc/pdc_def.h"


void* RdbUpdateCheck(PDC_TASK *pTask) 	
{
	UCHAR		stn_id, dev_id, data_type;
	GULONG		dev_num, rec_num, entry_num;
	USHORT		i, j;
	POINTER		point;
	time_t		tt_now;
	struct tm	tt_pt;
	DATE_TIME	update_time;
	WR_VAR_UNION new_value= {0};
	IOST		iost;

   	if(-1==GetLocalHostStnId(&stn_id))
		return ((void*)-1);
	if(-1==GetGroupNumById(stn_id, &dev_num))
		return ((void*)-1);

	iost.noUpdate= 1;
	point.stn_id= stn_id;
	dev_id= 1;
	for(;;)
	{
		sleep(pTask->check_period);

		printf("\n------CHECK_RDB_UPDATE------%d\n", time(NULL));

		while(-1==GetRecordNumById(stn_id, dev_id, &rec_num) || rec_num==0)
		{
			dev_id++;
			if(dev_id>dev_num)
				dev_id= 1;
		}

		tt_now= time(NULL);
		point.dev_id= dev_id;
 		for(data_type=1,j=0; j<rec_num; data_type++) 
  		{  		
			if(data_type>=MAX_DATATYPE_NUM)
				break;
			if(-1==GetEntryNumById(stn_id, dev_id, data_type, &entry_num))
				continue;
			j++;

			point.data_type= data_type;
			for(i=0; i<entry_num; i++)
			{
				point.pt_id= i;
				GetPtUpdateTimeById(&point, &update_time);
				tt_pt.tm_year= update_time.year-1900;
				tt_pt.tm_mon= update_time.month-1;
				tt_pt.tm_mday= update_time.day;
				tt_pt.tm_hour= update_time.hour;
				tt_pt.tm_min= update_time.minute;
				tt_pt.tm_sec= update_time.second;
				if((tt_now-mktime(&tt_pt))<=pTask->task_period)
					continue;
				SetPtValueIostById(&point, new_value, iost, update_time, RW_UPDATED);
			} /*for(entry)*/
		} /*for(data_type)*/
		dev_id++;
		if(dev_id>dev_num)
			dev_id= 1;
	} /*for(;;)*/	
}


