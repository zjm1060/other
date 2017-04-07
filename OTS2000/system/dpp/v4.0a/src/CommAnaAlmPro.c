/************************************************************************************************************************************************
FILENAME:	CommAnaAlmPro.c
FUNCTION:	
		functions of  process one ai data  over-back-limit alarm packet of communication process:CommAnaAlmPro()
		
		
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

    
int CommAnaAlmPro(POINTER pt,float value,unsigned short fault,DATE_TIME tm)   /*return 0 means no alarm,return 1 means send alarm*/
{
 	unsigned short num;
 	float z;

 	ANA_STRUCT	ai_db; 	
   	AISTRUCT	alm_object[10];   /* alarm change array */
    	AIALMBUFSTRUCT 	aialm;
    	LCUID lcuno;
    	
    	lcuno.stnid = pt.stn_id;
	lcuno.devid = pt.dev_id;
	
	/*memset (&ai_db,0,sizeof(ANA_STRUCT));*/
	
    	ReadDb_ANA(lcuno,ANA_TYPE,pt.pt_id,1,&ai_db);
  	
	num=0;
				
	if(ai_db.iost.scanEnable ==1)
	{
		/****  AI  channel fault alarm *****/
		
		 if (fault != ai_db.status.chan_fail)
		 {
			 
			alm_object[num].point_id = pt.pt_id;
			alm_object[num].data 	 = ai_db.data;
			
			memcpy(&(alm_object[num].status),&(ai_db.status),2);
			
			alm_object[num].status.alm_type = ANA_ALM_TYPE_CHANFAIL;			
			alm_object[num].status.chan_fail = fault;
			
			num++;			
			
		 }

		/******  AI channel good *******/
	
		if (fault == 0)
		{
			 z = value - ai_db.fValue;
  
  			 if (  ( ai_db.d_band!=0 && (fabs((double)z) >= (double) ai_db.d_band) )
				 ||( ai_db.d_band==0 && (fabs((double)z) >  (double) ai_db.d_band) ))
			 {
				
				/*000******* normal Analog input  processing *************/
				   				
				if ((value >= ai_db.l_lim) && (value <= ai_db.h_lim))
   				{
					 /* Input value reverse to normal value */  
					 
					if ((ai_db.status.lmt_state !=0 )&& (ai_db.status.lmt_state<=4)) 
						
						if (						    
							 ((value >= ai_db.lr_lmt) && (value <= ai_db.hr_lmt))||						
							   ( (value >= ai_db.l_lim)&&(value <= ai_db.h_lim)&& 
								(
								   (((ai_db.status.lmt_state ==1)||(ai_db.status.lmt_state ==2))&&(value < ai_db.lr_lmt)) ||
							           (((ai_db.status.lmt_state ==3)||(ai_db.status.lmt_state ==4))&&(value > ai_db.hr_lmt))
								 )
							   )							  						       
						   )		/*if( A||(B&&C&&(D||E)))	*/				
						   
						{
						      /*  printf(" %d back limit change alarm\n",pt.pt_id);*/

							alm_object[num].point_id = pt.pt_id;
							alm_object[num].data = ai_db.data;
							
							memcpy(&(alm_object[num].status),&(ai_db.status),2);
							alm_object[num].status.alm_type = ANA_ALM_TYPE_LMT;
							
							if((ai_db.status.lmt_state ==1)||(ai_db.status.lmt_state ==2))
								alm_object[num].status.lmt_state = ANA_ALM_HI_RECOVER;
								
							if((ai_db.status.lmt_state ==3)||(ai_db.status.lmt_state ==4))
								alm_object[num].status.lmt_state = ANA_ALM_LO_RECOVER;				

							num++;
						}
				}

				/*111***Input value over high-limit alarm processing *****/
				
				if ((value > ai_db.h_lim) && (value <= ai_db.hh_lim))
				  
					if (ai_db.status.lmt_state != ANA_ALM_HI_WARN )						
					{   
						/* printf("%d over high-limit alarm\n",pt.pt_id);  */

						 alm_object[num].point_id = pt.pt_id;
						 alm_object[num].data =ai_db.data ;
						 
						 memcpy(&(alm_object[num].status),&(ai_db.status),2);
						 alm_object[num].status.alm_type  = ANA_ALM_TYPE_LMT;
						 alm_object[num].status.lmt_state = ANA_ALM_HI_WARN;

						 num++;
						
					}

				/**222*** Input value over high-high-limit alarm processing *****/	

				if (value >= ai_db.hh_lim)  
				
				   	if (ai_db.status.lmt_state != ANA_ALM_HI_ALARM)     
				  	{     

					 	alm_object[num].point_id = pt.pt_id;
					 	alm_object[num].data = ai_db.data;
					 	
					 	memcpy(&(alm_object[num].status),&(ai_db.status),2);
					 	alm_object[num].status.alm_type  = ANA_ALM_TYPE_LMT;
					 	alm_object[num].status.lmt_state = ANA_ALM_HI_ALARM;

					 	num++;
					 
				  	}

			      /**333******  Input value over lower-limit alarm processing *****/ 

			     if ((value < ai_db.l_lim) && (value >= ai_db.ll_lim))   
				  
				  if (ai_db.status.lmt_state !=ANA_ALM_LO_WARN)
					
				  {  
					/* printf("%d over lower-limit alarm\n",pt.pt_id);  */


					 alm_object[num].point_id = pt.pt_id;
					 alm_object[num].data = ai_db.data;
					 
					 memcpy(&(alm_object[num].status),&(ai_db.status),2);
					 alm_object[num].status.alm_type  = ANA_ALM_TYPE_LMT;
					 alm_object[num].status.lmt_state = ANA_ALM_LO_WARN;

					 num++;
					
				  }

			     /**444*****  Input value over low-lower-limit alarm processing *******/

			      if (value < ai_db.ll_lim)  
				  
				  if (ai_db.status.lmt_state !=ANA_ALM_LO_ALARM) 
					
				  {     
					 /*printf("%d over lower-lower-limit alarm\n",pt.pt_id);  */

					 alm_object[num].point_id = pt.pt_id;
					 alm_object[num].data = ai_db.data;
					 
					 memcpy(&(alm_object[num].status),&(ai_db.status),2);
					 alm_object[num].status.alm_type  = ANA_ALM_TYPE_LMT;
					 alm_object[num].status.lmt_state = ANA_ALM_LO_ALARM;
	
					 num++;
					
				  }
			 }
	
		}
	}
	
					
	/********************************************* combine  ai change compacket ******************************************************/	
			
	if(num>0)
	{		
		
		 aialm.type_id 		= DPS_ANA_ALM;
		 aialm.stn_id  		= pt.stn_id;
		 aialm.dev_id  		= pt.dev_id;
		 aialm.data_type	= ANA_TYPE;
		 aialm.number 		= num;
		 
		 memcpy(&(aialm.ai_object[0]),&alm_object,num*sizeof(AISTRUCT));
		 		 
		 GetLocalHostId(&aialm.host_id);
		 
		 aialm.second = tm.second;
		 aialm.minute = tm.minute;
		 aialm.hour = tm.hour;
		 aialm.day = tm.day;
		 aialm.month = tm.month;
		 aialm.year = tm.year;   
		 
		 
		 ChangeSend_ANA(lcuno,1,0,&aialm);
		 
		 return 1;	 
		 
 	}
 	
 	else return 0;
 	

 		
}

