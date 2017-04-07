/************************************************************************************************************************************************
FILENAME:	ProcessCommData.c
FUNCTION:	
		functions of  processing all kinds of data sended from communication process :ProcessCommData()
		
		
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


void ProcessCommData(LCUID lcuno,struct plc_data_seg *seg, DPP_DATA *dpp)
{

	unsigned short no,i;
		
	printf("\n %d station ,%d lcu ProcessCommData is begining........\n",lcuno.stnid,lcuno.devid);
	
	for(no=0;no<seg->com_port_no;no++)
	{
		/***********************************	process AI   *******************************************************************/
	
		/*****	read ai change time *****/
	
	 	GetSysTime(&(dpp->ai_time));		

		if(seg->com_data[no].ai_size>0)
		{
		
	  		time( &(dpp->com_ai_ntime) );

	  		if((dpp->first_scan==1)||(seg->com_data[no].firstscan ==1)||((long)(dpp->com_ai_ntime - dpp->com_ai_otime) > ANA_ALLSEND_PERIOD)||(seg->com_ai_all_send_flag ==1 ))
	  			
	  			dpp->com_ai_allsend_flag = 1;

			/**read ai parameters from database**/

			ReadDb_ANA(lcuno,ANA_TYPE,seg->com_data[no].ai_off,seg->com_data[no].ai_size,dpp->ana_db);
	
			/***** read AI new value *****/

			ReadNewValue_ANA(lcuno,ANA_TYPE,seg->com_data[no].ai_off,
							seg->com_data[no].ai_size,
							seg->com_data[no].ai_data,
							seg->com_data[no].ai_qlt,
							seg->com_data[no].ai_en,dpp);

			if(dpp->first_scan==1)
			{
				for (i=seg->com_data[no].ai_off;i<seg->com_data[no].ai_off+seg->com_data[no].ai_size;i++)	 dpp->ai_value_old[i] = dpp->ai_value[i];
		   		for (i=0;i<AIALL_SIZE/16+1;i++)  dpp->ai_fault_old[i] = dpp->ai_fault[i];
		   		for (i=0;i<AIALL_SIZE/16+1;i++)  dpp->ai_force_old[i] = dpp->ai_force[i];
		 	}	 
  			
			Process_AI(lcuno,seg->com_data[no].ai_off,seg->com_data[no].ai_size,dpp,dpp->ai_time,dpp->com_ai_allsend_flag);
		
			if(dpp->ai_ptr2!=dpp->ai_ptr1)
			{
	  			printf("now ProcessCommData()::is sending ai change packet:ptr1=%d,ptr2=%d\n\n",dpp->ai_ptr1,dpp->ai_ptr2);
	
	  			dpp->ai_ptr2	= ChangeSend_ANA(lcuno,dpp->ai_ptr1,dpp->ai_ptr2,dpp->aibuf);

			}	

			if(dpp->aialm_ptr2!=dpp->aialm_ptr1)
			{
	  			printf("now ProcessCommData()::is sending aialm change packet:ptr1=%d,ptr2=%d\n\n",dpp->aialm_ptr1,dpp->aialm_ptr2);

	  			dpp->aialm_ptr2 = ChangeSend_ANA(lcuno,dpp->aialm_ptr1,dpp->aialm_ptr2,dpp->aialm);
			}
				

	  		if( dpp->com_ai_allsend_flag == 1)
	  		{
	    
	    			/*AI_ALL send*/
	    			printf("now ProcessCommData()::is sending com_ai_all data\n");
	    			
	    			dpp->com_ai_allsend_flag = 0;

	    			AllSend_ANA(lcuno,DPS_ANA_ALL,ANA_TYPE,
				  			seg->com_data[no].ai_off, 
				  			seg->com_data[no].ai_size, 
				  			dpp->ai_all,				  
				  			dpp->ai_time);
				  			 
				 dpp->com_ai_allsend_flag =0;
	    
	   		 	dpp->com_ai_otime = dpp->com_ai_ntime;
	  		}
		}
	
		/*******************************	process IMP    **************************************************************/
		
		if(seg->com_data[no].imp_size>0)
		{
		
			/** read pulse parameters from database **/
	
			ReadDb_IMP(lcuno,IMP_TYPE,
					seg->com_data[no].imp_off,
					seg->com_data[no].imp_size,				
					dpp->imp_db);
					
			/***** read PULSE value *****/

			ReadNewValue_IMP(lcuno,
						 seg->com_data[no].imp_off,
						 seg->com_data[no].imp_size,
						 seg->com_data[no].pi_data,
						 seg->com_data[no].pi_qlt,
						 seg->com_data[no].pi_en,
						 dpp);
			if(dpp->first_scan==1)
			{
		   		 for (i=0;i<PI_SIZE/16+1;i++)  dpp->pi_fault_old[i] = dpp->pi_fault[i];
		    		 for (i=0;i<PI_SIZE/16+1;i++)  dpp->pi_force_old[i] = dpp->pi_force[i];
			}	 

			Process_PI(lcuno, seg->com_data[no].imp_off,seg->com_data[no].imp_size,dpp,dpp->ai_time);	
	  		
	  		if(dpp->pi_ptr2!=dpp->pi_ptr1)
			{
				printf("now ProcessCommData()::is sending pi change packet:ptr1=%d,ptr2=%d\n\n",dpp->pi_ptr1,dpp->pi_ptr2);	

				dpp->pi_ptr2	= ChangeSend_IMP(lcuno,dpp->pi_ptr1,dpp->pi_ptr2,dpp->pibuf);
			}
		
		
			if(dpp->pialm_ptr2!=dpp->pialm_ptr1)
			{
	  			printf("now ProcessCommData()::is sending pialm change packet:ptr1=%d,ptr2=%d\n\n",dpp->pialm_ptr1,dpp->pialm_ptr2);

	  			dpp->pialm_ptr2 = ChangeSend_IMP(lcuno,dpp->pialm_ptr1,dpp->pialm_ptr2,dpp->pialm);
			}
	  		
	  		time( &(dpp->com_imp_ntime) );
	  
	  		if((dpp->first_scan==1)||(seg->com_data[no].firstscan ==1)|| ((long)(dpp->imp_ntime - dpp->imp_otime) >IMP_ALLSEND_PERIOD)||(seg->com_imp_all_send_flag ==1 ) )
	  		{
	    			/*PI_ALL send*/
	   			printf("now ProcessCommData()::is sending imp_all data\n\n");

	    			if(seg->imp_all_send_flag ==1 ) seg->imp_all_send_flag =0;

	   			AllSend_IMP(lcuno,DPS_IMP_ALL,IMP_TYPE,
				  			seg->com_data[no].imp_off,
				  			seg->com_data[no].imp_size, 
				  			dpp->pi_all,			
				  			dpp->ai_time);

	    			dpp->com_imp_otime = dpp->com_imp_ntime;
	  		}


		}
	
	
		/*****************************	process  POL / DOUT bitmap    **************************************************************/	
		
		if(seg->com_data[no].pol_size>0)
		{
	
			/*****	read di change time *****/
	
			GetSysTime(&(dpp->di_time));

			/**read pol parameters from database**/

			ReadDb_IND(lcuno,POL_TYPE,
						seg->com_data[no].pol_off,
						seg->com_data[no].pol_size,				
						dpp->ind_db);

			/***** read POL value *****/
			 			
			ReadNewValue_IND(lcuno,POL_TYPE,
							 seg->com_data[no].pol_off,
							 seg->com_data[no].pol_size,
							 seg->com_data[no].pol_data,
							 seg->com_data[no].pol_qlt,
							 seg->com_data[no].pol_en,
							 dpp);
			if(dpp->first_scan==1)
			{
				for (i=0;i<DIN_SIZE/16+1;i++)  dpp->pol_old[i] = dpp->pol_new[i];
				for (i=0;i<DIN_SIZE/16+1;i++)  dpp->pol_force_old[i] = dpp->pol_force[i];
				for (i=0;i<DIN_SIZE/16+1;i++)  dpp->pol_fault_old[i] = dpp->pol_fault[i];
			}
		
			Process_POL(lcuno, seg->com_data[no].pol_off,seg->com_data[no].pol_size,dpp,dpp->di_time);
		
			if(dpp->din_ptr2!=dpp->din_ptr1)
			{
				printf("now ProcessCommData()::is sending pol change packet:ptr1=%d,ptr2=%d\n\n",dpp->din_ptr1,dpp->din_ptr2);

				dpp->din_ptr2	= ChangeSend_IND(lcuno,dpp->din_ptr1,dpp->din_ptr2,dpp->dinbuf);
			}
		
			time( &(dpp->com_pol_ntime) );
	
			if((dpp->first_scan==1)||(seg->com_data[no].firstscan ==1)||((long)(dpp->pol_ntime - dpp->pol_otime) > IND_ALLSEND_PERIOD)||(seg->com_pol_all_send_flag ==1 ) )
			{
	    
	  			/*DIN_ALL send*/
	  			printf("now ProcessCommData()::is sending pol_all data\n\n");	

	 			AllSend_IND(lcuno,POL_TYPE,
				  		seg->com_data[no].pol_off,
				  		seg->com_data[no].pol_size,
				  		dpp->din_all,
				  		dpp->di_time);
	  
	   			dpp->com_pol_otime = dpp->com_pol_ntime;
	  		}

	  	}

	
	}
		
	
	if(seg->com_ai_all_send_flag ==1 ) seg->com_ai_all_send_flag =0;
	
	if(seg->com_imp_all_send_flag ==1) seg->com_imp_all_send_flag =0;

	if(seg->com_pol_all_send_flag ==1) seg->com_pol_all_send_flag =0;
}

  

