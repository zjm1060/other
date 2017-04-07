/************************************************************************************************************************************************
FILENAME:	ProcessSendata.c
FUNCTION:	
		functions of  processing pulse data  sended from plc to form alarm :ProcessPulse()
		

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



void Process_PI(LCUID lcuno,USHORT offset,USHORT length,DPP_DATA *dpp,DATE_TIME tm)
{
 unsigned short 	i,j,i0,num,num1,m,n,off,wordlen;
 unsigned short  	fault,force,fault_old,force_old;
  
 PISTRUCT	ai_object[PI_SIZE];
 PISTRUCT	alm1_object[PI_SIZE];

	memset(ai_object, 0, sizeof(ai_object));
	memset(alm1_object, 0, sizeof(alm1_object));
	
 /*printf("\n now is  IMP_TYPE...............\n");*/

 /*********************** IMP channel fault and alarm processing *****************/
	 num1=0;  num=0; 

	 for (i= offset;i<offset+length;i++) 
	 {
		 j=i/16;
		 i0=i-j*16;
	       
	     if(dpp->imp_db[i].iost.scanEnable ==1)
	     {

		/* pi value all-send form processing */
		 
		 fault= (dpp->pi_fault[j] >> i0) & 0x01;
		
    		 force= (dpp->pi_force[j] >> i0) & 0x01;
		 
		 dpp->pi_all[i].data = dpp->pi_new[i];
		 dpp->pi_all[i].status.option  = RW_IMP_ACCUM ;
                 dpp->pi_all[i].status.chan_fail  = fault ;
                 dpp->pi_all[i].status.man_set = force ;

		if(dpp->first_scan==1)  dpp->pi_old[i] = dpp->pi_new[i];

		/******************  IMP  channel fault alarm ****************/
		
		 fault_old= (dpp->pi_fault_old[j] >> i0) & 0x01;
		 
		 force_old=  (dpp->pi_force_old[j] >> i0) & 0x01;
		 
		 if (fault != fault_old)
		 {
			 
			alm1_object[num1].point_id = i;
			alm1_object[num1].data = dpp->pi_new[i];
				
			alm1_object[num1].status.alm_type = IMP_ALM_TYPE_CHANFAIL;			
			alm1_object[num1].status.chan_fail = fault;	
			
			num1++;			
		 }

		/******************  IMP man set alarm *********************/

		 if (force != force_old)
		 {
			 
			alm1_object[num1].point_id = i;
			alm1_object[num1].data = dpp->pi_new[i];
			
			alm1_object[num1].status.alm_type = IMP_ALM_TYPE_MANFORCE;
			alm1_object[num1].status.man_set = force;
			
			num1++;
		 }

		/*****************   IMP normal change alarm*****************/
	
		if ((fault == 0) &&(force == 0) )
		{
	
			 /****** value normal change processs ***********/ 
			 
  			 if (dpp->pi_old[i] != dpp->pi_new[i])
			 {
				
				 ai_object[num].point_id = i;
				 ai_object[num].data = dpp->pi_new[i];

				 ai_object[num].status.alm_type = IMP_ALM_TYPE_NORM;			 			 
				 ai_object[num].status.option = RW_IMP_ACCUM;


				 num++;				
			
			 }			 			 
		}
	     }
	 }

	m = num1;off=0;
	 
	if(num1>0)
	{		
		
		do{
		 
		 if ( m > ANA_ALM_MAX) n = ANA_ALM_MAX;
		 else n=m;

		 dpp->pialm[dpp->pialm_ptr1].type_id = DPS_IMP_ALM;
		 dpp->pialm[dpp->pialm_ptr1].stn_id =lcuno.stnid;
		 dpp->pialm[dpp->pialm_ptr1].dev_id=lcuno.devid;
		 dpp->pialm[dpp->pialm_ptr1].data_type= IMP_TYPE;
		 dpp->pialm[dpp->pialm_ptr1].number = n;
		 
		 memcpy(&(dpp->pialm[dpp->pialm_ptr1].ai_object[off]),&alm1_object[off],n*sizeof(AISTRUCT));
		 		 
		 GetLocalHostId(&dpp->pialm[dpp->pialm_ptr1].host_id);
		 dpp->pialm[dpp->pialm_ptr1].second = tm.second;
		 dpp->pialm[dpp->pialm_ptr1].minute = tm.minute;
		 dpp->pialm[dpp->pialm_ptr1].hour = tm.hour;
		 dpp->pialm[dpp->pialm_ptr1].day = tm.day;
		 dpp->pialm[dpp->pialm_ptr1].month = tm.month;
		 dpp->pialm[dpp->pialm_ptr1].year = tm.year;   

		/* Record_ANA(dpp->pialm_ptr1,dpp->pialm);*/

		 if (dpp->pialm_ptr1 == ALM_BUF_SIZE-1 ) dpp->pialm_ptr1=0;

		 else  dpp->pialm_ptr1 = dpp->pialm_ptr1+1;

		 if (dpp->pialm_ptr2 == dpp->pialm_ptr1) 
		 { 
			 dpp->pialm_ptr2=dpp->pialm_ptr2+1;
			 
			 if (dpp->pialm_ptr2 == ALM_BUF_SIZE) dpp->pialm_ptr2=0; 
		 }
		 
		 
		m = m-n; off = off+n;
		
		}while(m!=0);
 		
 	}


 	 m = num;off=0;
 	 
	 if(num>0)
	 {
		
		do{
		 
		 if ( m > ANA_ALM_MAX) n = ANA_ALM_MAX;
		 else n=m;

		 dpp->pibuf[dpp->pi_ptr1].type_id = DPS_IMPULSE;
		 dpp->pibuf[dpp->pi_ptr1].stn_id = lcuno.stnid;
		 dpp->pibuf[dpp->pi_ptr1].dev_id = lcuno.devid;
		 dpp->pibuf[dpp->pi_ptr1].data_type = IMP_TYPE;
		 dpp->pibuf[dpp->pi_ptr1].number = n;
		 
		 memcpy(&(dpp->pibuf[dpp->pi_ptr1].ai_object[off]),&ai_object[off],n*sizeof(AISTRUCT));

		 GetLocalHostId(&dpp->pibuf[dpp->pi_ptr1].host_id);
		 dpp->pibuf[dpp->pi_ptr1].second = tm.second;
		 dpp->pibuf[dpp->pi_ptr1].minute = tm.minute;
		 dpp->pibuf[dpp->pi_ptr1].hour = tm.hour;
		 dpp->pibuf[dpp->pi_ptr1].day = tm.day;
		 dpp->pibuf[dpp->pi_ptr1].month = tm.month;
		 dpp->pibuf[dpp->pi_ptr1].year = tm.year;
		 
		/* Record_ANA(dpp->pi_ptr1,dpp->pibuf);*/

		 if (dpp->pi_ptr1 == ALM_BUF_SIZE-1 ) dpp->pi_ptr1=0;
		 else	dpp->pi_ptr1 ++;
		 
		 if (dpp->pi_ptr2 == dpp->pi_ptr1) 
		 { 
			 dpp->pi_ptr2 ++;
			 
			 if (dpp->pi_ptr2 == ALM_BUF_SIZE) dpp->pi_ptr2=0; 
		 }
		 
		 
		m = m-n; off = off+n;
		
		}while(m!=0);			  		 	
     }
 
     for (i=offset;i<offset+length;i++)	dpp->pi_old[i] = dpp->pi_new[i];

	 j = offset/16;	
     wordlen = Mod16(offset+length);
        
    for (i=j;i<wordlen;i++)
	{
		dpp->pi_fault_old[i] = dpp->pi_fault[i];
		dpp->pi_force_old[i] = dpp->pi_force[i];
	}
}



