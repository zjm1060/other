/************************************************************************************************************************************************
FILENAME:	ProcessSysRtu.c
FUNCTION:	
		functions of  processing ana data and sysmsg of SYS RTU to form allsend packet:PollSysRtu()
		
		
VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:	2006.04.20		ycx
************************************************************************************************************************************************/



#include "../../../dms/v4.0a/inc/dms_com.h"

#include "../../../dps/v4.0a/inc/dps_rec_def.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../dps/v4.0a/inc/ipc_common.h"

#include "../../../lan/v4.0a/inc/lan_common.h"
#include "../../../plc/v4.0a/inc/lcu_data.h"

#include "../../../dpp/v4.0a/inc/dppdef.h"
#include "../../../dpp/v4.0a/inc/dpp_functions.h"



/*This function is for SYSRTU polling */

int PollSysRtu(void) 	/* 0 is OK,-1 is fault */
{
	unsigned short  j;
	LCUID           lcuno;
	unsigned int 	point_num;
	IND_STRUCT 		*msg_db;
	IND_DATA		*msg_all;
	ANA_STRUCT		*ana_db;
	AIALLSTRUCT		*ai_all;
	DATE_TIME		tm;

   	if(-1==GetLocalHostStnId(&lcuno.stnid))
		return -1;

	if(-1==GetSysLcuId(lcuno.stnid, &lcuno.devid))
		return -1;

	GetSysTime(&tm);
        
	if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, ANA_TYPE, &point_num) && point_num>0) 
	{
		if(NULL==(ana_db=malloc(point_num*sizeof(ANA_STRUCT))))
			return -1;
		if(NULL==(ai_all=malloc(point_num*sizeof(AIALLSTRUCT))))
		{
			free(ana_db);
			return -1;
		}

		ReadDb_ANA(lcuno, ANA_TYPE, 0, point_num, ana_db);

		for(j=0;j<point_num;j++)
		{
			ai_all[j].data = ana_db[j].data;
			ai_all[j].status = ana_db[j].status;
		}
	
		AllSend_ANA(lcuno, DPS_ANA_ALL, ANA_TYPE, 0, point_num, ai_all, tm);

		free(ana_db);
		free(ai_all);
	}	

	if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, PARAM_TYPE, &point_num) && point_num>0) 
	{
		if(NULL==(ana_db=malloc(point_num*sizeof(ANA_STRUCT))))
			return -1;
		if(NULL==(ai_all=malloc(point_num*sizeof(AIALLSTRUCT))))
		{
			free(ana_db);
			return -1;
		}

		ReadDb_ANA(lcuno, PARAM_TYPE, 0, point_num, ana_db);

		for(j=0;j<point_num;j++)
		{
			ai_all[j].data = ana_db[j].data;
			ai_all[j].status = ana_db[j].status;
		}
	
		AllSend_ANA(lcuno, DPS_ANA_ALL, PARAM_TYPE, 0, point_num, ai_all, tm);

		free(ana_db);
		free(ai_all);
	}	

	if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, SYSMSG_TYPE, &point_num) && point_num>0) 
	{
		if(NULL==(msg_db=malloc(point_num*sizeof(IND_STRUCT))))
			return -1;
		if(NULL==(msg_all=malloc(point_num*sizeof(IND_DATA))))
		{
			free(msg_db);
			return -1;
		}

		ReadDb_IND(lcuno, SYSMSG_TYPE, 0, point_num, msg_db);

		for(j=0;j<point_num;j++)
			msg_all[j]= msg_db[j].status;
	
		AllSend_IND(lcuno, SYSMSG_TYPE, 0, point_num, msg_all, tm);

		free(msg_db);
		free(msg_all);
	}	

	return 0;		
	
}


