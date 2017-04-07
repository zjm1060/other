/************************************************************************************************************************************************
FILENAME:	ProcessMessage.c
FUNCTION:	
		functions of  processing sysmsg data sended from plc to form alarm :Process_LCU_MSG()
		functions of  processing sysmsg data from database for data polling command:Process_SYS_MSG()

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


void Process_LCU_MSG(LCUID lcuno,USHORT offset,USHORT length,DPP_DATA *dpp,DATE_TIME tm)
{


        /*printf("\n now is LCU_MSG.......\n");*/

	Process_IND_ALL(lcuno,SYSMSG_TYPE, offset,length,dpp);
	
	Process_IND(lcuno,SYSMSG_TYPE,offset,length,dpp,tm);

}



void Process_SYS_MSG(LCUID lcuno,USHORT offset,USHORT length,DPP_DATA *dpp,DATE_TIME tm)
{

	unsigned short          i0,i,j;
	IND_DATA                ind_all[DIN_SIZE];	
	IND_DATA          	fmt;

	
        /*printf("\n now is SYS_MSG.......\n");*/

	for (i=offset;i<offset+length;i++)
	{
		j=i/16;
		i0=i-16*j; 
		
		if  (dpp->ind_db[i].iost.scanEnable  ==  1)  
		{		   
			   fmt = dpp->ind_db[i].status;
        
			   ind_all[i]=fmt;
	
		}		
	}
	
	memcpy(dpp->msg_all+offset, ind_all+offset,2*length);
}


