/************************************************************************************************************************************************
FILENAME:	ProcessParam.c
FUNCTION:	
		functions of  processing param data  sended from plc:ProcessParam()
		

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


void Process_PARAM(LCUID lcuno,USHORT offset,USHORT length,DPP_DATA *dpp,DATE_TIME tm)
{

        unsigned short i,j,i0;
        float value,xcode;

  /*********************** PARAM all send processing *****************/
	
	/*printf("\n now is  PARAM_TYPE...............\n");*/
	 
	for (i= offset;i<offset+length;i++) 
	 {
		 j=i/16;
		 i0=i-j*16;
	       
	     if(dpp->ana_db[i].iost.scanEnable ==1)
	     {

	         if ((dpp->ana_db[i].lo_code)<0) /*dpp->ana_new should be signed */
		
			{xcode=(short)*(dpp->param_new+i) ;}
		else
			{xcode=*(dpp->param_new+i) ;}
			
			
			
		if( xcode> dpp->ana_db[i].hi_code)	 dpp->ai_value[i]=	dpp->ana_db[i].hi_range;
			
		else if( xcode < dpp->ana_db[i].lo_code) dpp->ai_value[i]=	dpp->ana_db[i].lo_range;

		else
				value=( ( xcode- dpp->ana_db[i].lo_code ) * (dpp->ana_db[i].hi_range- dpp->ana_db[i].lo_range)
					/(dpp->ana_db[i].hi_code- dpp->ana_db[i].lo_code)+dpp->ana_db[i].lo_range)* (dpp->ana_db[i].eng_cvt) 
					+ (dpp->ana_db[i].compensate) + (dpp->ana_db[i].zero);
				
				
		 dpp->param_all[i].data = value*1000; 
		 
		 memcpy(&(dpp->param_all[i].status),&(dpp->ana_db[i].status),2);
                 
	     }
	 }


}
