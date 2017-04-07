

#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include	"../../../plc/v4.0a/inc/lcu_data.h"
#include	"../../../dpp/v4.0a/inc/dppdef.h"
#include	"../../../dpp/v4.0a/inc/dpp_functions.h"
#include	"../../../pas/v4.0a/inc/pdc_def.h"


void* SysRtuAllSend(PDC_TASK *pTask) 	
{
	unsigned short  j;
	LCUID           lcuno;
	unsigned int 	point_num;
	IND_STRUCT 		*msg_db;
	IND_DATA		*msg_all;
	ANA_STRUCT		*ana_db;
	AIALLSTRUCT		*ai_all;
	DATE_TIME		tm;
	BOOL			app_priv;
	USHORT			host_state;

	if(GetLocalHostPrvg("APP_SERVER", &app_priv) != 0)
	{
		Err_log("Fail to find APP_SERVER define list");
		return ((void*)-1);
	}
	if(!app_priv)
	{
		for(;;)
			sleep(10000);
	}

   	if(-1==GetLocalHostStnId(&lcuno.stnid))
		return ((void*)-1);

	if(-1==GetSysLcuId(lcuno.stnid, &lcuno.devid))
		return ((void*)-1);

	for(;;)
	{
		sleep(pTask->check_period);

		if(PdcCheck(pTask))
			continue;

		if(-1==GetLocalHostState(&host_state))
			continue;
		if(host_state!=1) /*Ö÷¿Ø*/
			continue;

		printf("\n------SYS_RTU_ALL_SEND------%d\n", time(NULL));

		GetSysTime(&tm);
        
		if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, ANA_TYPE, &point_num) && point_num>0) 
		{
			if(NULL==(ana_db=malloc(point_num*sizeof(ANA_STRUCT))))
				return ((void*)-1);
			if(NULL==(ai_all=malloc(point_num*sizeof(AIALLSTRUCT))))
			{
				free(ana_db);
				return ((void*)-1);
			}

			ReadDb_ANA(lcuno, ANA_TYPE, 0, (USHORT)point_num, ana_db);

			for(j=0;j<point_num;j++)
			{
				ai_all[j].data = ana_db[j].data;
				ai_all[j].status = ana_db[j].status;
			}
		
			AllSend_ANA(lcuno, DPS_ANA_ALL, ANA_TYPE, 0, (USHORT)point_num, ai_all, tm);

			free(ana_db);
			free(ai_all);
		}	

		if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, PARAM_TYPE, &point_num) && point_num>0) 
		{
			if(NULL==(ana_db=malloc(point_num*sizeof(ANA_STRUCT))))
				return ((void*)-1);
			if(NULL==(ai_all=malloc(point_num*sizeof(AIALLSTRUCT))))
			{
				free(ana_db);
				return ((void*)-1);
			}

			ReadDb_ANA(lcuno, PARAM_TYPE, 0, (USHORT)point_num, ana_db);

			for(j=0;j<point_num;j++)
			{
				ai_all[j].data = ana_db[j].data;
				ai_all[j].status = ana_db[j].status;
			}
		
			AllSend_ANA(lcuno, DPS_ANA_ALL, PARAM_TYPE, 0, (USHORT)point_num, ai_all, tm);

			free(ana_db);
			free(ai_all);
		}	

		if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, POL_TYPE, &point_num) && point_num>0) 
		{
			if(NULL==(msg_db=malloc(point_num*sizeof(IND_STRUCT))))
				return ((void*)-1);
			if(NULL==(msg_all=malloc(point_num*sizeof(IND_DATA))))
			{
				free(msg_db);
				return ((void*)-1);
			}

			ReadDb_IND(lcuno, POL_TYPE, 0, (USHORT)point_num, msg_db);

			for(j=0;j<point_num;j++)
				msg_all[j]= msg_db[j].status;
		
			AllSend_IND(lcuno, POL_TYPE, 0, (USHORT)point_num, msg_all, tm);

			free(msg_db);
			free(msg_all);
		}

		if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, SYSMSG_TYPE, &point_num) && point_num>0) 
		{
			if(NULL==(msg_db=malloc(point_num*sizeof(IND_STRUCT))))
				return ((void*)-1);
			if(NULL==(msg_all=malloc(point_num*sizeof(IND_DATA))))
			{
				free(msg_db);
				return ((void*)-1);
			}
			
			ReadDb_IND(lcuno, SYSMSG_TYPE, 0, (USHORT)point_num, msg_db);
			
			for(j=0;j<point_num;j++)
				msg_all[j]= msg_db[j].status;
			
			AllSend_IND(lcuno, SYSMSG_TYPE, 0, (USHORT)point_num, msg_all, tm);
			
			free(msg_db);
			free(msg_all);
		}
	} /*for()*/	
}


