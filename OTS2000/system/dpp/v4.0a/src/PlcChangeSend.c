#include "../../../dms/v4.0a/inc/dms_com.h"

#include "../../../dps/v4.0a/inc/dps_rec_def.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../dps/v4.0a/inc/ipc_common.h"

#include "../../../lan/v4.0a/inc/lan_common.h"
#include "../../../plc/v4.0a/inc/lcu_data.h"

#include "../../../dpp/v4.0a/inc/dppdef.h"
#include "../../../dpp/v4.0a/inc/dpp_functions.h"
			 

void  PlcChangeSend(LCUID lcuno,DPP_DATA *dpp)
{

	if(dpp->dii_ptr2!=dpp->dii_ptr1)
	{
	  printf("now is sending soe change packet:ptr1=%d,ptr2=%d\n\n",dpp->dii_ptr1,dpp->dii_ptr2);

	  dpp->dii_ptr2	= ChangeSend_SOE(lcuno,dpp->dii_ptr1,dpp->dii_ptr2,dpp->diibuf);
	}
	
	if(dpp->din_ptr2!=dpp->din_ptr1)
	{
	printf("now is sending pol change packet:ptr1=%d,ptr2=%d\n\n",dpp->din_ptr1,dpp->din_ptr2);

	dpp->din_ptr2	= ChangeSend_IND(lcuno,dpp->din_ptr1,dpp->din_ptr2,dpp->dinbuf);
	}
	
	if(dpp->msg_ptr2!=dpp->msg_ptr1)
	{
	printf("now is sending msg change packet:ptr1=%d,ptr2=%d\n\n",dpp->msg_ptr1,dpp->msg_ptr2);
																
	dpp->msg_ptr2	= ChangeSend_IND(lcuno,dpp->msg_ptr1,dpp->msg_ptr2,dpp->msgbuf);
	}
	
	if(dpp->dout_ptr2!=dpp->dout_ptr1)
	{
	     printf("now is sending dout change packet:ptr1=%d,ptr2=%d\n\n",dpp->dout_ptr1,dpp->dout_ptr2);
	     				      				      
	     dpp->dout_ptr2 = ChangeSend_IND(lcuno,dpp->dout_ptr1,dpp->dout_ptr2,dpp->doutbuf);
	}

	if(dpp->ai_ptr2!=dpp->ai_ptr1)
	{
	  printf("now is sending ai change packet:ptr1=%d,ptr2=%d\n\n",dpp->ai_ptr1,dpp->ai_ptr2);
	
	  dpp->ai_ptr2	= ChangeSend_ANA(lcuno,dpp->ai_ptr1,dpp->ai_ptr2,dpp->aibuf);

	}	

	if(dpp->aialm_ptr2!=dpp->aialm_ptr1)
	{
	  printf("now is sending aialm change packet:ptr1=%d,ptr2=%d\n\n",dpp->aialm_ptr1,dpp->aialm_ptr2);

	  dpp->aialm_ptr2 = ChangeSend_ANA(lcuno,dpp->aialm_ptr1,dpp->aialm_ptr2,dpp->aialm);
	}

	if(dpp->ao_ptr2!=dpp->ao_ptr1)
	{
	  printf("now is sending ao change packet:ptr1=%d,ptr2=%d\n\n",dpp->ao_ptr1,dpp->ao_ptr2);
	
	  dpp->ao_ptr2	= ChangeSend_ANA(lcuno,dpp->ao_ptr1,dpp->ao_ptr2,dpp->aobuf);

	}	

	if(dpp->aoalm_ptr2!=dpp->aoalm_ptr1)
	{
	  printf("now is sending aoalm change packet:ptr1=%d,ptr2=%d\n\n",dpp->aoalm_ptr1,dpp->aoalm_ptr2);

	 dpp->aoalm_ptr2 = ChangeSend_ANA(lcuno,dpp->aoalm_ptr1,dpp->aoalm_ptr2,dpp->aoalm);
	}

	if(dpp->pi_ptr2!=dpp->pi_ptr1)
	{
	printf("now is sending pi change packet:ptr1=%d,ptr2=%d\n\n",dpp->pi_ptr1,dpp->pi_ptr2);	

	dpp->pi_ptr2	= ChangeSend_ANA(lcuno,dpp->pi_ptr1,dpp->pi_ptr2,dpp->pibuf);
	}

}

