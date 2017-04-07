/************************************************************************************************************************************************
FILENAME:	CommWriteShare.c
FUNCTION:	
		functions of write new data to share memory of plc_data:CommWriteANA(),CommWriteIMP(),CommWritePOL()
		
		
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



void CommWriteANA(LCUID lcuno,unsigned short port_no,unsigned short firstrun,unsigned short offset,unsigned short length,unsigned char * ai_buf,unsigned char * ai_qlt)
{
	shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].firstscan = firstrun;
	
	shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].ai_size = length;
	
	shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].ai_off  = offset;
	
	memcpy(shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].ai_data,ai_buf,2*length);
	
	memcpy(shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].ai_qlt,ai_qlt,length/8+1);
	
	memset(shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].ai_en,0,length/8+1);
	
}

  

void CommWriteIMP(LCUID lcuno,unsigned short port_no,unsigned short firstrun,unsigned short offset,unsigned short length,unsigned char * pi_buf,unsigned char * pi_qlt)
{

	shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].firstscan = firstrun;
	
	shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].imp_size = length;
	
	shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].imp_off  = offset;
	
	memcpy(shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].pi_data,pi_buf,4*length);
	
	memcpy(shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].pi_qlt,pi_qlt,length/8+1);
	
	memset(shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].pi_en,0,length/8+1);

}


void CommWritePOL(LCUID lcuno,unsigned short port_no,unsigned short firstrun,unsigned short offset,unsigned short length,unsigned char * pol_buf,unsigned char * pol_qlt)
{

	shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].firstscan = firstrun;
	
	shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].pol_size = length;
	
	shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].pol_off  = offset;
	
	memcpy(shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].pol_data,pol_buf,length/8+1);
	
	memcpy(shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].pol_qlt,pol_qlt,length/8+1);
	
	memset(shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]].com_data[port_no].pol_en,0,length/8+1);
}
