#include "../../../dms/v4.0a/inc/dms_com.h"

#include "../../../dps/v4.0a/inc/dps_rec_def.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../dps/v4.0a/inc/ipc_common.h"

#include "../../../lan/v4.0a/inc/lan_common.h"
#include "../../../plc/v4.0a/inc/lcu_data.h"

#include "../../../dpp/v4.0a/inc/dppdef.h"
#include "../../../dpp/v4.0a/inc/dpp_functions.h"





 void PlcAllSend(LCUID lcuno,struct plc_data_seg *seg, DPP_DATA *dpp)
{
	
	  time( &(dpp->ntime) );


	  if((dpp->first_scan==1)||(seg->firstscan ==1)||((long)(dpp->ntime - dpp->ana_otime) > 3000) )
	  {
	    


	  /*AI_ALL send*/
	    printf("now is sending ai_all data\n");

	    AllSend_ANA(lcuno,DPS_ANA_ALL,ANA_TYPE,
				  seg->head.ai_off, 
				  seg->head.ai_size, 
				  dpp->ai_all,				  
				  dpp->ai_time);


	  /*RTD_ALL send*/
	  printf("now is sending rtd_all data\n");

	  AllSend_ANA(lcuno,DPS_ANA_ALL,ANA_TYPE,
				  seg->head.rtd_off, 
				  seg->head.rtd_size, 
				  dpp->ai_all,
				  dpp->ai_time);

	  /*AO_ALL send*/
	  printf("now is sending ao_all data\n");
	
	  AllSend_ANA(lcuno,DPS_ANA_ALL,OUTA_TYPE,
				  seg->head.ao_off, 
				  seg->head.ao_size, 
				  dpp->ao_all,		 
				  dpp->ai_time);

	  dpp->ana_otime = dpp->ntime;
	 
	  }


#ifndef INDPRO
	  if((dpp->first_scan==1)||(seg->firstscan ==1)|| ((long)(dpp->ntime - dpp->imp_otime) >240) )
	  {
	    
	    printf("now is sending imp_all data\n\n");

	  /*PI_ALL send*/

	    AllSend_ANA(lcuno,DPS_IMP_ALL,IMP_TYPE,
				  seg->head.imp_off,
				  seg->head.imp_size, 
				  dpp->pi_all,			
				  dpp->ai_time);

	  dpp->imp_otime = dpp->ntime;
	  }
#endif	 
	  /*DII_ALL send*/

	  if((dpp->first_scan==1)||(seg->firstscan ==1)||(dpp->soeallsend_flag == 1)|| ((long)(dpp->ntime - dpp->soe_otime) > 120))
	  {

	    printf("now is sending soe_all data\n\n");
	  
	    AllSend_IND(lcuno,SOE_TYPE,
					  seg->head.soe_off, 
					  seg->head.soe_size, 
					  dpp->dii_all,	  
					  dpp->ai_time);


	    dpp->soe_otime = dpp->ntime;

	    if(dpp->soeallsend_flag == 1) dpp->soeallsend_flag =0;
		  
	  }


	  if((dpp->first_scan==1)||(seg->firstscan ==1)||((long)(dpp->ntime - dpp->ind_otime) > 30) )
	  {
	    
	    printf("now is sending msg_all data,off=%d ,size=%d\n\n",seg->head.msg_off,seg->head.msg_size);

	  /*MSG_ALL send*/

	    AllSend_IND(lcuno,SYSMSG_TYPE,
				  seg->head.msg_off,
				  seg->head.msg_size,
				  dpp->msg_all,
				  dpp->ai_time);
				  
	

	  /*DIN_ALL send*/
	  printf("now is sending pol_all data\n\n");	

	  AllSend_IND(lcuno,POL_TYPE,
				  seg->head.pol_off,
				  seg->head.pol_size,
				  dpp->din_all,
				  dpp->di_time);


	  /*DOUT_ALL send*/
	   printf("now is sending dout_all data\n\n");


	   AllSend_IND(lcuno,OUTS_TYPE,
				  seg->head.dout_off,
				  seg->head.dout_size,
				  dpp->dout_all,
				  dpp->di_time);
	  
	   dpp->ind_otime = dpp->ntime;
	  }

 

} 

