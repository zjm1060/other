/************************************************************************************************************************************************
FILENAME:	ProcessAnalog.c
FUNCTION:	
		functions of  processing ai data of dpp_data to form alarm packet and allsend packet:ProcessAnalog()
		
		
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


 /* flag_aialm =0 Analoge input normal 
    flag_aialm =1 Analoge input over hh-limit
    flag_aialm =2 Analoge input over h-limit
    flag_aialm =3 Analoge input over l-limit
    flag_aialm =4 Analoge input over ll-limit */


    
void Process_AI(LCUID lcuno,USHORT offset,USHORT length,DPP_DATA *dpp,DATE_TIME tm, UCHAR allsendflag)
{
 USHORT i,j,i0,j0,num,num1,m,n,off,wordlen,jend,mend;
 float z;
 float xcode;
 
 USHORT         fault,force,fault_old,force_old;  
 AISTRUCT		ai_object[AIALL_SIZE];     /* normal change array */
 AISTRUCT		alm1_object[AIALL_SIZE];   /* alarm change array */
 
 struct tm  atime;
 time_t     atime_t;
 float	    dt;
 float      grad[SAMPLE_NUM];
 

	memset(ai_object, 0, sizeof(ai_object));
	memset(alm1_object, 0, sizeof(alm1_object));

    atime.tm_sec = tm.second;
    atime.tm_min = tm.minute;
    atime.tm_hour= tm.hour;
    atime.tm_mday= tm.day;
    atime.tm_mon = tm.month;
    atime.tm_year= tm.year - 1900;
    atime.tm_isdst = 0;
   	
   	atime_t = mktime(&atime);
   	
   	num=0; num1=0;

	 for (i= offset;i<offset+length;i++) 
	 {
		 j=i/16;
		 i0=i-j*16;
		
	     if(dpp->ana_db[i].iost.scanEnable ==1)
	     {
	         if (dpp->ana_db[i].lo_code <0) /*dpp->ana_new should be signed */
		
				xcode=(short)*(dpp->ai_new+i) ;
		else
				xcode=*(dpp->ai_new+i) ;
		
			if( xcode> dpp->ana_db[i].hi_code)	 dpp->ai_value[i]=dpp->ana_db[i].hi_range; 
			
			else if( xcode < dpp->ana_db[i].lo_code) dpp->ai_value[i]=dpp->ana_db[i].lo_range; 

			else
				dpp->ai_value[i]=( ( xcode- dpp->ana_db[i].lo_code ) * (dpp->ana_db[i].hi_range- dpp->ana_db[i].lo_range)
					/(dpp->ana_db[i].hi_code- dpp->ana_db[i].lo_code)+dpp->ana_db[i].lo_range)* (dpp->ana_db[i].eng_cvt) 
					+ (dpp->ana_db[i].compensate) + (dpp->ana_db[i].zero);

			fault = (dpp->ai_fault[j] >> i0) & 0x01;  fault_old = (dpp->ai_fault_old[j] >> i0) & 0x01;
		
    			force = (dpp->ai_force[j] >> i0) & 0x01;  force_old = (dpp->ai_force_old[j] >> i0) & 0x01;

			/*************************** Analog input value All_Send form processing ************************************/
				
/*wzg20070506		 if(allsendflag==1)*/			 
			{
				memcpy(&(dpp->ai_all[i].status),&(dpp->ana_db[i].status),2);
				
				if(fault ==0)
				{
				
					if( xcode> dpp->ana_db[i].hi_code)	  dpp->ai_all[i].status.overflow=1;
		
					else if( xcode < dpp->ana_db[i].lo_code)  dpp->ai_all[i].status.overflow=1;

					else	dpp->ai_all[i].status.overflow=0;
				}
				else   dpp->ai_all[i].status.overflow=0;
                                
                if(dpp->first_scan==1) dpp->flag_overflow[i]= dpp->ai_all[i].status.overflow;
						
				dpp->ai_all[i].data = (int)(dpp->ai_value[i]*1000);
				
				dpp->ai_all[i].status.alm_type   = ANA_ALM_TYPE_NORM;
				dpp->ai_all[i].status.chan_fail  = fault ;/*printf("fault=%d\n",dpp->ai_all[i].status.chan_fail);*/
				dpp->ai_all[i].status.ctrl_block = force ;/*printf("i=%d,force=%d\n",i,dpp->ai_all[i].status.ctrl_block);*/
						
				if (dpp->ai_all[i].status.overflow==0 && (dpp->ai_value[i] > dpp->ana_db[i].h_lim) && (dpp->ai_value[i] <= dpp->ana_db[i].hh_lim)) 
					dpp->ai_all[i].status.lmt_state = ANA_ALM_HI_WARN;

				if (dpp->ai_all[i].status.overflow==0 && dpp->ai_value[i] > dpp->ana_db[i].hh_lim)
					dpp->ai_all[i].status.lmt_state = ANA_ALM_HI_ALARM;

				if (dpp->ai_all[i].status.overflow==0 && (dpp->ai_value[i] < dpp->ana_db[i].l_lim) && (dpp->ai_value[i] >= dpp->ana_db[i].ll_lim))
					dpp->ai_all[i].status.lmt_state = ANA_ALM_LO_WARN;

				if (dpp->ai_all[i].status.overflow==0 && dpp->ai_value[i] < dpp->ana_db[i].ll_lim)
					dpp->ai_all[i].status.lmt_state = ANA_ALM_LO_ALARM;
                                
				if (dpp->ai_all[i].status.overflow==0 && (dpp->ai_value[i] <= dpp->ana_db[i].h_lim) && (dpp->ai_value[i] >= dpp->ana_db[i].l_lim))
					dpp->ai_all[i].status.lmt_state = ANA_ALM_NORMAL;
 
			 }
	 
		/*added by wzg 20080129*/
		if(dpp->first_scan)
			dpp->flag_aialm[i] = dpp->ai_all[i].status.lmt_state;
			
			
		/*************************** Analog input ALARM form processing ****************************************************/
		
		/****  AI  channel fault alarm *****/
		
		if(!dpp->first_scan)
		{

		 if (fault != fault_old)
		 {
			 alm1_object[num1].point_id = i;
			alm1_object[num1].data = (GLONG)(dpp->ai_value[i]*1000);
			
			memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
			alm1_object[num1].status.alm_type = ANA_ALM_TYPE_CHANFAIL;
			alm1_object[num1].status.chan_fail= fault;		
						
			num1++;			
			
		 }
	 	
		/*****  AI  io_ctrl block alarm ******/

		 if (force != force_old)
		 {
			 alm1_object[num1].point_id = i;
			alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
			
			memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
			alm1_object[num1].status.alm_type   = ANA_ALM_TYPE_CTRLBLOCK;
			alm1_object[num1].status.ctrl_block = force;
			
			num1++;
			
		 }
		
		/*****  AI overflow alarm ***** 
		

		if( fault==0 && (dpp->flag_overflow[i]==0)&& ((xcode> dpp->ana_db[i].hi_code) || ( xcode < dpp->ana_db[i].lo_code)) )
		{	
	
			alm1_object[num1].point_id = i;
			alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
			
			memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
			alm1_object[num1].status.alm_type   = ANA_ALM_TYPE_OVERFLOW;
			alm1_object[num1].status.overflow = 1;
			dpp->flag_overflow[i]=1;
			
			num1++;
		}
			
		if( fault==0 && (dpp->flag_overflow[i]==1)&& ((xcode<= dpp->ana_db[i].hi_code) && ( xcode >= dpp->ana_db[i].lo_code)) )
		{
			alm1_object[num1].point_id = i;
			alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
			
			memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
			alm1_object[num1].status.alm_type   = ANA_ALM_TYPE_OVERFLOW;
			alm1_object[num1].status.overflow = 0;
			dpp->flag_overflow[i]=0;
				
			num1++;
				
		}*/


		/*****  AI overflow NOT alarm, but change database,modified by ycx 070529 ******/ 		

		if( fault==0 && (dpp->flag_overflow[i]==0)&& ((xcode> dpp->ana_db[i].hi_code) || ( xcode < dpp->ana_db[i].lo_code)) )
		{	
			ai_object[num].point_id = i;
			ai_object[num].data = (int)(dpp->ai_value[i]*1000) ;
					
			memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);
			ai_object[num].status.alm_type  =  ANA_ALM_TYPE_NORM;   
			ai_object[num].status.lmt_state =  ANA_ALM_NORMAL ;
			ai_object[num].status.overflow = 1;

			dpp->flag_overflow[i]=1;

			num++;
		}
			
		if( fault==0 && (dpp->flag_overflow[i]==1)&& ((xcode<= dpp->ana_db[i].hi_code) && ( xcode >= dpp->ana_db[i].lo_code)) )
		{
			ai_object[num].point_id = i;
			ai_object[num].data = (int)(dpp->ai_value[i]*1000) ;
					
			memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);
			ai_object[num].status.alm_type  =  ANA_ALM_TYPE_NORM;   
			ai_object[num].status.lmt_state =  ANA_ALM_NORMAL ;
			ai_object[num].status.overflow = 0;

			dpp->flag_overflow[i]=0;

			dpp->flag_aialm[i] =0;	/******** add by ycx 070529 ******/

			num++;	
		}
		 
		/******  AI  normal change send when channel fault or force value *******/
	
		if (fault_old==1 && fault == 1) 
		{
			 z = dpp->ai_value[i]-dpp->ana_db[i].fValue;
  
  			 if (  ( dpp->ana_db[i].d_band!=0 && (fabs((double)z) >= (double) dpp->ana_db[i].d_band) )
				 ||( dpp->ana_db[i].d_band==0 && (fabs((double)z) >  (double) dpp->ana_db[i].d_band) ))
			 {
				ai_object[num].point_id = i;
				ai_object[num].data = (int)(dpp->ai_value[i]*1000) ;
						
				memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);
				ai_object[num].status.alm_type  =  ANA_ALM_TYPE_NORM;   
				ai_object[num].status.lmt_state =  ANA_ALM_NORMAL ;
				ai_object[num].status.chan_fail= fault;
				ai_object[num].status.ctrl_block = force;
				ai_object[num].status.overflow = dpp->flag_overflow[i];
				
				/*printf("aaaa: %d normal change alarm,value=%f\n",i,dpp->ai_value[i]);*/
						  
				num++;
						
				continue;

			 }
		}
				
		 
		/******  AI channel is always good *******/
	
		if ((fault == 0) && (fault_old == 0) && (xcode <= dpp->ana_db[i].hi_code) && ( xcode >= dpp->ana_db[i].lo_code))
		{
			 z = dpp->ai_value[i]-dpp->ana_db[i].fValue;

  			 if (  ( dpp->ana_db[i].d_band!=0 && (fabs((double)z) >= (double) dpp->ana_db[i].d_band) )
				 ||( dpp->ana_db[i].d_band==0 && (fabs((double)z) >  (double) dpp->ana_db[i].d_band) ))
			 {
				/*000******* normal Analog input  processing *************/
				   				
				if ((dpp->ai_value[i] >= dpp->ana_db[i].l_lim) && (dpp->ai_value[i] <= dpp->ana_db[i].h_lim))
   				{
    				        if (dpp->flag_aialm[i] == 0)
      				        {  
      				        	 
						ai_object[num].point_id = i;
						ai_object[num].data = (int)(dpp->ai_value[i]*1000) ;
						
						memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);
						ai_object[num].status.alm_type  =  ANA_ALM_TYPE_NORM;   
						ai_object[num].status.lmt_state =  ANA_ALM_NORMAL ;
						ai_object[num].status.overflow = dpp->flag_overflow[i];
						 
						 /*printf("aaaa: %d normal change alarm,value=%f\n",i,dpp->ai_value[i]);*/
						  
						num++;
						
						continue;
					}
				
					 /* Input value reverse to normal value */  
					
					if  (dpp->flag_aialm[i] == 1) 
					   	if (dpp->ai_value[i] < (dpp->ana_db[i].h_lim-dpp->ana_db[i].hr_lmt)) 				    					   
					  	{
							alm1_object[num1].point_id = i;
							alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
							
							memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
							alm1_object[num1].status.alm_type  = ANA_ALM_TYPE_LMT;							
							alm1_object[num1].status.lmt_state= ANA_ALM_HH_RECOVER;
							num1++;
						
							alm1_object[num1].point_id = i;
							alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
							
							memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
							alm1_object[num1].status.alm_type  = ANA_ALM_TYPE_LMT;							
							alm1_object[num1].status.lmt_state= ANA_ALM_HI_RECOVER;
							num1++;
							
							dpp->flag_aialm[i]=0;
							dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
							continue;

						}
						else
						{
						      	
							alm1_object[num1].point_id = i;
							alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
							
							memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
							alm1_object[num1].status.alm_type  = ANA_ALM_TYPE_LMT;							
							alm1_object[num1].status.lmt_state= ANA_ALM_HH_RECOVER;
							num1++;
							
							ai_object[num].point_id = i;
							ai_object[num].data = (int)(dpp->ai_value[i]*1000);
							
							memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);
							ai_object[num].status.alm_type  = ANA_ALM_TYPE_NORM;					
							ai_object[num].status.lmt_state = ANA_ALM_HI_WARN;
							ai_object[num].status.overflow = dpp->flag_overflow[i];
							num++;
							
							dpp->flag_aialm[i]=2;						
							dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
							continue;

						}
						

					if ( dpp->flag_aialm[i] ==2 ) 
						if (dpp->ai_value[i] < (dpp->ana_db[i].h_lim-dpp->ana_db[i].hr_lmt)) 				    					   
						{
							alm1_object[num1].point_id = i;
							alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
							
							memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
							alm1_object[num1].status.alm_type  = ANA_ALM_TYPE_LMT;							
							alm1_object[num1].status.lmt_state= ANA_ALM_HI_RECOVER;
							num1++;
							
							dpp->flag_aialm[i]=0;
							dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
							continue;

						}
						else
						{
						      	ai_object[num].point_id = i;
							ai_object[num].data = (int)(dpp->ai_value[i]*1000);
							
							memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);
							ai_object[num].status.alm_type  = ANA_ALM_TYPE_NORM;					
							ai_object[num].status.lmt_state = ANA_ALM_HI_WARN;
							ai_object[num].status.overflow = dpp->flag_overflow[i];
							num++;
							
							dpp->flag_aialm[i]=2;						
							dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
							continue;
						
						}
					

					if (dpp->flag_aialm[i] ==3 )
						if(dpp->ai_value[i] >(dpp->ana_db[i].l_lim+dpp->ana_db[i].lr_lmt))	    
						{
							alm1_object[num1].point_id = i;
							alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
							
							memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
							alm1_object[num1].status.alm_type  = ANA_ALM_TYPE_LMT;							
							alm1_object[num1].status.lmt_state= ANA_ALM_LO_RECOVER;				
							num1++;
							
							dpp->flag_aialm[i]=0;
							dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
							continue;
						}
						else
						{
						    ai_object[num].point_id = i;
							ai_object[num].data = (int)(dpp->ai_value[i]*1000);
							
							memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);
							ai_object[num].status.alm_type  = ANA_ALM_TYPE_NORM;
						    ai_object[num].status.lmt_state = ANA_ALM_LO_WARN;
							ai_object[num].status.overflow = dpp->flag_overflow[i];
						    num++;
						    
						    dpp->flag_aialm[i]=3;
							dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
							continue;
						
						}

					if (dpp->flag_aialm[i] ==4 )
						if (dpp->ai_value[i] >(dpp->ana_db[i].l_lim+dpp->ana_db[i].lr_lmt))	    
						{
							alm1_object[num1].point_id = i;
							alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
							
							memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
							alm1_object[num1].status.alm_type  = ANA_ALM_TYPE_LMT;							
							alm1_object[num1].status.lmt_state = ANA_ALM_LL_RECOVER;				
							num1++;
							
							alm1_object[num1].point_id = i;
							alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
							
							memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
							alm1_object[num1].status.alm_type  = ANA_ALM_TYPE_LMT;							
							alm1_object[num1].status.lmt_state= ANA_ALM_LO_RECOVER;				
							num1++;
							
							dpp->flag_aialm[i]=0;
							dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
							continue;

						}
						else
						{

							alm1_object[num1].point_id = i;
							alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
							
							memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
							alm1_object[num1].status.alm_type  = ANA_ALM_TYPE_LMT;							
							alm1_object[num1].status.lmt_state = ANA_ALM_LL_RECOVER;				
							num1++;
							
						      	ai_object[num].point_id = i;
							ai_object[num].data = (int)(dpp->ai_value[i]*1000);
							
							memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);
							ai_object[num].status.alm_type  = ANA_ALM_TYPE_NORM;
						    ai_object[num].status.lmt_state = ANA_ALM_LO_WARN;
							ai_object[num].status.overflow = dpp->flag_overflow[i];
						    num++;
						    
						    dpp->flag_aialm[i]=3;
							dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
							continue;
							
						}

				}

				/*111***Input value over high-limit alarm processing *****/
				
				else if ((dpp->ai_value[i] > dpp->ana_db[i].h_lim) && (dpp->ai_value[i] <= dpp->ana_db[i].hh_lim))  
					if (dpp->flag_aialm[i] == 0) 						
					{   
						 alm1_object[num1].point_id = i;
						 alm1_object[num1].data =(int)(dpp->ai_value[i]*1000) ;
						 
						 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
						 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
						 alm1_object[num1].status.lmt_state  =  ANA_ALM_HI_WARN;
						 num1++;
						 
						 dpp->flag_aialm[i]=2;					
						 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
						 continue;
					}
					else if (dpp->flag_aialm[i] == 3) 						
					{   
						alm1_object[num1].point_id = i;
						alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
							
						memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
						alm1_object[num1].status.alm_type  = ANA_ALM_TYPE_LMT;							
						alm1_object[num1].status.lmt_state= ANA_ALM_LO_RECOVER;
						num1++;
										
						alm1_object[num1].point_id = i;
						alm1_object[num1].data =(int)(dpp->ai_value[i]*1000) ;

						memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
						alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
						alm1_object[num1].status.lmt_state  =  ANA_ALM_HI_WARN;
						num1++;

						dpp->flag_aialm[i]=2;						 
						dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
						continue;
					}
					
					else if (dpp->flag_aialm[i] ==4 )
					{

						 alm1_object[num1].point_id = i;
						 alm1_object[num1].data =(int)(dpp->ai_value[i]*1000) ;
						 
						 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
						 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
						 alm1_object[num1].status.lmt_state  =  ANA_ALM_LL_RECOVER;
						 num1++;

						 alm1_object[num1].point_id = i;
						 alm1_object[num1].data =(int)(dpp->ai_value[i]*1000) ;
						 
						 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
						 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
						 alm1_object[num1].status.lmt_state  =  ANA_ALM_LO_RECOVER;
						 num1++;
						 
						 alm1_object[num1].point_id = i;
						 alm1_object[num1].data =(int)(dpp->ai_value[i]*1000) ;
						 
						 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
						 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
						 alm1_object[num1].status.lmt_state  =  ANA_ALM_HI_WARN;
 						 num1++;
 						 
						 dpp->flag_aialm[i]=2;
						 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
						 
						 continue;
					}
					else if ((dpp->flag_aialm[i] == 1)  && (dpp->ai_value[i] < (dpp->ana_db[i].hh_lim-dpp->ana_db[i].hr_lmt)) )
					
				  	{     

					 	alm1_object[num1].point_id = i;
					 	alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 	memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 	alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 	alm1_object[num1].status.lmt_state =  ANA_ALM_HH_RECOVER;
					 	num1++;
					 	
					 	dpp->flag_aialm[i]= 2;
						dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
						continue;
				 	 }
				 	 
					else if ((dpp->flag_aialm[i] == 1) && (dpp->ai_value[i] > (dpp->ana_db[i].hh_lim-dpp->ana_db[i].hr_lmt)) )
					
				  	{     

						 ai_object[num].point_id = i;
						 ai_object[num].data = (int)(dpp->ai_value[i]*1000);
						 
						 memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);
						 ai_object[num].status.alm_type  =  ANA_ALM_TYPE_NORM;
						 ai_object[num].status.lmt_state  =  ANA_ALM_HI_ALARM;
						 ai_object[num].status.overflow = dpp->flag_overflow[i];
						 num++;
						 
					 	dpp->flag_aialm[i]= 1;
						dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
						continue;
				 	 }
					
					else /* if (flag_aialm[i] == 2) */
					{     
						 
						 
						 ai_object[num].point_id = i;
						 ai_object[num].data = (int)(dpp->ai_value[i]*1000);
						 
						 memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);
						 ai_object[num].status.alm_type  =  ANA_ALM_TYPE_NORM;
						 ai_object[num].status.lmt_state  =  ANA_ALM_HI_WARN;
						 ai_object[num].status.overflow = dpp->flag_overflow[i];
						 num++;
						 
						 dpp->flag_aialm[i]= 2;
						 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
						 
						 continue;
					}

				/**222*** Input value over high-high-limit alarm processing *****/	

				else if (dpp->ai_value[i] > dpp->ana_db[i].hh_lim)  
				   if (dpp->flag_aialm[i] == 0)
     
				  {     

					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_HI_WARN;
					 num1++;
					 
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_HI_ALARM;
					 num1++;
					 
					 dpp->flag_aialm[i]= 1;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 continue;
				  }
				
				  else if (dpp->flag_aialm[i] == 2)
     
				  {     

					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_HI_ALARM;
					 num1++;
					 
					 dpp->flag_aialm[i]= 1;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 continue;
				  }
				  else if (dpp->flag_aialm[i] == 3)
				  {     
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state =  ANA_ALM_LO_RECOVER;
					 num1++;

					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_HI_WARN;
					 num1++;
					 
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_HI_ALARM;
 					 num1++;
					 
					 dpp->flag_aialm[i]= 1;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 continue;
				  }
				  				  
				  else if (dpp->flag_aialm[i] == 4)
				  {     
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LL_RECOVER;
					 num1++;

					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state =  ANA_ALM_LO_RECOVER;
					 num1++;

					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_HI_WARN;
					 num1++;
					 
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_HI_ALARM;
					  num1++;
					  
					 dpp->flag_aialm[i]= 1;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 continue;
				  }
				  
				  else/*if (flag_aialm[i] == 1)*/
				  {     
					
					 
					 ai_object[num].point_id = i;
					 ai_object[num].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);
					 ai_object[num].status.alm_type  =  ANA_ALM_TYPE_NORM;
					 ai_object[num].status.lmt_state  =  ANA_ALM_HI_ALARM;
					 ai_object[num].status.overflow = dpp->flag_overflow[i];
					 num++;
					 
					 dpp->flag_aialm[i]=  1;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 
					 continue;
				  }

			      /**333******  Input value over lower-limit alarm processing *****/ 

			     else if ((dpp->ai_value[i] < dpp->ana_db[i].l_lim) && (dpp->ai_value[i] >= dpp->ana_db[i].ll_lim))   
				  if (dpp->flag_aialm[i] == 0 )					
				  {  
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LO_WARN;
					 num1++;
					 
					 dpp->flag_aialm[i]=3;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 
					 continue;
				  }
				  else if ( dpp->flag_aialm[i] ==1)
				  {  
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state =  ANA_ALM_HH_RECOVER;
					 num1++;

					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state =  ANA_ALM_HI_RECOVER;
					 
					 num1++;
					 
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LO_WARN;
					 num1++;
					 
					 dpp->flag_aialm[i]=3;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 continue;
				  }				  
				  else if ( dpp->flag_aialm[i] ==2)
				  {  
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state =  ANA_ALM_HI_RECOVER;
					 
					 num1++;
					 
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LO_WARN;
					 num1++;
					 
					 dpp->flag_aialm[i]=3;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 
					 continue;
				  }
				  
				  else if ((dpp->flag_aialm[i] == 4) &&(dpp->ai_value[i] > (dpp->ana_db[i].ll_lim+dpp->ana_db[i].lr_lmt)) ) 
				  {    		
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LL_RECOVER;
					 num1++;
					 
					 dpp->flag_aialm[i]= 3;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 continue;
				  }	
				  
				  else if ((dpp->flag_aialm[i] == 4)&&(dpp->ai_value[i] < (dpp->ana_db[i].ll_lim+dpp->ana_db[i].lr_lmt)) ) 
				  {    		
					 ai_object[num].point_id = i;
					 ai_object[num].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);					 					 
					 ai_object[num].status.alm_type  =  ANA_ALM_TYPE_NORM;
					 ai_object[num].status.lmt_state  =  ANA_ALM_LO_ALARM;
					 ai_object[num].status.overflow = dpp->flag_overflow[i];
					 num++;
					 
					 dpp->flag_aialm[i]= 4;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 continue;
				  }				  
			  
				  else  /*if ((flag_aialm[i] == 3)*/
				  {     
					
					 
					 ai_object[num].point_id = i;
					 ai_object[num].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);					 					 
					 ai_object[num].status.alm_type  =  ANA_ALM_TYPE_NORM;
					 ai_object[num].status.lmt_state  =  ANA_ALM_LO_WARN;
					 ai_object[num].status.overflow = dpp->flag_overflow[i];
					 
					 dpp->flag_aialm[i]= 3;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 
					 /*printf("6666: %d in lower-limit normal change,value=%f\n",i,dpp->ai_value[i]);*/
					 
					 num++;
					 continue;
				  }

			     /**444*****  Input value over low-lower-limit alarm processing *******/

			    else if (dpp->ai_value[i] < dpp->ana_db[i].ll_lim)  
				  if (dpp->flag_aialm[i] ==0) 					
				  {     
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LO_WARN;
					 num1++;
					 
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LO_ALARM;
					 num1++;
					 
					 dpp->flag_aialm[i]= 4;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 continue;
				  }
				  else if (dpp->flag_aialm[i] ==3) 					
				  {     
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LO_ALARM;
					 num1++;
					 
					 dpp->flag_aialm[i]= 4;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 continue;
				  }
				  else if (dpp->flag_aialm[i] ==1 )
				  {

					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state =  ANA_ALM_HH_RECOVER;
					 num1++;
					 
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state =  ANA_ALM_HI_RECOVER;
					 num1++;
					 
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LO_WARN;
					 num1++;
					 
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LO_ALARM;
					 num1++;
					 
					 dpp->flag_aialm[i]= 4;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 continue;
				  }
				  else if (dpp->flag_aialm[i] ==2 )
				  {
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state =  ANA_ALM_HI_RECOVER;
					 num1++;

					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LO_WARN;
					 num1++;
					 
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LO_ALARM;			   
					 num1++;

					 dpp->flag_aialm[i]= 4;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 
					 continue;
				  
				  }
			          else  /*if(flag_aialm[i] == 4)*/
				  {     
					 
					 
					 ai_object[num].point_id = i;
					 ai_object[num].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2);
					 ai_object[num].status.alm_type  =  ANA_ALM_TYPE_NORM;
					 ai_object[num].status.lmt_state  =  ANA_ALM_LO_ALARM;
					 ai_object[num].status.overflow = dpp->flag_overflow[i];
 					 num++;
 					 
 					 dpp->flag_aialm[i]= 4;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 continue;
				  }
			
				  else	
				  {
					  printf(" pt %d limit value is not valid !!!!!\n",i);
				  }		   	  	  
			 
			}
		 }
		
		 /******  AI channel is from fault to good *******/

		 if ((fault == 0) && (fault_old != 0) && (xcode <= dpp->ana_db[i].hi_code) && ( xcode >= dpp->ana_db[i].lo_code))
		{
			 z = dpp->ai_value[i]-dpp->ana_db[i].fValue;
  
			 if (  ( dpp->ana_db[i].d_band!=0 && (fabs((double)z) >= (double) dpp->ana_db[i].d_band) )
				 ||( dpp->ana_db[i].d_band==0 && (fabs((double)z) >  (double) dpp->ana_db[i].d_band) ))
			 {
				 /*111***Input value over high-limit alarm processing *****/
				
				 if ((dpp->ai_value[i] > dpp->ana_db[i].h_lim) && (dpp->ai_value[i] <= dpp->ana_db[i].hh_lim))  
				 {
						 alm1_object[num1].point_id = i;
						 alm1_object[num1].data =(int)(dpp->ai_value[i]*1000) ;
						 
						 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
						 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
						 alm1_object[num1].status.lmt_state  =  ANA_ALM_HI_WARN;
						 num1++;
						 
						 dpp->flag_aialm[i]=2;					
						 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
						 continue;
				}

				/**222*** Input value over high-high-limit alarm processing *****/	

				else if (dpp->ai_value[i] > dpp->ana_db[i].hh_lim)  				       
				{     
				 
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_HI_ALARM;
					 num1++;
					 
					 dpp->flag_aialm[i]= 1;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 continue;
				  }
				

			      /**333******  Input value over lower-limit alarm processing *****/ 

			     else if ((dpp->ai_value[i] < dpp->ana_db[i].l_lim) && (dpp->ai_value[i] >= dpp->ana_db[i].ll_lim))   
				  {  
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LO_WARN;
					 num1++;
					 
					 dpp->flag_aialm[i]=3;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 
					 continue;
				  }

			     /**444*****  Input value over low-lower-limit alarm processing *******/

			    else if (dpp->ai_value[i] < dpp->ana_db[i].ll_lim)  
				{     
					 
					 alm1_object[num1].point_id = i;
					 alm1_object[num1].data = (int)(dpp->ai_value[i]*1000);
					 
					 memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
					 alm1_object[num1].status.alm_type  =  ANA_ALM_TYPE_LMT;
					 alm1_object[num1].status.lmt_state  =  ANA_ALM_LO_ALARM;
					 num1++;
					 
					 dpp->flag_aialm[i]= 4;
					 dpp->ai_all[i].status.lmt_state= dpp->flag_aialm[i];
					 continue;
				 }
				 else	
				 {
					 printf(" pt %d limit value is not valid !!!!!\n",i);
				 }		   	  	  
			 
			}
		 }

		 
		}	/*end of if(! first_scan)*/
		 
		 
	     }/*end of if(scan_enable)*/
	     
	     
	} /*end of for() */
	

                  
	/*************************** Analog input TREND alarm form processing ********************************************/
	if(!dpp->first_scan)
	{
	
		for (i=offset;i<offset+length;i++)               
		{
			if((dpp->ana_db[i].ioattr.trendAlm ==0)&&(dpp->trendAlm_old[i]==1))	/* trendAlm flag modified by oix */
			{
				dpp->trendAlm_old[i] =0;
				dpp->trend_id[i]=0;
			}
		}		
	
	
			
		for (i=offset;i<offset+length;i++)  /* continuous points after first 5 points*/
		{
			if(dpp->ana_db[i].ioattr.trendAlm ==1)	
			{	 		
		 		j0=i/16;
		 		i0=i-j0*16;
		 	
				fault = (dpp->ai_fault[j0] >> i0) & 0x01;  fault_old = (dpp->ai_fault_old[j0] >> i0) & 0x01;
		
    				/*force = (dpp->ai_force[j0] >> i0) & 0x01;  force_old = (dpp->ai_force_old[j0] >> i0) & 0x01;*/

			 	if(dpp->trend_id[i] == 0xffff && fault ==0 && fault_old ==0)
			 	{
					/*printf("\nttttttttttttttttttttttt,i=%d\n",i);*/
				
					/* slide data and time */			
					dpp->value0[i] = dpp->value1[i]; 
					dpp->value1[i] = dpp->value2[i]; 
					dpp->value2[i] = dpp->value3[i]; 
					dpp->value3[i] = dpp->value4[i]; 
					dpp->value4[i] = dpp->ai_value[i];
			
					dpp->tm0[i] = dpp->tm1[i];
					dpp->tm1[i] = dpp->tm2[i];
					dpp->tm2[i] = dpp->tm3[i];
					dpp->tm3[i] = dpp->tm4[i] ;
					dpp->tm4[i] = atime_t;
				
					dpp->ms0[i] = dpp->ms1[i];
					dpp->ms1[i] = dpp->ms2[i];
					dpp->ms2[i] = dpp->ms3[i];
					dpp->ms3[i] = dpp->ms4[i];
					dpp->ms4[i] = tm.ms;
			
					/* move data and time to calcu grad */
			
					dpp->trend[i].value[0] = dpp->value0[i];
					dpp->trend[i].value[1] = dpp->value1[i];
					dpp->trend[i].value[2] = dpp->value2[i];
					dpp->trend[i].value[3] = dpp->value3[i];
					dpp->trend[i].value[4] = dpp->value4[i];
			
					dpp->trend[i].ai_tm[0] = dpp->tm0[i];
					dpp->trend[i].ai_tm[1] = dpp->tm1[i];
					dpp->trend[i].ai_tm[2] = dpp->tm2[i];
					dpp->trend[i].ai_tm[3] = dpp->tm3[i];
					dpp->trend[i].ai_tm[4] = dpp->tm4[i];	
							
					dpp->trend[i].ms[0]=dpp->ms0[i] ;
			 		dpp->trend[i].ms[1]=dpp->ms1[i] ;
			 		dpp->trend[i].ms[2]=dpp->ms2[i] ;
			 		dpp->trend[i].ms[3]=dpp->ms3[i] ;
			 		dpp->trend[i].ms[4]=dpp->ms4[i] ;

					for(j=0;j<SAMPLE_NUM-1;j++)
		
					{
						dt =(float)(((float)dpp->trend[i].ai_tm[j+1]+((float)dpp->trend[i].ms[j+1]/1000.0))-((float)dpp->trend[i].ai_tm[j]+((float)dpp->trend[i].ms[j]/1000.0)));
					
						/*printf("tm2=%d,tm1=%d,ms2=%d,ms1=%d,dt=%f\n",dpp->trend[i].ai_tm[j+1],dpp->trend[i].ai_tm[j],dpp->trend[i].ms[j+1],dpp->trend[i].ms[j],dt);*/
					
						grad[j] = (dpp->trend[i].value[j+1] - dpp->trend[i].value[j])/dt;
					
						/*printf("v2=%f,v1=%f\n",dpp->trend[i].value[j+1],dpp->trend[i].value[j]);*/
					
						/*printf("grad[%d]=%f  ,dt=%f\n",j,grad[j],dt);*/
					}
			
					
					
					//
					
					if(dpp->ana_db[i].hi_trlmt >=0)
					{
						  dpp->trend[i].t_value = (float)FACTOR4 * grad[3] + (float)FACTOR3 * grad[2] + (float)FACTOR2 * grad[1] + (float)FACTOR1 * grad[0];		

						dpp->trend[i].t_value=0;
						if(grad[3]>0)
							dpp->trend[i].t_value+=(float)FACTOR4 * grad[3];
						if(grad[2]>0)
							dpp->trend[i].t_value+=(float)FACTOR4 * grad[2];

						if(grad[1]>0)
							dpp->trend[i].t_value+=(float)FACTOR4 * grad[1];

						if(grad[0]>0)
							dpp->trend[i].t_value+=(float)FACTOR4 * grad[0];

						/*dpp->trend[i].t_value = (float)(fabs((double)dpp->trend[i].t_value ));*/						
						
						/*printf("hi_trlmt=%f,t_value=%f,t_value_old=%f\n",dpp->ana_db[i].hi_trlmt,dpp->trend[i].t_value,dpp->trend[i].t_value_old);*/
						
						if ((dpp->trend[i].t_flag == 0)
							&&(dpp->trend[i].t_value > dpp->ana_db[i].hi_trlmt)
							&&(dpp->trend[i].t_value_old > dpp->ana_db[i].hi_trlmt))
						{
							/*printf("\n %d over-trend-limit alarm,t_value=%f\n",i,dpp->trend[i].t_value);*/
							
							alm1_object[num1].point_id = i;
							alm1_object[num1].data   = (int)(dpp->trend[i].t_value*1000);
							
							memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
							alm1_object[num1].status.alm_type    =  ANA_ALM_TYPE_TR;
							alm1_object[num1].status.trend_state =  ANA_ALM_TR_WARN;
							
							dpp->trend[i].t_flag = 1;
							
							num1++;
						}
						
						if((dpp->trend[i].t_flag == 1)
							&&(dpp->trend[i].t_value < (dpp->ana_db[i].hi_trlmt-dpp->ana_db[i].hr_trlmt)))
						{
							/*printf("\n %d  back-trend-limit alarm,t_value=%f\n",i,dpp->trend[i].t_value);*/
							
							alm1_object[num1].point_id = i;
							alm1_object[num1].data   = (int)(dpp->trend[i].t_value*1000);
							
							memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
							alm1_object[num1].status.alm_type    =  ANA_ALM_TYPE_TR;
							alm1_object[num1].status.trend_state =  ANA_ALM_TR_RECOVER;
							
							dpp->trend[i].t_flag = 0;
							
							num1++;
						}
						
					}
					else 
					{
						dpp->trend[i].t_value=0;
						if(grad[3]<0)
							dpp->trend[i].t_value+=(float)FACTOR4 * grad[3];
						if(grad[2]<0)
							dpp->trend[i].t_value+=(float)FACTOR4 * grad[2];
						
						if(grad[1]<0)
							dpp->trend[i].t_value+=(float)FACTOR4 * grad[1];
						
						if(grad[0]<0)
							dpp->trend[i].t_value+=(float)FACTOR4 * grad[0];
				 				
						if ((dpp->trend[i].t_flag == 0)
							&&(dpp->trend[i].t_value < dpp->ana_db[i].hi_trlmt)
							&&(dpp->trend[i].t_value_old < dpp->ana_db[i].hi_trlmt))
						{
					 		
							alm1_object[num1].point_id = i;
							alm1_object[num1].data   = (int)(dpp->trend[i].t_value*1000);
							
							memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
							alm1_object[num1].status.alm_type    =  ANA_ALM_TYPE_TR;
							alm1_object[num1].status.trend_state =  ANA_ALM_TR_WARN;
							
							dpp->trend[i].t_flag = 1;
							
							num1++;
						}
						
						if((dpp->trend[i].t_flag == 1)
							&&(dpp->trend[i].t_value > (dpp->ana_db[i].hi_trlmt-dpp->ana_db[i].hr_trlmt)))
						{
					 		
							alm1_object[num1].point_id = i;
							alm1_object[num1].data   = (int)(dpp->trend[i].t_value*1000);
							
							memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
							alm1_object[num1].status.alm_type    =  ANA_ALM_TYPE_TR;
							alm1_object[num1].status.trend_state =  ANA_ALM_TR_RECOVER;
							
							dpp->trend[i].t_flag = 0;
							
							num1++;
						}
					}

					dpp->trend[i].t_value_old = dpp->trend[i].t_value;
					}	
			}
		}/*end of for */
		
		for (i=offset;i<offset+length;i++)               /* get first 5 points ai_value in 5 dpp scan times */
		{
			j0=i/16;
		 	i0=i-j0*16;
		 	
			fault = (dpp->ai_fault[j0] >> i0) & 0x01;  /*fault_old = (dpp->ai_fault_old[j0] >> i0) & 0x01;*/

			if((dpp->ana_db[i].ioattr.trendAlm ==1)&&(dpp->trendAlm_old[i]==0) && fault==0)	
			{
				if(dpp->trend_id[i] < SAMPLE_NUM)
				{
			
					dpp->trend[i].value[dpp->trend_id[i]]= dpp->ai_value[i];
			
					dpp->trend[i].ai_tm[dpp->trend_id[i]] = atime_t;
				
					dpp->trend[i].ms[dpp->trend_id[i]] = tm.ms;
				}
			
				dpp->trend_id[i]++;
			}
		
		}
	
		for (i=offset;i<offset+length;i++)		/* cal first trend after get first 5 points ai_value*/
		{ 
			if(dpp->ana_db[i].ioattr.trendAlm ==1)	
			{	

		      		if(dpp->trend_id[i] == SAMPLE_NUM)
		      		{
		      			/*printf("\n00000000000000000000000,i=%d\n",i);*/
		      	
					dpp->value0[i] = dpp->trend[i].value[0];
					dpp->value1[i] = dpp->trend[i].value[1];
					dpp->value2[i] = dpp->trend[i].value[2];
					dpp->value3[i] = dpp->trend[i].value[3];
					dpp->value4[i] = dpp->trend[i].value[4];
				
					dpp->tm0[i] = dpp->trend[i].ai_tm[0];
					dpp->tm1[i] = dpp->trend[i].ai_tm[1];
					dpp->tm2[i] = dpp->trend[i].ai_tm[2];
					dpp->tm3[i] = dpp->trend[i].ai_tm[3];
					dpp->tm4[i] = dpp->trend[i].ai_tm[4];
			
					dpp->ms0[i] = dpp->trend[i].ms[0];
					dpp->ms1[i] = dpp->trend[i].ms[1];
					dpp->ms2[i] = dpp->trend[i].ms[2];
					dpp->ms3[i] = dpp->trend[i].ms[3];
					dpp->ms4[i] = dpp->trend[i].ms[4];
			
					dpp->trend_id[i]= 0xffff;
					dpp->trendAlm_old[i] = 1;
		      
		        		for(j=0;j<SAMPLE_NUM-1;j++)
		
					{
						dt =(float)(((float)dpp->trend[i].ai_tm[j+1]+((float)dpp->trend[i].ms[j+1]/1000.0))-((float)dpp->trend[i].ai_tm[j]+((float)dpp->trend[i].ms[j]/1000.0)));				
				
						/*printf("tm2=%d,tm1=%d,ms2=%d,ms1=%d,dt=%f\n",dpp->trend[i].ai_tm[j+1],dpp->trend[i].ai_tm[j],dpp->trend[i].ms[j+1],dpp->trend[i].ms[j],dt);*/
				
						grad[j] = (dpp->trend[i].value[j+1] - dpp->trend[i].value[j])/dt;
					
						/*printf("v2=%f,v1=%f\n",dpp->trend[i].value[j+1],dpp->trend[i].value[j]);
						printf("grad[%d]=%f,dt=%f\n",j,grad[j],dt);*/
					}
			
					dpp->trend[i].t_value = (float)FACTOR4 * grad[3] + (float)FACTOR3 * grad[2] + (float)FACTOR2 * grad[1] + (float)FACTOR1 * grad[0];
			
					/*dpp->trend[i].t_value = (float)(fabs((double)dpp->trend[i].t_value ));*/
			
					/*printf("t_value=%f\n\n",dpp->trend[i].t_value);*/
	             		}					
			}
		}
		
	}/*end of if*/
	

					
	/********************************************* combine  aialm change compacket **********************************************************************/	
	
	m = num1;off=0;
	
	if(num1>0)
	{	
		#ifdef	DEBUG
	       printf("2222 aialm num1 =%d\n",num1);
		#endif

	    do{
		  
		 if ( m > ANA_ALM_MAX) n = ANA_ALM_MAX;
		 else n=m;

		 dpp->aialm[dpp->aialm_ptr1].type_id = DPS_ANA_ALM;
		 dpp->aialm[dpp->aialm_ptr1].stn_id =lcuno.stnid;
		 dpp->aialm[dpp->aialm_ptr1].dev_id=lcuno.devid;
		 dpp->aialm[dpp->aialm_ptr1].data_type= ANA_TYPE;
		 dpp->aialm[dpp->aialm_ptr1].number = n;
		 
		 memcpy(&(dpp->aialm[dpp->aialm_ptr1].ai_object[0]),&alm1_object[off],n*sizeof(AISTRUCT));
		 
		/* printf("pt=%d,alm_type1=%d,alm2=%d\n",dpp->aialm[dpp->aialm_ptr1].ai_object[0].point_id,dpp->aialm[dpp->aialm_ptr1].ai_object[0].status.alm_type,alm1_object[off].status.alm_type);*/
		 		 
		  GetLocalHostId(&dpp->aialm[dpp->aialm_ptr1].host_id);
		 dpp->aialm[dpp->aialm_ptr1].second = tm.second;
		 dpp->aialm[dpp->aialm_ptr1].minute = tm.minute;
		 dpp->aialm[dpp->aialm_ptr1].hour = tm.hour;
		 dpp->aialm[dpp->aialm_ptr1].day = tm.day;
		 dpp->aialm[dpp->aialm_ptr1].month = tm.month;
		 dpp->aialm[dpp->aialm_ptr1].year = tm.year;   

		 Record_ANA(dpp->aialm_ptr1,dpp->aialm);

		 if (dpp->aialm_ptr1 == ALM_BUF_SIZE-1 ) dpp->aialm_ptr1=0;

		 else  dpp->aialm_ptr1 = dpp->aialm_ptr1+1;
 
		 
		m = m-n; off = off+n;
		
		}while(m!=0);
 		
 	}
 	
	/********************************************* combine  ai change compacket **********************************************************************/	

 	 m = num;off=0; 
 	 
	 if(num>0)
	 {	
		#ifdef	DEBUG
	 		printf("1111 ai change num =%d\n",num);
		#endif
	 	
		do{
		 
		  if ( m > ANA_ALM_MAX) n = ANA_ALM_MAX;
		  else n=m;
		  
		 
		 dpp->aibuf[dpp->ai_ptr1].type_id = DPS_ANALOG;
		 dpp->aibuf[dpp->ai_ptr1].stn_id = lcuno.stnid;
		 dpp->aibuf[dpp->ai_ptr1].dev_id = lcuno.devid;
		 dpp->aibuf[dpp->ai_ptr1].data_type = ANA_TYPE;
		 dpp->aibuf[dpp->ai_ptr1].number = n;
		 
		 memcpy(&(dpp->aibuf[dpp->ai_ptr1].ai_object[0]),&ai_object[off],n*sizeof(AISTRUCT));
                 
		/* printf("pt=%d,alm_type1=%d,alm2=%d\n",dpp->aibuf[dpp->ai_ptr1].ai_object[0].point_id,dpp->aibuf[dpp->ai_ptr1].ai_object[0].status.alm_type,ai_object[off].status.alm_type);*/
	
         GetLocalHostId( &dpp->aibuf[dpp->ai_ptr1].host_id );
		 dpp->aibuf[dpp->ai_ptr1].second = tm.second;
		 dpp->aibuf[dpp->ai_ptr1].minute = tm.minute;
		 dpp->aibuf[dpp->ai_ptr1].hour = tm.hour;
		 dpp->aibuf[dpp->ai_ptr1].day = tm.day;
		 dpp->aibuf[dpp->ai_ptr1].month = tm.month;
		 dpp->aibuf[dpp->ai_ptr1].year = tm.year;
		 
		 Record_ANA(dpp->ai_ptr1,dpp->aibuf);

		 if (dpp->ai_ptr1 == ALM_BUF_SIZE-1 ) dpp->ai_ptr1=0;
		 else	dpp->ai_ptr1 ++;
		 
		 if (dpp->ai_ptr2 == dpp->ai_ptr1) 
		 { 
			 dpp->ai_ptr2 ++;
			 
			 if (dpp->ai_ptr2 == ALM_BUF_SIZE) dpp->ai_ptr2=0; 
		 }
		 
		 
		m = m-n; off = off+n;
		
		}while(m!=0);
			 		 	
	 }

    for (i=offset;i<offset+length;i++)	dpp->ai_value_old[i] = dpp->ai_value[i]; 
 
    
	//modified by ycx 2012.3.28
	j = offset/16;		/*the first short array no*/
	m = offset%16;		/*the first short array bit no:[0,15]*/

	jend = (offset+length)/16;	/*the last short array no*/
	mend = (offset+length)%16;	/*the last short array bit no:[0,15]*/


    if(m==0&&mend==0)
	{
		for (i=j;i<jend;i++)		
		{	
			dpp->ai_fault_old[i] = dpp->ai_fault[i];
			dpp->ai_force_old[i] = dpp->ai_force[i];
		}
	}
	else if(m==0&&mend!=0)
	{
		for (i=j;i<jend;i++)		
		{	
			dpp->ai_fault_old[i] = dpp->ai_fault[i];
			dpp->ai_force_old[i] = dpp->ai_force[i];
		}
		
		dpp->ai_fault_old[i] = (dpp->ai_fault_old[i] & 0xff00)+(dpp->ai_fault[i] & 0x00ff);
		dpp->ai_force_old[i] = (dpp->ai_force_old[i] & 0xff00) +(dpp->ai_force[i] & 0x00ff);

	}
	else if(m!=0&&mend==0)
	{
		dpp->ai_fault_old[j] = (dpp->ai_fault_old[j] & 0x00ff)+(dpp->ai_fault[j] & 0xff00);
		dpp->ai_force_old[j] = (dpp->ai_force_old[j] & 0x00ff) +(dpp->ai_force[j] & 0xff00);

		for (i=j+1;i<jend;i++)		
		{	
			dpp->ai_fault_old[i] = dpp->ai_fault[i];
			dpp->ai_force_old[i] = dpp->ai_force[i];
		}

	}
	else
	{
		dpp->ai_fault_old[j] = (dpp->ai_fault_old[j] & 0x00ff)+(dpp->ai_fault[j] & 0xff00);
		dpp->ai_force_old[j] = (dpp->ai_force_old[j] & 0x00ff) +(dpp->ai_force[j] & 0xff00);

		for (i=j+1;i<jend;i++)		
		{	
			dpp->ai_fault_old[i] = dpp->ai_fault[i];
			dpp->ai_force_old[i] = dpp->ai_force[i];
		}

		dpp->ai_fault_old[i] = (dpp->ai_fault_old[i] & 0xff00)+(dpp->ai_fault[i] & 0x00ff);
		dpp->ai_force_old[i] = (dpp->ai_force_old[i] & 0xff00) +(dpp->ai_force[i] & 0x00ff);

	}

}

void Record_ANA(USHORT ptr,AIALMBUFSTRUCT *aibuf)
{	

#ifdef DEBUG
	
	FILE *fp;
	int i,num,result,fh;
	unsigned short status;
	
	struct _stat buf;	
	unsigned long  No,fsize;

	#ifdef WINDOWS_OS	
		char filename[256]={"\\home\\ems\\h9000\\his\\trap\\ana_alm.txt"};
		char cmd[256]={"del "};
	#else
		char filename[256]={"//home//ems//h9000//his//trap//ana_alm.txt"};
		char cmd[256]={"rm "};
	#endif

	 if((fh = _open(filename,_O_RDONLY))!=-1)
	 {
		 result =_fstat(fh,&buf);

		 if(result!=0)
			 printf("Bad file handle\n");
		 else
		 {
			 _close(fh);	 

			 if((fsize = buf.st_size )>=RECORD_FILE_SIZE) 			
			 {
				 printf("%s size=%ld\n",filename,fsize);
				 
				 strcat(cmd,filename);
				 printf("%s\n",cmd);
				 system(cmd); 
			 }
			 
		 }
	 }

	
	if (_access(filename,0)==-1)
	{
		printf("%s is not exist\n",filename);
			
		fp = fopen(filename,"w+");
		
		if(fp== NULL)	{printf(" can't create %s\n",filename); return;}
		
		rewind(fp);
		
		No=0;
		
		fprintf(fp,"%d\n",No);
		
		fclose(fp);	
	}

	fp = fopen(filename,"r+");

	     rewind(fp);
	
	     fscanf(fp,"%d",&No);
		 
	     /*printf("11No=%d\n",No);*/
		 
		 fseek(fp,0,SEEK_END);
		
		 fprintf(fp,"\nNo=%d,ptr=%d\n",No,ptr);
		 
		 fprintf(fp,"type_id=%d ,stn_id=%d ,dev_id=%d ,data_type=%d ,number=%d \n",(aibuf+ptr)->type_id,
		 			   	    	   (aibuf+ptr)->stn_id,
		 			   	    	   (aibuf+ptr)->dev_id,
		 			   	    	   (aibuf+ptr)->data_type,
		 			   	    	   (aibuf+ptr)->number);
		 num = (aibuf+ptr)->number;
		  		 
		 for(i=0;i<num;i++) 
		 {	
		 	memcpy(&status,&((aibuf+ptr)->ai_object[i].status),2);
		 	
		 	fprintf(fp,"pt_id=%4d,data=%11d,status=%4x\n",(aibuf+ptr)->ai_object[i].point_id,
		 					     		(aibuf+ptr)->ai_object[i].data,status);
		 }
		 
		 fprintf(fp,"host_id=%d,sec=%d,min=%d,hour=%d,day=%d,month=%d,year=%d\n\n",(aibuf+ptr)->host_id,
		 				     	    (aibuf+ptr)->second,(aibuf+ptr)->minute,
		 				     	    (aibuf+ptr)->hour,(aibuf+ptr)->day,
		 				     	    (aibuf+ptr)->month,(aibuf+ptr)->year);	
			                                   	 				     
		 	
		 No++;
		 	 
		 rewind(fp);
		 
		 fprintf(fp,"%d",No);		 
		 
		 fclose(fp);
	
#endif
}


