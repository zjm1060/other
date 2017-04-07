/************************************************************************************************************************************************
FILENAME:	ProcessSendata.c
FUNCTION:	
		functions of  processing data  sended to plc :Process_Sendata()
		

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



void Process_Sendata(LCUID lcuno, SENDATA * s_data, struct plc_send_seg *send_seg)
{
	unsigned short 	i,j,i0,no,s; 	
	DMS_COMMON		common_aa;
	float 			hi_range,lo_range,hi_code,lo_code,xcode;
	float		fValue;
	unsigned short		plc_code;
	unsigned char	 	chan[SEND_ANA_SIZE];
	unsigned char	 	status[SEND_POL_SIZE];

	memset(send_seg->ana_qlt, 0, sizeof(send_seg->ana_qlt));

	/********** DOWNLOAD ANA**********/
 	no=0; 
 
 	for(i=0;i< s_data->anum;i++)
 	{
		if (ReadEntryByNameStr(s_data->ana[i].l_name, &common_aa)== -1)
		{
			SendTraceMsg(0, 0, "in Process_Sendata(): No this point=%d in the database file !!!\n", s_data->ana[i].l_name);
			plc_code = 0xffff;
		} 
		else
		{			
			lo_range = common_aa.var.anlg.eng_para.LO_RANGE;
			hi_range = common_aa.var.anlg.eng_para.HI_RANGE;
			lo_code  = common_aa.var.anlg.eng_para.LO_CODE;
			hi_code  = common_aa.var.anlg.eng_para.HI_CODE;
			fValue   = common_aa.var.anlg.fValue;

			if (hi_range == lo_range) 
			{	
				SendTraceMsg(0, 0, "in Process_Sendata():(hi_range-lo_range =0), ERROR!");
				plc_code = 0xffff;
			}
			else
			{
				chan[i] = common_aa.var.anlg.value.status.chan_fail & 0x01;
				if( fValue > hi_range)	 
					fValue= hi_range;
					
				if( fValue < lo_range)     
					fValue=	lo_range;

				xcode = ((fValue - common_aa.var.anlg.eng_para.COMPSATE -common_aa.var.anlg.eng_para.ZERO) /common_aa.var.anlg.eng_para.ENG_CVT-lo_range)
							/(hi_range - lo_range)*(hi_code -lo_code )+lo_code;
							
				plc_code = (unsigned short)xcode;
			}
		}

		memcpy(&send_seg->ana_data[no],&plc_code,2);
		Swap2Byte(&send_seg->ana_data[no]);	
		
		no=no+2;
		
	}

	s=0;i=0;	
	for(i0=0;i0<s_data->anum;i0++)
	{
		j = i0%8;
		s = s |(chan[i0]<<j);
		send_seg->ana_qlt[i] = s ;
		if((i0+1)%8 ==0) 
		{			
			i=i+1;
			s=0;
		}
	}	

	/********** DOWNLOAD ANA_SCALE**********/
	no=0; 
	
	for(i=0;i< s_data->anum_scale;i++)
	{
		if (ReadEntryByNameStr(s_data->ana_scale[i].l_name, &common_aa)== -1)
		{
			SendTraceMsg(0, 0, "in Process_Sendata(): No this point=%d in the database file !!!\n", s_data->ana_scale[i].l_name);
			plc_code = 0xffff;
		} 
		else
		{
			chan[i] = common_aa.var.anlg.value.status.chan_fail & 0x01;
			plc_code = (short)(common_aa.var.anlg.fValue*s_data->ana_scale[i].scale);
			SendTraceMsg(0,0, "ana_scale %d: %d %f %f", i, plc_code,
						common_aa.var.anlg.fValue, s_data->ana_scale[i].scale);
		}

		memcpy(&send_seg->ana_data[no+SEND_ANA_SIZE], &plc_code, 2);
		Swap2Byte(&send_seg->ana_data[no+SEND_ANA_SIZE]);	

		no=no+2;
	}
	
	for(i0=0;i0<s_data->anum_scale;i0++)
	{
		int j= i0+SEND_ANA_SIZE/2;
		char	qlt= chan[i0]<<(j%8);

		send_seg->ana_qlt[j/8] |= qlt;
	}	

	send_seg->ai_size = s_data->anum + s_data->anum_scale;
	
	/********** DOWNLOAD IND **********/	
 	for(i=0;i< s_data->dnum;i++)
 	{
		if (ReadEntryByNameStr(s_data->ind[i].l_name, &common_aa)== -1)
		{
			SendTraceMsg(0, 0, "in Process_Sendata(): No this point=%d in the database file !!!\n", s_data->ind[i].l_name);
			plc_code = 0x0;
		} 
 				
		status[i] = common_aa.var.ind.status.rt_state & 0x01;					
	}
	
	
	s=0;i=0;	
	for(i0=0;i0<s_data->dnum;i0++)
	{
		j = i0%8;
		
		s = s |(status[i0]<<j);
			
		send_seg->pol_data[i] = s ;
		
		if((i0+1)%8 ==0) 
		{
			i=i+1;
			s=0;
		}
	}
	
	send_seg->pol_size = s_data->dnum;

	send_seg->send_update_flag++;	
	if(send_seg->send_update_flag >= MAX_FLAG_NUM) 
		send_seg->send_update_flag = 0;
}





