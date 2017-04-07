#include "/users/ems/system/dms/v4.0a/inc/dms_com.h"

#include "/users/ems/system/dps/v4.0a/inc/dps_rec_def.h"
#include "/users/ems/system/dps/v4.0a/inc/dps_type_def.h"
#include "/users/ems/system/dps/v4.0a/inc/ipc_common.h"

#include "/users/ems/system/oix/v4.0a/inc/priority_def.h" 


#include "/users/ems/system/lan/v4.0a/inc/lan_common.h"

#include "/users/ems/system/dpp/v4.0a/inc/dppdef.h"
#include "/users/ems/system/dpp/v4.0a/inc/lcudef.h"
#include "/users/ems/system/cmd/v4.0a/inc/cmddef.h"
#include "/users/ems/system/dpp/v4.0a/inc/dpp_functions.h"

#include "/users/ems/system/plc/v4.0a/inc/lcu_data.h"



/**********************read plc link status *****************/

/*int ReadLcuLinkStat(LCUID_INC_IDX lcuno)
{
	DMS_COMMON	common_aa;
	POINT_NAME	pt_name;
	char		ptNameStr[POINT_NAME_SIZE];


	dpp[lcuno.stn_idx][lcuno.dev_idx].isdataflag =0;
	
	if(GetStnNameById(lcuno.stnid,pt_name.stnName) == -1)
	{
		printf ( "\n dpp.dxe DBread: No %d station in the database!!!", lcuno.stnid);

		return -1;
	}

	if(GetGroupNameById(lcuno.stnid,lcuno.devid,pt_name.grpName) == -1)
	{
		printf ( "\n dpp.dxe DBread: No %d lcu of %d station in the database!!!", lcuno.devid, lcuno.stnid);

		return -1;
	}

	if(GetRecNameByType(SYSMSG_TYPE,pt_name.recName) ==-1)
	{
		printf ( "\n dpp.dxe DBread: Type Error in the database!!!");

		return -1;
	}

	strcpy(pt_name.entryName,"LCU_LINK_STAT");

	if(PointNametoStr(ptNameStr,&pt_name) ==-1)
	{
		printf ( "\n dpp.dxe DBread: PointName Error in the database!!!");

		return -1;
	}
	
	if ( ReadEntryByNameStr (ptNameStr ,&common_aa) == -1)
	{
		printf ( "\nDBread: No link state point of %d lcu %d station in the database file !", lcuno.devid, lcuno.stnid );

		return -1;
	}		

	if((shm_plc_data->seg[lcuno.stnid][lcuno.devid].endpp ==1) && (common_aa.var.ind.status.state==1))
		dpp[lcuno.stnid][lcuno.devid].isdataflag =1;			 

	if(	dpp[lcuno.stnid][lcuno.devid].isdataflag ==0)	return 0;
}
*/
	
		
int ReadPlcDb(LCUID lcuno)
{
	
	/**read plc ai parameters from database**/

	 ReadDb_ANA(lcuno,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.ai_off,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.ai_size,
						ANA_TYPE,
						dpp[lcuno.stnid][lcuno.devid].ai_db);
	
	/**read plc rtd parameters from database**/

	ReadDb_ANA(lcuno,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.rtd_off,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.rtd_size,
						ANA_TYPE,
						dpp[lcuno.stnid][lcuno.devid].ai_db);

	/**	read plc ao parameters from database**/

	ReadDb_ANA(lcuno,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.ao_off,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.ao_size,
						OUTA_TYPE,
						dpp[lcuno.stnid][lcuno.devid].ao_db);
	

	/**read plc soe parameters from database**/
	ReadDb_IND(lcuno,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.soe_off,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.soe_size,
						SOE_TYPE,
						dpp[lcuno.stnid][lcuno.devid].dii_db);

	 	 
	/**read plc pol parameters from database**/
	ReadDb_IND(lcuno,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.pol_off,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.pol_size,
						POL_TYPE,
						dpp[lcuno.stnid][lcuno.devid].din_db);
		 
	/**read plc message parameters from database**/
	ReadDb_IND(lcuno,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.msg_off,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.msg_size,
						SYSMSG_TYPE,
						dpp[lcuno.stnid][lcuno.devid].msg_db);

	
	/**	read dout parameters from database**/
	ReadDb_IND(lcuno,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.dout_off,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.dout_size,
						OUTS_TYPE,
						dpp[lcuno.stnid][lcuno.devid].dout_db);

	/**	read impulse parameters from database**/
	
	ReadDb_IMP(lcuno,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.imp_off,
						shm_plc_data->seg[lcuno.stnid][lcuno.devid].head.imp_size,
						IMP_TYPE,
						dpp[lcuno.stnid][lcuno.devid].pi_db);

	return 1;

}

