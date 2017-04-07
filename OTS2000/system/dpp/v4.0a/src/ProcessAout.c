/************************************************************************************************************************************************
FILENAME:	ProcessAout.c
FUNCTION:	
		functions of  processing ao data sended from plc to form alarm packet and allsend packet:ProcessAout()
		
		
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



void Process_AO(LCUID lcuno,USHORT offset,USHORT length,DPP_DATA *dpp,DATE_TIME tm,UCHAR allsendflag)
{
 unsigned short i,j,i0,num,num1,m,n,off,wordlen;
 
 float z,xcode;
 unsigned short  fault,force,fault_old,force_old;
  
 AISTRUCT	ai_object[AO_SIZE];
 AISTRUCT	alm1_object[AO_SIZE];


 /*printf("\n now is  AO_TYPE...............\n");*/

 /*********************** AO channel fault and alarm processing *****************/
	 num1=0;  num=0; 

	for (i= offset;i<offset+length;i++) 
	{
		 j=i/16;
		 i0=i-j*16;
	       
	 if(dpp->ana_db[i].iost.scanEnable ==1)
	 {

	         if ((dpp->ana_db[i].lo_code)<0) /*dpp->ana_new should be signed */
		
			xcode=(short)*(dpp->ao_new+i) ;
		else
			xcode=*(dpp->ao_new+i) ;

		if( xcode> dpp->ana_db[i].hi_code)	 dpp->ao_value[i]=	dpp->ana_db[i].hi_range;
			
		else if( xcode < dpp->ana_db[i].lo_code) dpp->ao_value[i]=	dpp->ana_db[i].lo_range;

		else
				dpp->ao_value[i]=( ( xcode- dpp->ana_db[i].lo_code ) * (dpp->ana_db[i].hi_range- dpp->ana_db[i].lo_range)
					/(dpp->ana_db[i].hi_code- dpp->ana_db[i].lo_code)+dpp->ana_db[i].lo_range)* (dpp->ana_db[i].eng_cvt) 
					+ (dpp->ana_db[i].compensate) + (dpp->ana_db[i].zero);
		 
		 /* Analog output value all-send form processing */
		 
		 fault= (dpp->ao_fault[j] >> i0) & 0x01;
		
    		 force= (dpp->ao_force[j] >> i0) & 0x01;
                 
 /*wzg20070506  if(allsendflag ==1)*/
                 {
                    memcpy(&(dpp->ao_all[i].status),&(dpp->ana_db[i].status),2);
		 		 
                    dpp->ao_all[i].data = dpp->ao_value[i]*1000;
					dpp->ao_all[i].status.alm_type  = ANA_ALM_TYPE_NORM;
                    dpp->ao_all[i].status.chan_fail = fault;
                    dpp->ao_all[i].status.ctrl_block = force ;
                    
                 }
                 
     if(!dpp->first_scan)
     {


		/******************  AO  channel fault alarm***********************************/
		
		 fault_old= (dpp->ao_fault_old[j] >> i0) & 0x01;
		 
		 force_old=  (dpp->ao_force_old[j] >> i0) & 0x01;
		 
		 if (fault != fault_old)
		 {
			 memset(&alm1_object[num1], 0, sizeof(alm1_object[num1]));
			alm1_object[num1].point_id = i;
			alm1_object[num1].data = dpp->ao_value[i]*1000;
			
			memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
				
			alm1_object[num1].status.alm_type = ANA_ALM_TYPE_CHANFAIL;			
			alm1_object[num1].status.chan_fail = fault;	
			
			num1++;			
		 }

		/******************  AO  man set alarm***********************************/

		 if (force != force_old)
		 {
			 memset(&alm1_object[num1], 0, sizeof(alm1_object[num1]));
			 
			alm1_object[num1].point_id = i;
			alm1_object[num1].data = dpp->ao_value[i]*1000;
			
			memcpy(&(alm1_object[num1].status),&(dpp->ana_db[i].status),2);
			alm1_object[num1].status.alm_type   = ANA_ALM_TYPE_CTRLBLOCK;
			alm1_object[num1].status.ctrl_block = force;
			
			num1++;
		 }
	 
		/*****************   AO normal change alarm**************************************/
	
		if (fault ==fault_old &&  force == force_old) 
		{
	
			 /****** value normal change processs ***********/ 
			 
			 z = dpp->ao_value[i]-dpp->ao_value_old[i];
  
  			 if (  ( dpp->ana_db[i].d_band!=0 && (fabs((double)z) >= (double) dpp->ana_db[i].d_band) )
				 ||( dpp->ana_db[i].d_band==0 && (fabs((double)z) >  (double) dpp->ana_db[i].d_band) ))
			 {
				 memset(&ai_object[num], 0, sizeof(ai_object[num]));
				 
				 ai_object[num].point_id = i;
				 ai_object[num].data = dpp->ao_value[i]*1000 ;
				 
				 memcpy(&(ai_object[num].status),&(dpp->ana_db[i].status),2); /*modified by yh 20070513*/
				 ai_object[num].status.alm_type = ANA_ALM_TYPE_LMT;
				 ai_object[num].status.lmt_state = ANA_ALM_NORMAL;
				 
				 /*added by yh 20070513*/
				 ai_object[num].status.chan_fail = fault;	
                 ai_object[num].status.ctrl_block = force ; 

				 num++;				
			
			 }			 			 
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

		memset(&dpp->aoalm[dpp->aoalm_ptr1], 0, sizeof(dpp->aoalm[dpp->aoalm_ptr1]));
		 dpp->aoalm[dpp->aoalm_ptr1].type_id = DPS_ANA_ALM;
		 dpp->aoalm[dpp->aoalm_ptr1].stn_id =lcuno.stnid;
		 dpp->aoalm[dpp->aoalm_ptr1].dev_id=lcuno.devid;
		 dpp->aoalm[dpp->aoalm_ptr1].data_type= OUTA_TYPE;
		 dpp->aoalm[dpp->aoalm_ptr1].number = n;
		 
		 memcpy(&(dpp->aoalm[dpp->aoalm_ptr1].ai_object[off]),&alm1_object[off],n*sizeof(AISTRUCT));
		 		 
		 GetLocalHostId(&dpp->aoalm[dpp->aoalm_ptr1].host_id);
		 dpp->aoalm[dpp->aoalm_ptr1].second = tm.second;
		 dpp->aoalm[dpp->aoalm_ptr1].minute = tm.minute;
		 dpp->aoalm[dpp->aoalm_ptr1].hour = tm.hour;
		 dpp->aoalm[dpp->aoalm_ptr1].day = tm.day;
		 dpp->aoalm[dpp->aoalm_ptr1].month = tm.month;
		 dpp->aoalm[dpp->aoalm_ptr1].year = tm.year;   

		 Record_ANA(dpp->aoalm_ptr1,dpp->aoalm);

		 if (dpp->aoalm_ptr1 == ALM_BUF_SIZE-1 ) dpp->aoalm_ptr1=0;

		 else  dpp->aoalm_ptr1 = dpp->aoalm_ptr1+1;

		 if (dpp->aoalm_ptr2 == dpp->aoalm_ptr1) 
		 { 
			 dpp->aoalm_ptr2=dpp->aoalm_ptr2+1;
			 
			 if (dpp->aoalm_ptr2 == ALM_BUF_SIZE) dpp->aoalm_ptr2=0; 
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

		 memset(&dpp->aobuf[dpp->ao_ptr1], 0, sizeof(dpp->aobuf[dpp->ao_ptr1]));
		 dpp->aobuf[dpp->ao_ptr1].type_id = DPS_ANALOG;
		 dpp->aobuf[dpp->ao_ptr1].stn_id = lcuno.stnid;
		 dpp->aobuf[dpp->ao_ptr1].dev_id = lcuno.devid;
		 dpp->aobuf[dpp->ao_ptr1].data_type = OUTA_TYPE;
		 dpp->aobuf[dpp->ao_ptr1].number = n;
		 
		 memcpy(&(dpp->aobuf[dpp->ao_ptr1].ai_object[off]),&ai_object[off],n*sizeof(AISTRUCT));

		 GetLocalHostId(&dpp->aobuf[dpp->ao_ptr1].host_id);
		 dpp->aobuf[dpp->ao_ptr1].second = tm.second;
		 dpp->aobuf[dpp->ao_ptr1].minute = tm.minute;
		 dpp->aobuf[dpp->ao_ptr1].hour = tm.hour;
		 dpp->aobuf[dpp->ao_ptr1].day = tm.day;
		 dpp->aobuf[dpp->ao_ptr1].month = tm.month;
		 dpp->aobuf[dpp->ao_ptr1].year = tm.year;
		 
		 Record_ANA(dpp->ao_ptr1,dpp->aobuf);

		 if (dpp->ao_ptr1 == ALM_BUF_SIZE-1 ) dpp->ao_ptr1=0;
		 else	dpp->ao_ptr1 ++;
		 
		 if (dpp->ao_ptr2 == dpp->ao_ptr1) 
		 { 
			 dpp->ao_ptr2 ++;
			 
			 if (dpp->ao_ptr2 == ALM_BUF_SIZE) dpp->ao_ptr2=0; 
		 }
		 
		 
		m = m-n; off = off+n;
		
		}while(m!=0);			  		 	
     }
 
     for (i=offset;i<offset+length;i++)	dpp->ao_value_old[i] = dpp->ao_value[i];

	 j = offset/16;	
     wordlen = Mod16(offset+length);
     for (i=j;i<wordlen;i++)
	 {
		 dpp->ao_fault_old[i] = dpp->ao_fault[i];
		 dpp->ao_force_old[i] = dpp->ao_force[i];
	 }

}
