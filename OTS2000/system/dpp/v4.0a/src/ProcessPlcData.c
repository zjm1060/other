/************************************************************************************************************************************************
FILENAME:	ProcessPlcData.c
FUNCTION:	
		function of judge plc is online or not:ReadLcuLinkStat()
		main function of  processing all kinds of data  sended from plc:ProcessPlcData()
		

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


///

extern SYS_COMM_TABLE	*shm_syscomm_addr;

/**********************read plc link status *****************/
int ReadLcuLinkStat(LCUID_INC_IDX lcuno,struct plc_data_seg *seg,DPP_DATA *dpp)//cxs
{	
	POINTER 	pt;
	USHORT		OnOffSta,LinkSta,ScanSta,HostSta;
	UCHAR		host_id;
	
	pt.stn_id=lcuno.lcu_id.stnid;
	pt.dev_id=lcuno.lcu_id.devid;
	pt.data_type=SYSMSG_TYPE;
	
	if(GetEntryIdByName( "LCU_ONOFF_STA", &pt)==-1)
	{
		printf ( "\n ProcessPlcdata:: No LCU_ONOFF_STA point of %d lcu %d station in the database!", lcuno.lcu_id.devid, lcuno.lcu_id.stnid);
		return -1;
	}
	if ( GetIndPtRtStateById (&pt ,&OnOffSta) == -1)
	{
		printf ( "\n ProcessPlcdata:: No LCU_ONOFF_STA point %d.%d.%d.%d in database\n",pt.stn_id,pt.dev_id,pt.data_type,pt.pt_id);
		return -1;
	}		


	if(GetEntryIdByName( "LINK_STA", &pt)==-1)
	{
		printf ( "\n ProcessPlcdata:: No LINK_STA point of %d lcu %d station in the database!", lcuno.lcu_id.devid, lcuno.lcu_id.stnid);
		return -1;
	}
	if ( GetIndPtRtStateById (&pt ,&LinkSta) == -1)
	{
		printf ( "\n ProcessPlcdata:: No LINK_STA point %d.%d.%d.%d in database\n",pt.stn_id,pt.dev_id,pt.data_type,pt.pt_id);
		return -1;
	}
	

	if(GetEntryIdByName( "SCAN_STA", &pt)==-1)
	{
		printf ( "\n ProcessPlcdata:: No SCAN_STA point of %d lcu %d station in the database!", lcuno.lcu_id.devid, lcuno.lcu_id.stnid);
		return -1;
	}	
	if ( GetIndPtRtStateById (&pt ,&ScanSta) == -1)
	{
		printf ( "\n ProcessPlcdata:: No SCAN_STA point %d.%d.%d.%d in database\n",pt.stn_id,pt.dev_id,pt.data_type,pt.pt_id);

		return -1;
	}
	

	if(GetEntryIdByName( "SCAN_HOST", &pt)==-1)
	{
		printf ( "\n ProcessPlcdata:: No SCAN_HOST point of %d lcu %d station in the database!", lcuno.lcu_id.devid, lcuno.lcu_id.stnid);

		return -1;
	
	}	
	if ( GetIndPtRtStateById (&pt ,&HostSta) == -1)
	{
		printf ( "\n ProcessPlcdata:: No SCAN_HOST point %d.%d.%d.%d in database\n",pt.stn_id,pt.dev_id,pt.data_type,pt.pt_id);

		return -1;
	}
	
	GetLocalHostId(&host_id);
	
	if (seg->endpp !=1) 
	{
		#ifdef DEBUG
		printf("\n  ####:  %d station ,%d lcu's seg->endpp=%d,本机非主采集服务器\n",lcuno.stnid,lcuno.devid,seg->endpp);
		#endif
		
		return -1; 
	}
		
	
	/******** dpp->isdataflag =0 ,dpp will not process data******/
	if((OnOffSta !=LCU_ON || LinkSta !=LCU_ON  || ScanSta != SCANRUN || seg->head_thr_valid!=1) && dpp->fault_count<10 ) /*modified ycx 050717*/
	{
		dpp->isdataflag =0;

		
		if(g_is_island==FALSE)	/* if is not in IPCMODE */
		{
			if(HostSta == host_id ) dpp->fault_count++;
		}
		else		
			dpp->fault_count++;
		
		
		
		#ifdef DEBUG		
		printf("\n dpp->isdataflag =0, error, 本主采集服务器 dpp 停止处理 %d station ,%d lcu 数据! dpp->fault_count=%d\n",lcuno.stnid,lcuno.devid,dpp->fault_count);
		printf("\n seg->endpp = %d, 但 在线状态 =%d,网络状态 =%d,采集状态 =%d,头区状态=%d\n",OnOffSta,LinkSta,ScanSta,seg->head_thr_valid);
		#endif			
		
	}
	
	/******** dpp->isdataflag = -1 ,dpp will set data chan_fail=1******/
	
	if(dpp->fault_count>=10)
	{	
		dpp->isdataflag = -1; 	
					
		#ifdef DEBUG	
		printf("\n dpp->isdataflag =-1, error, 本主采集服务器 dpp 报告 %d station ,%d lcu's 数据通道故障!\n",lcuno.stnid,lcuno.devid);
		#endif
				
	}

	/********* dpp->isdataflag =1,  dpp will process data***********/
		
	if( OnOffSta ==LCU_ON && LinkSta ==LCU_ON  && ScanSta == SCANRUN && seg->head_thr_valid==1) /*modified ycx 050717*/
	{
		if(g_is_island==FALSE)	/* if is not in IPCMODE */
		{
			if(HostSta == host_id ) {dpp->fault_count=0;dpp->isdataflag =1;}
		}
		else		
		{
			dpp->fault_count =0;dpp->isdataflag =1;
		}
		
		#ifdef DEBUG
		printf("\n dpp->isdataflag=1 : ok, 本主采集服务器 dpp 处理 %d station ,%d lcu 数据正常工作!\n",lcuno.stnid,lcuno.devid );	
		#endif
	}
	
   
	return 0;	
	
}


void ProcessPlcData(LCUID_INC_IDX lcuno,struct plc_data_seg *seg, DPP_DATA *dpp,struct plc_send_seg *sendseg)
{
	unsigned  short	i,ptr,pol_head,soealm_head,objalm_head;
	GULONG	  msg_num;
	
	unsigned char  	aic_qlt[(AIC_SIZE/16+1)*2];
	unsigned char  	aic_en [(AIC_SIZE/16+1)*2];
	
	unsigned char	msg_qlt[(MSG_SIZE/16+1)*2];
	unsigned char	msg_en[(MSG_SIZE/16+1)*2];
	
	unsigned char	pi_en[(PI_SIZE/16+1)*2];
	unsigned char	picomm_en[(PI_SIZE/16+1)*2];
	
	unsigned char	polc_qlt[(DIC_SIZE/16+1)*2];
	unsigned char	polc_en[(DIC_SIZE/16+1)*2];	
				
	unsigned char	ObjTimeFlag;
	DATE_TIME	Obj_time;

	LCUID   lcuid_tmp;//cxs
	
	lcuid_tmp.stnid=lcuno.lcu_id.stnid;//cxs
	lcuid_tmp.devid=lcuno.lcu_id.devid;//cxs
	
	#ifdef DEBUG
	printf("\n %d station ,%d lcu ProcessPlcData is begining........\n",lcuno.stnid,lcuno.devid);
	#endif
	
	if(seg->firstscan == 1 ) 
	{     
		char err_str[128];
		
		 dpp->soeptr = seg->soealm_ptr ;
       
       	 dpp->polptr = seg->pol_seg_ptr ;
      		        		 
		 dpp->objptr = seg->objalm_ptr;		/*add by ycx 07-07-04 */

       	 seg->firstscan=0;
       		 
		 sprintf(err_str, "plcscan.exe first run : seg->soealm_ptr= %d,seg->pol_seg_ptr= %d,seg->objalm_ptr=%d", seg->soealm_ptr,seg->pol_seg_ptr,seg->objalm_ptr);
		 err_write(err_str);
		
		
     }
	
	if(dpp->first_scan ==1 )		/* wait for plc new data  */
	{	
	 /*	printf(" \n!!!!!!! %d station %d ,seg->pol_seg_ptr=%d, dpp->polptr=%d.. data=%d\n",lcuno.stnid,lcuno.devid,seg->pol_seg_ptr,dpp->polptr,seg->pol_data[dpp->polptr].pol_data[0]);
	 */
	 
	 	if( (seg->head.ai_size!=0 && dpp->ana_update_flag==seg->ana_update_flag) 
	 		|| (seg->head.rtd_size!=0 && dpp->rtd_update_flag==seg->rtd_update_flag) 
			|| (seg->head.obj_size!=0 && dpp->obj_update_flag==seg->obj_update_flag) 
			|| (seg->head.imp_size!=0 && dpp->imp_update_flag==seg->imp_update_flag)
			|| (seg->head.msg_size!=0 && dpp->sysmsg_update_flag==seg->sysmsg_update_flag)
			|| (seg->head.pol_size!=0 && dpp->polptr==seg->pol_seg_ptr) )
			
		  return;
	 }
	 
	
	
	/**read ai/ao/rtd/pi/dii bitmap/msg change time**/

	if(seg->head.timetag_flag !=0)
	{
		 dpp->ai_time.ms = seg->head.ai_sec_ms % 1000;
		 dpp->ai_time.second = seg->head.ai_sec_ms /1000;
		 dpp->ai_time.minute = seg->head.ai_hour_min & 0xff;
		 dpp->ai_time.hour = (seg->head.ai_hour_min >>8) & 0xff;
		 dpp->ai_time.day = seg->head.ai_month_day &0xff;
		 dpp->ai_time.month = (seg->head.ai_month_day >>8) & 0xff;
		 dpp->ai_time.year = seg->head.ai_year;
	}
	else
		 GetSysTime(&(dpp->ai_time));		

	/***************	process AI  *******************************************************************/
	
	dpp->ai_ntime = time(NULL);
		
	if((dpp->first_scan==1)||((long)(dpp->ai_ntime - dpp->ai_otime) >= ANA_ALLSEND_PERIOD)||
	  		(shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].data_type==ANA_TYPE && shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag ==1 ))
	  		
	  		dpp->ai_allsend_flag = 1;
        
       dpp->ao_ntime = time(NULL);

       if((dpp->first_scan==1)||((long)(dpp->ao_ntime - dpp->ao_otime) >= ANA_ALLSEND_PERIOD)||
                    (shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].data_type==OUTA_TYPE && shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag ==1  ))
                    
                        dpp->ao_allsend_flag =1;
                        
	
	
	if(dpp->ana_update_flag!=seg->ana_update_flag)
        {
        	#ifdef DEBUG
		printf("\ndpp->ana_update_flag=%d,seg->ana_update_flag=%d\n\n",dpp->ana_update_flag,seg->ana_update_flag);
		#endif
		
		dpp->ana_update_flag=seg->ana_update_flag;
		
		/******************** read ai parameters from database **********************/

		ReadDb_ANA(lcuid_tmp,ANA_TYPE,
				seg->head.ai_off,
				seg->head.ai_size,				
				dpp->ana_db);
	
		/***** read AI new value *****/


		ReadNewValue_ANA(lcuid_tmp,ANA_TYPE,
						 seg->head.ai_off,
						 seg->head.ai_size,
						 seg->ai_data,seg->ai_qlt,seg->ai_en,
						 dpp);
                
                if(dpp->first_scan==1 )
                {
                    for(i=0;i<AIALL_SIZE/16+1;i++)  dpp->ai_fault_old[i]=dpp->ai_fault[i];
                    for(i=0;i<AIALL_SIZE/16+1;i++)  dpp->ai_force_old[i]=dpp->ai_force[i];
                }
	
		Process_AI(lcuid_tmp,seg->head.ai_off,seg->head.ai_size,dpp,dpp->ai_time,dpp->ai_allsend_flag);			
	
		/******************** read aic parameters from database **********************/

		ReadDb_ANA(lcuid_tmp,ANA_TYPE,
				seg->head.aic_off,
				seg->head.aic_size,				
				dpp->ana_db);
	
		/***** read aic new value *****/
              /* if(dpp->first_scan==1 )*/
                    
                {memset(aic_qlt,0,(AIC_SIZE/16+1)*2);memset(aic_en,0,(AIC_SIZE/16+1)*2);}
                
		ReadNewValue_ANA(lcuid_tmp,ANA_TYPE,
						 seg->head.aic_off,
						 seg->head.aic_size,
						 seg->aic_data,aic_qlt,aic_en,
						 dpp);
						 
               
		if(dpp->first_scan==1 )
                {
                    for(i=0;i<AIALL_SIZE/16+1;i++)  dpp->ai_fault_old[i]=dpp->ai_fault[i];
                    for(i=0;i<AIALL_SIZE/16+1;i++)  dpp->ai_force_old[i]=dpp->ai_force[i];
                }
	
		Process_AI(lcuid_tmp,seg->head.aic_off,seg->head.aic_size,dpp,dpp->ai_time,dpp->ai_allsend_flag);	
		
              
		/******************** read aicomm parameters from database **********************/

		ReadDb_ANA(lcuid_tmp,ANA_TYPE,
				seg->head.aicomm_off,
				seg->head.aicomm_size,				
				dpp->ana_db);
	
		/***** read aicomm new value *****/

		ReadNewValue_ANA(lcuid_tmp,ANA_TYPE,
						 seg->head.aicomm_off,
						 seg->head.aicomm_size,
						 seg->aicomm_data,seg->aicomm_qlt,seg->aicomm_en,
						 dpp);

                if(dpp->first_scan==1)
                {
                    for(i=0;i<AIALL_SIZE/16+1;i++)  dpp->ai_fault_old[i]=dpp->ai_fault[i];
                    for(i=0;i<AIALL_SIZE/16+1;i++)  dpp->ai_force_old[i]=dpp->ai_force[i];
                }
 			
		Process_AI(lcuid_tmp,seg->head.aicomm_off,seg->head.aicomm_size,dpp,dpp->ai_time,dpp->ai_allsend_flag);	

		
		/******************	send alarm record ******************/

		if(dpp->ai_ptr2!=dpp->ai_ptr1)
		{
			#ifdef DEBUG		
	  		printf("\n1111, now is sending ai change packet:ptr1=%d,ptr2=%d\n",dpp->ai_ptr1,dpp->ai_ptr2);   		
	  		printf("pt=%d,alm_type1=%d\n",dpp->aibuf[dpp->ai_ptr2].ai_object[0].point_id,dpp->aibuf[dpp->ai_ptr2].ai_object[0].status.alm_type);
			#endif
			
	  		dpp->ai_ptr2	= ChangeSend_ANA(lcuid_tmp,dpp->ai_ptr1,dpp->ai_ptr2,dpp->aibuf);

		}	

		if(dpp->aialm_ptr2!=dpp->aialm_ptr1)
		{
			#ifdef DEBUG
	  		printf("\n2222, now is sending aialm change packet:ptr1=%d,ptr2=%d\n",dpp->aialm_ptr1,dpp->aialm_ptr2);  		
	  		printf("pt=%d,alm_type1=%d\n",dpp->aialm[dpp->aialm_ptr2].ai_object[0].point_id,dpp->aialm[dpp->aialm_ptr2].ai_object[0].status.alm_type);
	  		#endif
	  		
	  		dpp->aialm_ptr2 = ChangeSend_ANA(lcuid_tmp,dpp->aialm_ptr1,dpp->aialm_ptr2,dpp->aialm);
		}
		
		
		/***************	process AO *******************************************************************/
 		/*****	read ao parameters from database**/

		ReadDb_ANA(lcuid_tmp,OUTA_TYPE,
					seg->head.ao_off,
					seg->head.ao_size,				
					dpp->ana_db);
	

		/***** read AO value *****/
		ReadNewValue_ANA(lcuid_tmp,OUTA_TYPE,
						 seg->head.ao_off,
						 seg->head.ao_size,
						 seg->ao_data,seg->ao_qlt,seg->ao_en,
						 dpp);
						 
               if(dpp->first_scan==1)
                {
                    for(i=0;i<AO_SIZE/16+1;i++)  dpp->ao_fault_old[i]=dpp->ao_fault[i];
                    for(i=0;i<AO_SIZE/16+1;i++)  dpp->ao_force_old[i]=dpp->ao_force[i];
                }
	
		Process_AO(lcuid_tmp,seg->head.ao_off,seg->head.ao_size,dpp,dpp->ai_time,dpp->ao_allsend_flag);
	
		if(dpp->ao_ptr2!=dpp->ao_ptr1)
		{
	  		/* printf("\nnow is sending ao change packet:ptr1=%d,ptr2=%d\n",dpp->ao_ptr1,dpp->ao_ptr2); */
	
	  		dpp->ao_ptr2	= ChangeSend_ANA(lcuid_tmp,dpp->ao_ptr1,dpp->ao_ptr2,dpp->aobuf);

		}	

		if(dpp->aoalm_ptr2!=dpp->aoalm_ptr1)
		{
	  		/* printf("\nnow is sending aoalm change packet:ptr1=%d,ptr2=%d\n",dpp->aoalm_ptr1,dpp->aoalm_ptr2); */

	 		dpp->aoalm_ptr2 = ChangeSend_ANA(lcuid_tmp,dpp->aoalm_ptr1,dpp->aoalm_ptr2,dpp->aoalm);
		}
		
	
	}


	/*************** AI/AIC/AICOMM  ALL send ***************************************/
	  

	  if(dpp->ai_allsend_flag == 1)
	  {
	        
	    /* printf("\nnow is sending ai_all data\n"); */

	    AllSend_ANA(lcuid_tmp,DPS_ANA_ALL,ANA_TYPE,
				  seg->head.ai_off, 
				  seg->head.ai_size, 
				  dpp->ai_all,				  
				  dpp->ai_time); 
	    
	    AllSend_ANA(lcuid_tmp,DPS_ANA_ALL,ANA_TYPE,
				  seg->head.aic_off, 
				  seg->head.aic_size, 
				  dpp->ai_all,				  
				  dpp->ai_time); 
  
	    AllSend_ANA(lcuid_tmp,DPS_ANA_ALL,ANA_TYPE,
				  seg->head.aicomm_off, 
				  seg->head.aicomm_size, 
				  dpp->ai_all,				  
				  dpp->ai_time); 

	    dpp->ai_otime = dpp->ai_ntime;
	    
	    dpp->ai_allsend_flag = 0;
	    
	    if(seg->ai_all_send_flag ==1) seg->com_ai_all_send_flag =1;
	  }
	  
	 /*******AO_ALL send*****************/

	  
          if(dpp->ao_allsend_flag == 1)
          {
	   
	   /*printf("\nnow is sending ao_all data\n");*/
	   
	   
	   AllSend_ANA(lcuid_tmp,DPS_ANA_ALL,OUTA_TYPE,
				  seg->head.ao_off, 
				  seg->head.ao_size, 
				  dpp->ao_all,		 
				  dpp->ai_time);

	   dpp->ao_otime = dpp->ao_ntime;
           
            dpp->ao_allsend_flag = 0;
	 
	  }

	/***************	process RTD    *******************************/
        dpp->rtd_ntime = time(NULL) ;		
  		
  	if((dpp->first_scan==1)||((long)(dpp->rtd_ntime - dpp->rtd_otime) >= ANA_ALLSEND_PERIOD)||
	  		(shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].data_type==ANA_TYPE && shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag ==1 ) )
	  		
	  		dpp->rtd_allsend_flag = 1;

	if (dpp->rtd_update_flag!=seg->rtd_update_flag)
    	{
		/*printf("\ndpp->rtd_update_flag=%d,seg->rtd_update_flag=%d\n\n",dpp->rtd_update_flag,seg->rtd_update_flag);*/
		
		dpp->rtd_update_flag=seg->rtd_update_flag;
		

		/**read rtd parameters from database**/

		ReadDb_ANA(lcuid_tmp,ANA_TYPE,
				seg->head.rtd_off,
				seg->head.rtd_size,				
				dpp->ana_db);

		/***** read RTD value *****/

		ReadNewValue_ANA(lcuid_tmp,ANA_TYPE,
						 seg->head.rtd_off,
						 seg->head.rtd_size,
						 seg->rtd_data,seg->rtd_qlt,seg->rtd_en,
						 dpp);

                if(dpp->first_scan==1 )
                {
                    for(i=0;i<AIALL_SIZE/16+1;i++)  dpp->ai_fault_old[i]=dpp->ai_fault[i];
                    for(i=0;i<AIALL_SIZE/16+1;i++)  dpp->ai_force_old[i]=dpp->ai_force[i];
                }

		Process_AI(lcuid_tmp,seg->head.rtd_off,seg->head.rtd_size,dpp,dpp->ai_time,dpp->rtd_allsend_flag);
	
		if(dpp->ai_ptr2!=dpp->ai_ptr1)
		{
	  		/*printf("\n RTD now is sending ai change packet:ptr1=%d,ptr2=%d\n",dpp->ai_ptr1,dpp->ai_ptr2); */
	
	  		dpp->ai_ptr2	= ChangeSend_ANA(lcuid_tmp,dpp->ai_ptr1,dpp->ai_ptr2,dpp->aibuf);

		}	

		if(dpp->aialm_ptr2!=dpp->aialm_ptr1)
		{
	  		 /*printf("\n RTD now is sending aialm change packet:ptr1=%d,ptr2=%d\n",dpp->aialm_ptr1,dpp->aialm_ptr2); */

	  		dpp->aialm_ptr2 = ChangeSend_ANA(lcuid_tmp,dpp->aialm_ptr1,dpp->aialm_ptr2,dpp->aialm);
		}
			
	}
	
        /*************** 	RTD_ALL send ***************************************/
	  if( dpp->rtd_allsend_flag == 1 )
	  {
	  	/* printf("\nnow is sending rtd_all data\n"); */

	  	AllSend_ANA(lcuid_tmp,DPS_ANA_ALL,ANA_TYPE,
				  seg->head.rtd_off, 
				  seg->head.rtd_size, 
				  dpp->ai_all,
				  dpp->ai_time);

	  	dpp->rtd_otime = dpp->rtd_ntime;
	  	
	  	dpp->rtd_allsend_flag = 0;
	  	
	 
	  }


		

	/***************	process OBJ    *****************************************************************************/
	
	
	if(dpp->obj_update_flag!=seg->obj_update_flag)
        {
        	#ifdef DEBUG
		printf("\ndpp->obj_update_flag=%d,seg->obj_update_flag=%d\n\n",dpp->obj_update_flag,seg->obj_update_flag);
		#endif
		
		dpp->obj_update_flag=seg->obj_update_flag;
		
		/** read obj parameters from database **/

		ReadDb_OBJ(lcuid_tmp,DEVOBJ_TYPE,
				0,
				seg->head.obj_size,				
				dpp->obj_db);

		/***** read OBJ value *****/

		ReadNewValue_OBJ(lcuid_tmp, 0,
					seg->head.obj_size,
					seg->obj_data,
					dpp);
	 
		Process_OBJ(lcuid_tmp,0,seg->head.obj_size,dpp);
	
	}
	  
	  dpp->obj_ntime = time(NULL);

	  if( dpp->first_scan==1 ||((long)(dpp->obj_ntime - dpp->obj_otime) >= ANA_ALLSEND_PERIOD)||
	  	(shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].data_type==DEVOBJ_TYPE && shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag ==1 ) )
	  {
	    
		/*OBJ_ALL send*/
	  	/* printf("\nnow is sending obj_all data\n"); */

	  	AllSend_OBJ(lcuid_tmp,DPS_DEV_FB_ALL,DEVOBJ_TYPE,
				  0, 
				  seg->head.obj_size, 
				  dpp->obj_all,
				  dpp->ai_time);

	  	dpp->obj_otime = dpp->obj_ntime;
	  	
	 
	  }
		

	/***************	process IMP    **************************************************************/
	
	if(dpp->imp_update_flag!=seg->imp_update_flag)
        {
        	#ifdef DEBUG
        	printf("\ndpp->imp_update_flag=%d,seg->imp_update_flag=%d\n\n",dpp->imp_update_flag,seg->imp_update_flag);
        	#endif
        	
		dpp->imp_update_flag=seg->imp_update_flag;
		
		/**	read impulse parameters from database**/
	
		ReadDb_IMP(lcuid_tmp,IMP_TYPE,
				seg->head.imp_off,
				seg->head.imp_size,				
				dpp->imp_db);

		/***** read PULSE value *****/
		
	       
                    
                   memset(pi_en,0,(PI_SIZE/16+1)*2);	
 	

		ReadNewValue_IMP(lcuid_tmp,
						 seg->head.imp_off,
						 seg->head.imp_size,
						 seg->pi_data,seg->pi_qlt,pi_en,
						 dpp);
		if(dpp->first_scan==1 )
		{
		    for (i=0;i<PI_SIZE/16+1;i++)  
			{
				dpp->pi_fault_old[i] = dpp->pi_fault[i];
				dpp->pi_force_old[i] = dpp->pi_force[i];
			}
		 }	 

		Process_PI(	lcuid_tmp, seg->head.imp_off,seg->head.imp_size,dpp,dpp->ai_time);

		/**	read picomm parameters from database**/
	
		ReadDb_IMP(lcuid_tmp,IMP_TYPE,
				seg->head.impcomm_off,
				seg->head.impcomm_size,				
				dpp->imp_db);

		/***** read PULSE value *****/
		/*if(dpp->first_scan==1)*/
		
                	memset(picomm_en,0,(PI_SIZE/16+1)*2);
                
		ReadNewValue_IMP(lcuid_tmp,
						 seg->head.impcomm_off,
						 seg->head.impcomm_size,
						 seg->picomm_data,seg->picomm_qlt,picomm_en,
						 dpp);
		if(dpp->first_scan==1)
		{
		    for (i=0;i<PI_SIZE/16+1;i++)  
			{
				dpp->pi_fault_old[i] = dpp->pi_fault[i];
				dpp->pi_force_old[i] = dpp->pi_force[i];
			}
		 }	 

		Process_PI(	lcuid_tmp, seg->head.impcomm_off,seg->head.impcomm_size,dpp,dpp->ai_time);

		/**	send alarm record **/

		if(dpp->pi_ptr2!=dpp->pi_ptr1)
		{
			/* printf("\nnow is sending pi change packet:ptr1=%d,ptr2=%d\n",dpp->pi_ptr1,dpp->pi_ptr2);	*/

			dpp->pi_ptr2	= ChangeSend_IMP(lcuid_tmp,dpp->pi_ptr1,dpp->pi_ptr2,dpp->pibuf);
		}
		
		
		if(dpp->pialm_ptr2!=dpp->pialm_ptr1)
		{
	  		/* printf("\nnow is sending pialm change packet:ptr1=%d,ptr2=%d\n",dpp->pialm_ptr1,dpp->pialm_ptr2); */

	  		dpp->pialm_ptr2 = ChangeSend_IMP(lcuid_tmp,dpp->pialm_ptr1,dpp->pialm_ptr2,dpp->pialm);
		}
	}	
	  
	  /******* send piall *******/
	  
	  dpp->imp_ntime =time(NULL);
	  
	  if((dpp->first_scan==1)|| ((long)(dpp->imp_ntime - dpp->imp_otime) >=IMP_ALLSEND_PERIOD)||
	  	( shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].data_type==IMP_TYPE && shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag ==1  ) )
	  {
	    /*PI_ALL send*/
	    /* printf("\nnow is sending imp_all data\n\n"); */

	    AllSend_IMP(lcuid_tmp,DPS_IMP_ALL,IMP_TYPE,
				  seg->head.imp_off,
				  seg->head.imp_size, 
				  dpp->pi_all,			
				  dpp->ai_time);

	    AllSend_IMP(lcuid_tmp,DPS_IMP_ALL,IMP_TYPE,
				  seg->head.impcomm_off,
				  seg->head.impcomm_size, 
				  dpp->pi_all,			
				  dpp->ai_time);
				  
	    dpp->imp_otime = dpp->imp_ntime;
	    
	    
	  }


	/***************	process PARAM    **************************************************************/
		

        if((dpp->first_scan==1) || (dpp->param_update_flag!=seg->param_update_flag)||
        	(shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].data_type==PARAM_TYPE && shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag ==1  ))
	{
		#ifdef DEBUG
		printf("\ndpp->param_update_flag=%d,seg->param_update_flag=%d\n\n",dpp->param_update_flag,seg->param_update_flag);
	        #endif
	       
	        dpp->param_update_flag=seg->param_update_flag;
	        
	        /**read PARAM parameters from database**/
		
		  ReadDb_ANA(lcuid_tmp,PARAM_TYPE,
				seg->head.param_off,
				seg->head.param_size,				
				dpp->ana_db);
	
		/***** read PARAM new value *****/

		ReadNewValue_PARAM(lcuid_tmp,
						 seg->head.param_off,
						 seg->head.param_size,
						 seg->param_data,
						 dpp);
						 
						 

		Process_PARAM(	lcuid_tmp,seg->head.param_off,seg->head.param_size,dpp,dpp->ai_time);
		
	        /*PARAM_ALL send*/

	    	/* printf("\nnow is sending param_all data\n"); */

	    	AllSend_ANA(lcuid_tmp,DPS_ANA_ALL,PARAM_TYPE,
				     seg->head.param_off, 
				    seg->head.param_size, 
				  	  dpp->param_all,				  
				  	   dpp->ai_time);
				  	   
	 }
	
	/***************	process SYSMSG    **************************************************************/
	

	 if(dpp->sysmsg_update_flag!=seg->sysmsg_update_flag)
	 {
		#ifdef DEBUG
		printf("\ndpp->sysmsg_update_flag=%d,seg->sysmsg_update_flag=%d\n\n",dpp->sysmsg_update_flag,seg->sysmsg_update_flag);
		#endif
		
		dpp->sysmsg_update_flag=seg->sysmsg_update_flag;  
		
		/**read message parameters from database**/
		
		  ReadDb_IND(lcuid_tmp,SYSMSG_TYPE,
					seg->head.msg_off,
					seg->head.msg_size,				
					dpp->ind_db);

		  /***** read MSG value *****/
		 /* if(dpp->first_scan==1)*/
		  
		 	{memset(msg_qlt,0,(MSG_SIZE/16+1)*2);memset(msg_en,0,(MSG_SIZE/16+1)*2);}
	
		  ReadNewValue_IND(lcuid_tmp,SYSMSG_TYPE,
						 seg->head.msg_off,
						 seg->head.msg_size,
						 seg->msg_data,msg_qlt,msg_en,
				                 dpp);
				 
		  if(dpp->first_scan==1 )
		  {
		    for (i=0;i<MSG_SIZE/16+1;i++)  
			{
				dpp->msg_old[i] = dpp->msg_new[i];
				dpp->msg_force_old[i] = dpp->msg_force[i];
				dpp->msg_fault_old[i] = dpp->msg_fault[i];
			}			
			/*Process_IND_ALL(lcuno,SYSMSG_TYPE,seg->head.msg_off, seg->head.msg_size,dpp);*/

		  }			

		  if(!dpp->first_scan)
		  {
	      
				Process_IND(lcuid_tmp, SYSMSG_TYPE,seg->head.msg_off, seg->head.msg_size,dpp,dpp->ai_time);
		  
				if(dpp->msg_ptr2!=dpp->msg_ptr1)
				{
					 printf("\nnow is sending msg change packet:ptr1=%d,ptr2=%d\n",dpp->msg_ptr1,dpp->msg_ptr2); 
																
					dpp->msg_ptr2	= ChangeSend_IND(lcuid_tmp,dpp->msg_ptr1,dpp->msg_ptr2,dpp->msgbuf);
                       
				}
		  }
	 }
	
	 dpp->msg_ntime =time(NULL);
	 
	  /*MSG_ALL send*/
	  
	  if((dpp->first_scan==1)||((long)(dpp->msg_ntime - dpp->msg_otime) >= IND_ALLSEND_PERIOD)||
	  	(shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].data_type==SYSMSG_TYPE && shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag ==1  ) )
	  {
	    
	    	/* printf("\nnow is sending LCU_MSG_ALL data from lcu \n"); */

		Process_IND_ALL(lcuid_tmp,SYSMSG_TYPE,seg->head.msg_off, seg->head.msg_size,dpp);

	    	AllSend_IND(lcuid_tmp,SYSMSG_TYPE,
				  seg->head.msg_off,
				  seg->head.msg_size,
				  dpp->msg_all,
				  dpp->ai_time);

	   	dpp->msg_otime = dpp->msg_ntime;
	   	
	   	if(-1 != GetEntryNumById(lcuid_tmp.stnid,lcuno.dev_idx,SYSMSG_TYPE,&msg_num))
	   	{
	   			if(msg_num>seg->head.msg_size)
	   			{
	   				/* printf("\nnow is sending SYS_MSG_ALL data from database \n"); */
	   		
		  			ReadDb_IND(lcuid_tmp,SYSMSG_TYPE,
								seg->head.msg_off+seg->head.msg_size,
							        msg_num-seg->head.msg_size,				
							        dpp->ind_db);
					
	    				Process_SYS_MSG(lcuid_tmp, seg->head.msg_off+seg->head.msg_size,msg_num-seg->head.msg_size,dpp,dpp->ai_time);
	    		
	    				AllSend_IND(lcuid_tmp,SYSMSG_TYPE,
				  				seg->head.msg_off+seg->head.msg_size,
				  				msg_num-seg->head.msg_size,
				  				dpp->msg_all,
				 				dpp->ai_time);
				  }
		}
				    
	  }
	
	/***************	process DII BITMAP    **************************************************************/		 
	
	/*printf("SOE_off=%d,soe_size=%d\n",seg->head.soe_off,seg->head.soe_size);*/
	
	/**read DII parameters from database**/
	
         ReadDb_IND(lcuid_tmp,SOE_TYPE,
				seg->head.soe_off,
				seg->head.soe_size,				
				dpp->ind_db);	 	 

	/***** read DII bitmap value *****/

         ReadNewValue_IND(lcuid_tmp,SOE_TYPE,
					 seg->head.soe_off,
					 seg->head.soe_size,
					 seg->soe_data,seg->soe_qlt,seg->soe_en,
					 dpp);

	if(dpp->first_scan==1 )
	{         
	        for (i=0;i<DII_SIZE/16+1;i++)  
			{
				dpp->dii_old[i] = dpp->dii_new[i];
				dpp->dii_fault_old[i] = dpp->dii_fault[i];
				dpp->dii_force_old[i] = dpp->dii_force[i];
			}
	          	
			dpp->soeallsend_flag =0;
	}
		  
	  	
	dpp->dii_ntime = time(NULL);
	  	 
	/*DII_ALL send*/

	if((dpp->first_scan==1)|| ((long)(dpp->dii_ntime - dpp->dii_otime) >= DII_ALLSEND_PERIOD)||
		(shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].data_type==SOE_TYPE && shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag ==1  ))
	{

	    	/* printf("\nnow is sending dii_all data\n\n"); */
	    	
	  	Process_IND_ALL(lcuid_tmp,SOE_TYPE, seg->head.soe_off,seg->head.soe_size,dpp);
	  	
	    	AllSend_IND(lcuid_tmp,SOE_TYPE,
					  seg->head.soe_off, 
					  seg->head.soe_size, 
					  dpp->dii_all,	  
					  dpp->ai_time);


	    	dpp->dii_otime = dpp->dii_ntime;  
	    	
	 }
		  
		  
	 Process_DII(lcuid_tmp,seg->head.soe_off,seg->head.soe_size,dpp,dpp->ai_time);
	 	  
	 if(dpp->dii_ptr2!=dpp->dii_ptr1)
	 {
	  	 /*printf("\nnow is sending dii change packet:ptr1=%d,ptr2=%d\n",dpp->dii_ptr1,dpp->dii_ptr2); */

	  	dpp->dii_ptr2	= ChangeSend_IND(lcuid_tmp,dpp->dii_ptr1,dpp->dii_ptr2,dpp->diibuf);
	 }

	if(dpp->soeallsend_flag == 1)	/*DII CHANGE SEND IN ALLSEND FORMAT FLAG,WRITE IN PROCESS_DII.C*/
	{
	  	dpp->soeallsend_flag =0;
	  	
	    	AllSend_SOE(lcuid_tmp,dpp->dii_num,dpp->dii_object,dpp->ai_time);
	  	 	
	}

 	  
  	/***************	process SOE ALARM RECORD BUFFER    **************************************************************/
		
	#ifdef DEBUG
	printf("\naa..dpp->soeptr=%d,seg->soealm_ptr=%d\n\n",dpp->soeptr,seg->soealm_ptr);
	#endif

	soealm_head = seg->soealm_ptr;
	
	if(seg->soealm_ptr < seg->head.soealm_max )		
	{		
		while(dpp->soeptr!= soealm_head )
		{
			/* printf("****************************************************\n"); 
			
			printf("\n bb...dpp->soeptr=%d,soealm_head =%d \n\n",dpp->soeptr,soealm_head); */
			
			Process_SOE(lcuid_tmp,
						dpp->soeptr,
						//seg->head.soealm_max,
						seg->head.soe_size,
						seg->soealm_buf, 
						dpp);

			dpp->soeptr++;
		
			if(dpp->soeptr >= seg->head.soealm_max)	
				
				dpp->soeptr = 0;
		}
			
	}
	
	if(dpp->soe_ptr2!=dpp->soe_ptr1)
	{
	  	/* printf("\nnow is sending SOEALM  change packet:ptr1=%d,ptr2=%d\n",dpp->soe_ptr1,dpp->soe_ptr2); */

	  	dpp->soe_ptr2	= ChangeSend_SOE(lcuid_tmp,dpp->soe_ptr1,dpp->soe_ptr2,dpp->soebuf);
	}





	/***************	process  POL / DOUT bitmap    **************************************************************/
	
       if(dpp->first_scan == 1 ) 
       { 		 
       			/*dpp->polptr = seg->pol_seg_ptr;
       			
       			if(dpp->polptr == 0) ptr = MAX_POL_BUF-1;
			 
			else ptr = dpp->polptr-1;*/
			
			ptr= dpp->polptr;
		       
			if(seg->pol_data[ptr].pol_timetag_flag!=0)
			{
				dpp->di_time.ms		= seg->pol_data[ptr].pol_sec_ms % 1000;
				dpp->di_time.second	= seg->pol_data[ptr].pol_sec_ms /1000;
				dpp->di_time.minute	= seg->pol_data[ptr].pol_hour_min & 0xff;
				dpp->di_time.hour	= (seg->pol_data[ptr].pol_hour_min >>8) & 0xff;
				dpp->di_time.day	= seg->pol_data[ptr].pol_month_day &0xff;
				dpp->di_time.month	= (seg->pol_data[ptr].pol_month_day >>8) & 0xff;
				dpp->di_time.year	= seg->pol_data[ptr].pol_year;		
			}
			else
				GetSysTime(&(dpp->di_time));
				
			/**********  di *************/
			
		 	/*printf(" \n##############2 %d station ,%d lcu,dpp->firstscan=1... ptr=%d.. data=%d\n",lcuno.stnid,lcuno.devid,ptr,seg->pol_data[ptr].pol_data[0]);*/
		
			ReadDb_IND(lcuid_tmp,POL_TYPE,
						seg->head.pol_off,
						seg->head.pol_size,				
						dpp->ind_db);

			ReadNewValue_IND(lcuid_tmp,POL_TYPE,
							 seg->head.pol_off,
							 seg->head.pol_size,
							 seg->pol_data[ptr].pol_data,seg->pol_data[ptr].pol_qlt,seg->pol_data[ptr].pol_en,
							 dpp);

			Process_IND_ALL(lcuid_tmp,POL_TYPE, seg->head.pol_off, seg->head.pol_size,dpp);

			/**********  dic *************/
			
			ReadDb_IND(lcuid_tmp,POL_TYPE,
						seg->head.polc_off,
						seg->head.polc_size,				
						dpp->ind_db);
						
                        
                        memset(polc_qlt,0,(DIC_SIZE/16+1)*2);memset(polc_en,0,(DIC_SIZE/16+1)*2); 
                        
			ReadNewValue_IND(lcuid_tmp,POL_TYPE,
							 seg->head.polc_off,
							 seg->head.polc_size,
							 seg->pol_data[ptr].polc_data,polc_qlt,polc_en,
							 dpp);

			Process_IND_ALL(lcuid_tmp,POL_TYPE, seg->head.polc_off, seg->head.polc_size,dpp);

			/********** commdi **********/
			
			ReadDb_IND(lcuid_tmp,POL_TYPE,
						seg->head.polcomm_off,
						seg->head.polcomm_size,				
						dpp->ind_db);

			ReadNewValue_IND(lcuid_tmp,POL_TYPE,
							 seg->head.polcomm_off,
							 seg->head.polcomm_size,
							 seg->pol_data[ptr].polcomm_data,seg->pol_data[ptr].polcomm_qlt,seg->pol_data[ptr].polcomm_en,
							 dpp);

			Process_IND_ALL(lcuid_tmp,POL_TYPE, seg->head.polcomm_off, seg->head.polcomm_size,dpp);

			
			for (i=0;i<DIN_SIZE/16+1;i++) 
			{
				dpp->pol_old[i] = dpp->pol_new[i];
				dpp->pol_fault_old[i] = dpp->pol_fault[i];
				dpp->pol_force_old[i] = dpp->pol_force[i];
			}
	       
			ReadDb_IND(lcuid_tmp,OUTS_TYPE,
						seg->head.dout_off,
						seg->head.dout_size,			
						dpp->ind_db);	

			ReadNewValue_IND(lcuid_tmp,OUTS_TYPE,
							 seg->head.dout_off,
							 seg->head.dout_size,
							 seg->pol_data[ptr].dout_data,seg->pol_data[ptr].dout_qlt,seg->pol_data[ptr].dout_en,
							 dpp);

			Process_IND_ALL(lcuid_tmp,OUTS_TYPE, seg->head.dout_off, seg->head.dout_size,dpp);

			for (i=0;i<DOUT_SIZE/16+1;i++)  dpp->dout_old[i] = dpp->dout_new[i];
			for (i=0;i<DOUT_SIZE/16+1;i++)  dpp->dout_force_old[i] = dpp->dout_force[i];
			for (i=0;i<DOUT_SIZE/16+1;i++)  dpp->dout_fault_old[i] = dpp->dout_fault[i];

			   		      
       }
       
	
	/*printf("seg->head.pol_share_max=%d\n\n",seg->head.pol_share_max);*/
	
	
	#ifdef DEBUG
 	printf("\naaaaa..dpp->polptr=%d,seg->pol_seg_ptr=%d\n\n",dpp->polptr,seg->pol_seg_ptr);
 	#endif
 	
 	 
		
		
 	pol_head = seg->pol_seg_ptr ;
 		
	if(pol_head < MAX_POL_BUF)
	{
		while( dpp->polptr!= pol_head)
		{
			/*printf("\nbbbbb..dpp->polptr=%d,pol_head=%d\n\n",dpp->polptr,pol_head);*/	
			
			ptr = dpp->polptr;
			
			/*printf(" \n##############3 %d station %d ,ptr=%d.. data=%d\n",lcuno.stnid,lcuno.devid,dpp->polptr,seg->pol_data[dpp->polptr].pol_data[0]);*/
			
			/**read  bitmap change time **/

			if(seg->pol_data[ptr].pol_timetag_flag!=0)  
			{
				dpp->di_time.ms		= seg->pol_data[ptr].pol_sec_ms % 1000;
				dpp->di_time.second	= seg->pol_data[ptr].pol_sec_ms /1000;
				dpp->di_time.minute	= seg->pol_data[ptr].pol_hour_min & 0xff;
				dpp->di_time.hour	= (seg->pol_data[ptr].pol_hour_min >>8) & 0xff;
				dpp->di_time.day	= seg->pol_data[ptr].pol_month_day &0xff;
				dpp->di_time.month	= (seg->pol_data[ptr].pol_month_day >>8) & 0xff;
				dpp->di_time.year	= seg->pol_data[ptr].pol_year;		
			}
			else
				GetSysTime(&(dpp->di_time));

			/*printf("pol_off=%d,pol_size=%d\n",seg->head.pol_off,seg->head.pol_size);*/

			/**read pol parameters from database**/

			ReadDb_IND(lcuid_tmp,POL_TYPE,
						seg->head.pol_off,
						seg->head.pol_size,				
						dpp->ind_db);

			/***** read POL value *****/
			
			ReadNewValue_IND(lcuid_tmp,POL_TYPE,
							 seg->head.pol_off,
							 seg->head.pol_size,
							 seg->pol_data[ptr].pol_data,seg->pol_data[ptr].pol_qlt,seg->pol_data[ptr].pol_en,
							 dpp);

			Process_IND(lcuid_tmp,POL_TYPE,seg->head.pol_off,seg->head.pol_size,dpp,dpp->di_time);

			if(dpp->din_ptr2!=dpp->din_ptr1)
			{
				/*printf("\nnow is sending pol change packet:ptr1=%d,ptr2=%d\n",dpp->din_ptr1,dpp->din_ptr2); */

				dpp->din_ptr2	= ChangeSend_IND(lcuid_tmp,dpp->din_ptr1,dpp->din_ptr2,dpp->dinbuf);
			}

			/**********  dic *************/
			
			ReadDb_IND(lcuid_tmp,POL_TYPE,
						seg->head.polc_off,
						seg->head.polc_size,				
						dpp->ind_db);
                        
                        memset(polc_qlt,0,(DIC_SIZE/16+1)*2);memset(polc_en,0,(DIC_SIZE/16+1)*2); 

			ReadNewValue_IND(lcuid_tmp,POL_TYPE,
							 seg->head.polc_off,
							 seg->head.polc_size,
							 seg->pol_data[ptr].polc_data,polc_qlt,polc_en,
							 dpp);

			Process_IND(lcuid_tmp,POL_TYPE,seg->head.polc_off, seg->head.polc_size,dpp,dpp->di_time);

			if(dpp->din_ptr2!=dpp->din_ptr1)
			{
				/*printf("\nnow is sending pol change packet:ptr1=%d,ptr2=%d\n",dpp->din_ptr1,dpp->din_ptr2); */

				dpp->din_ptr2	= ChangeSend_IND(lcuid_tmp,dpp->din_ptr1,dpp->din_ptr2,dpp->dinbuf);
			}

			/********** commdi **********/
			
			ReadDb_IND(lcuid_tmp,POL_TYPE,
						seg->head.polcomm_off,
						seg->head.polcomm_size,				
						dpp->ind_db);

			ReadNewValue_IND(lcuid_tmp,POL_TYPE,
							 seg->head.polcomm_off,
							 seg->head.polcomm_size,
							 seg->pol_data[ptr].polcomm_data,seg->pol_data[ptr].polcomm_qlt,seg->pol_data[ptr].polcomm_en,
							 dpp);

			
			Process_IND(lcuid_tmp,POL_TYPE,seg->head.polcomm_off, seg->head.polcomm_size,dpp,dpp->di_time);
			
			if(dpp->din_ptr2!=dpp->din_ptr1)
			{
				/*printf("\nnow is sending pol change packet:ptr1=%d,ptr2=%d\n",dpp->din_ptr1,dpp->din_ptr2); */

				dpp->din_ptr2	= ChangeSend_IND(lcuid_tmp,dpp->din_ptr1,dpp->din_ptr2,dpp->dinbuf);
			}
			
			
			/**	read dout parameters from database**/
			
			ReadDb_IND(lcuid_tmp,OUTS_TYPE,
						seg->head.dout_off,
						seg->head.dout_size,			
						dpp->ind_db);	

			/***** read DOUT value *****/
			
			ReadNewValue_IND(lcuid_tmp,OUTS_TYPE,
							 seg->head.dout_off,
							 seg->head.dout_size,
							 seg->pol_data[ptr].dout_data,seg->pol_data[ptr].dout_qlt,seg->pol_data[ptr].dout_en,
							 dpp);

			Process_IND(lcuid_tmp,OUTS_TYPE,seg->head.dout_off,seg->head.dout_size,dpp,dpp->di_time);
			
			if(dpp->dout_ptr2!=dpp->dout_ptr1)
			{
	     			/* printf("\nnow is sending dout change packet:ptr1=%d,ptr2=%d\n",dpp->dout_ptr1,dpp->dout_ptr2); */
	     				      				      
	     			dpp->dout_ptr2 = ChangeSend_IND(lcuid_tmp,dpp->dout_ptr1,dpp->dout_ptr2,dpp->doutbuf);
			}
	
			dpp->polptr++;

			if(dpp->polptr == MAX_POL_BUF)	dpp->polptr=0;
				
				
		}

	}
	
	dpp->pol_ntime = time(NULL);
	
	if((dpp->first_scan==1)||((long)(dpp->pol_ntime - dpp->pol_otime) >= IND_ALLSEND_PERIOD)||
		(shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].data_type==POL_TYPE && shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag ==1 ) )
	{	
		/* printf("\nnow is sending pol_all data\n\n");	*/
	    
	  	/*DIN_ALL send*/

		Process_IND_ALL(lcuid_tmp,POL_TYPE, seg->head.pol_off, seg->head.pol_size,dpp);
	  	

	 	 AllSend_IND(lcuid_tmp,POL_TYPE,
				  seg->head.pol_off,
				  seg->head.pol_size,
				  dpp->din_all,
				  dpp->di_time);
  
	 	/*DIC_ALL send*/ 
		Process_IND_ALL(lcuid_tmp,POL_TYPE, seg->head.polc_off, seg->head.polc_size,dpp);

	  	AllSend_IND(lcuid_tmp,POL_TYPE,
				  seg->head.polc_off,
				  seg->head.polc_size,
				  dpp->din_all,
				  dpp->di_time);



	  	/*DICOMM_ALL send*/
	  	Process_IND_ALL(lcuid_tmp,POL_TYPE, seg->head.polcomm_off, seg->head.polcomm_size,dpp);

		AllSend_IND(lcuid_tmp,POL_TYPE,
				  seg->head.polcomm_off,
				  seg->head.polcomm_size,
				  dpp->din_all,
				  dpp->di_time);


	   	dpp->pol_otime = dpp->pol_ntime;
	   
	   	if(shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag ==1 )
	   	{
	   	 	 seg->com_pol_all_send_flag =1;
	   	}

	  }

	dpp->dout_ntime = time(NULL);
	
	if((dpp->first_scan==1)||((long)(dpp->dout_ntime - dpp->dout_otime) >= IND_ALLSEND_PERIOD)||
		(shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].data_type==OUTS_TYPE && shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag ==1 ) )
	{
	    
	  /*DOUT_ALL send*/
	   /* printf("\nnow is sending dout_all data\n\n"); */
		
		Process_IND_ALL(lcuid_tmp,OUTS_TYPE, seg->head.dout_off, seg->head.dout_size,dpp);

	   AllSend_IND(lcuid_tmp,OUTS_TYPE,
				  seg->head.dout_off,
				  seg->head.dout_size,
				  dpp->dout_all,
				  dpp->di_time);
	  
	   dpp->dout_otime = dpp->dout_ntime;
	   
	   
	  }
	  
	  	
	/***************	process OBJ ALARM RECORD BUFFER    **************************************************************/

	ptr=seg->pol_seg_ptr;  /*add by ycx 07.3.5*/
	
	if(seg->pol_data[ptr].pol_timetag_flag==0)  	{ObjTimeFlag=0;GetSysTime(&Obj_time);}
	
	else	ObjTimeFlag=1;

	objalm_head = seg->objalm_ptr;

	if(dpp->first_scan == 1 )    
	{ 
		char	err_str[128];
		
	/*	objalm_head = seg->objalm_ptr;
		dpp->objptr = objalm_head;*/
		sprintf(err_str, "dpp first scan : seg->objalm_ptr=%d,dpp->objptr=%d ", seg->objalm_ptr,dpp->objptr);
		err_write(err_str);
	}
		
	if(seg->objalm_ptr < seg->head.objalm_max)		
	{		
		char err_str[128]; 
		
		if( objalm_head > dpp->objptr)
		{	
			if( (objalm_head - dpp->objptr >=2)  )
			{
				sprintf(err_str, "too much objalm:dpp_tail =%d, dpp_head = %d ",dpp->objptr,objalm_head);
				err_write(err_str);
			}
		}
		else if( objalm_head < dpp->objptr)
		{
			if(seg->head.objalm_max + objalm_head - dpp->objptr >=2)
			{
				sprintf(err_str, "too much objalm:dpp_tail =%d,dpp_head = %d ",dpp->objptr,objalm_head);
				err_write(err_str);
			}
			
		}
		
		while(dpp->objptr!= objalm_head )
		{
			Process_OBJAlm(lcuid_tmp,
						dpp->objptr,
						seg->head.objalm_max,
						seg->objalm_buf, 
						dpp,ObjTimeFlag,Obj_time);

			dpp->objptr++;
		
			if(dpp->objptr >= seg->head.objalm_max)	
				
				dpp->objptr = 0;
		}
			
	}
	
	if(dpp->obj_ptr2!=dpp->obj_ptr1)
	{
	      /*  printf("\nnow is sending ObjAlm change packet:ptr1=%d,ptr2=%d\n",dpp->obj_ptr1,dpp->obj_ptr2); */

	  	dpp->obj_ptr2	= ChangeSend_OBJ(lcuid_tmp,dpp->obj_ptr1,dpp->obj_ptr2,dpp->objbuf);
	}


	  
 
/*******************************	process  SENDATA ana & pol    **************************************************************/	
	//modified by wzg 20120319 for ntime[lcuno.stn_idx][lcuno.dev_idx] to ntime, otime[lcuno.stn_idx][lcuno.dev_idx] to otime
	if(dpp->dpp_sendata_flag ==1)	
	{ 
		dpp->dpp_sendata.ntime = time(NULL);
		
		if((long)(dpp->dpp_sendata.ntime - dpp->dpp_sendata.otime) >= DATA_DOWNSEND_PERIOD)
		{
			Process_Sendata(lcuid_tmp,&dpp->dpp_sendata,sendseg);
		
			dpp->dpp_sendata.otime = dpp->dpp_sendata.ntime;
		}
			
			
	}

	if( shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag==1 )  shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag=0;
	
	
	if(dpp->first_scan ==1) dpp->first_scan=0;
}

/*modified by wzg 20070512*/
void ProcessData2(LCUID_INC_IDX lcuno, DPP_DATA *dpp)
{

	GULONG pt_num,i;

	DATE_TIME tm;
	
	LCUID   lcuid_tmp;//cxs
	//LCUID   lcuid_idx_tmp;//cxs
	
	lcuid_tmp.stnid=lcuno.lcu_id.stnid;//cxs wzg 20120319
	lcuid_tmp.devid=lcuno.lcu_id.devid;//cxs


	GetSysTime(&(tm));

	if(-1 != GetEntryNumById(lcuno.lcu_id.stnid,lcuno.lcu_id.devid,ANA_TYPE,&pt_num))
   	{
    	if(pt_num>0)
		{
			ReadDb_ANA(lcuid_tmp, ANA_TYPE, 0, pt_num, dpp->ana_db);

			for(i=0;i<pt_num;i++)
			{
				dpp->ai_all[i].data = dpp->ana_db[i].data;
				dpp->ai_all[i].status = dpp->ana_db[i].status;
				if(!dpp->ana_db[i].ioattr.sysVirtual)
					dpp->ai_all[i].status.chan_fail  = 1 ;
			}

			AllSend_ANA(lcuid_tmp,DPS_ANA_ALL,ANA_TYPE,
					  0, 
					  pt_num, 
					  dpp->ai_all,				  
					  tm); 
			
		}
			  
	}

	if(-1 != GetEntryNumById(lcuid_tmp.stnid,lcuid_tmp.devid,IMP_TYPE,&pt_num))
   	{
		if(pt_num>0)
		{
			ReadDb_IMP(lcuid_tmp, IMP_TYPE, 0, pt_num, dpp->imp_db);

			for(i=0;i<pt_num;i++)
			{
				dpp->pi_all[i].data = dpp->imp_db[i].imp_accm;
				dpp->pi_all[i].status = dpp->imp_db[i].status;
				dpp->pi_all[i].status.option  = RW_IMP_ACCUM ;
				if(!dpp->imp_db[i].ioattr.sysVirtual)
					dpp->pi_all[i].status.chan_fail  = 1 ;
			}

			AllSend_IMP(lcuid_tmp,DPS_IMP_ALL,IMP_TYPE,
					  0,
					  pt_num, 
					  dpp->pi_all,			
					  tm);
		}
			 
	}

	if(-1 != GetEntryNumById(lcuid_tmp.stnid,lcuid_tmp.devid,SOE_TYPE,&pt_num))
   	{
    	if(pt_num>0)
		{	
			ReadDb_IND(lcuid_tmp,SOE_TYPE,
						0,
						pt_num,				
						dpp->ind_db);

			for(i=0;i<pt_num;i++)
			{
				dpp->dii_all[i]= dpp->ind_db[i].status;
				if(!dpp->ind_db[i].ioattr.sysVirtual)
					dpp->dii_all[i].chan_fail  = 1 ;
			}

    		AllSend_IND(lcuid_tmp,SOE_TYPE,
			  				0,
			  				pt_num,
			  				dpp->dii_all,
			 				tm);
		}
			  
	}

	if(-1 != GetEntryNumById(lcuid_tmp.stnid,lcuid_tmp.devid,POL_TYPE,&pt_num))
   	{
    	if(pt_num>0)
		{
			ReadDb_IND(lcuid_tmp,POL_TYPE,
						0,
						pt_num,				
						dpp->ind_db);

			for(i=0;i<pt_num;i++)
			{
				dpp->din_all[i]= dpp->ind_db[i].status;
				if(!dpp->ind_db[i].ioattr.sysVirtual)
					dpp->din_all[i].chan_fail  = 1 ;
			}

			AllSend_IND(lcuid_tmp,POL_TYPE,
			  				0,
			  				pt_num,
			  				dpp->din_all,
			 				tm);
		}
			  
	}


	if(-1 != GetEntryNumById(lcuid_tmp.stnid,lcuid_tmp.devid,SYSMSG_TYPE,&pt_num))
   	{
    		if(pt_num>0)
		{
			ReadDb_IND(lcuid_tmp,SYSMSG_TYPE,
						0,
						pt_num,				
						dpp->ind_db);
						
			/*printf("pt_num=%d\n",pt_num);*/

			for(i=0;i<pt_num;i++)
			{
				dpp->msg_all[i]= dpp->ind_db[i].status;
				if(!dpp->ind_db[i].ioattr.sysVirtual)
					dpp->msg_all[i].chan_fail  = 1 ;			
			}

    			AllSend_IND(lcuid_tmp,SYSMSG_TYPE,
			  				0,
			  				pt_num,
			  				dpp->msg_all,
			 				tm);
		}
	}

	if(-1 != GetEntryNumById(lcuid_tmp.stnid,lcuid_tmp.devid,OUTA_TYPE,&pt_num))
   	{
    	if(pt_num>0)
		{
			ReadDb_ANA(lcuid_tmp, OUTA_TYPE, 0, pt_num, dpp->ana_db);

			for(i=0;i<pt_num;i++)
			{
				dpp->ao_all[i].data = dpp->ana_db[i].data;
				dpp->ao_all[i].status = dpp->ana_db[i].status;
				if(!dpp->ana_db[i].ioattr.sysVirtual)
					dpp->ao_all[i].status.chan_fail  = 1 ;
			}

			AllSend_ANA(lcuid_tmp,DPS_ANA_ALL,OUTA_TYPE,
					  0, 
					  pt_num, 
					  dpp->ao_all,				  
					  tm); 
		}
			  
	}

	if(-1 != GetEntryNumById(lcuid_tmp.stnid,lcuid_tmp.devid,OUTS_TYPE,&pt_num))
   	{
    	if(pt_num >0)
		{
			ReadDb_IND(lcuid_tmp,OUTS_TYPE,
						0,
						pt_num,				
						dpp->ind_db);

			for(i=0;i<pt_num;i++)
			{
				dpp->dout_all[i]= dpp->ind_db[i].status;
				if(!dpp->ind_db[i].ioattr.sysVirtual)
					dpp->dout_all[i].chan_fail  = 1 ;
			}

    			AllSend_IND(lcuid_tmp,OUTS_TYPE,
			  				0,
			  				pt_num,
			  				dpp->dout_all,
			 				tm);
		}
	}

	/*modified by ycx 070516
	if(-1 != GetEntryNumById(lcuno.stnid,lcuno.devid,PARAM_TYPE,&pt_num))
   	{
    	if(pt_num >0)
		{
			ReadDb_ANA(lcuno, PARAM_TYPE, 0, pt_num, dpp->ana_db);

			for(i=0;i<pt_num;i++)
			{
				dpp->param_all[i].data = dpp->ana_db[i].data;
				dpp->param_all[i].status = dpp->ana_db[i].status;
				if(!dpp->ana_db[i].ioattr.sysVirtual)
					dpp->param_all[i].status.chan_fail  = 1 ;
			}

		    AllSend_ANA(lcuno,DPS_ANA_ALL,PARAM_TYPE,
							0, 
							pt_num, 
			  				dpp->param_all,				  
			  				tm);
		}
	} */	
}

/*
void ProcessData2(LCUID lcuno, DPP_DATA *dpp)
{

	GULONG pol_num,ai_num,ao_num,soe_num,msg_num,dout_num,imp_num,param_num,i;

	DATE_TIME tm;
			
		GetSysTime(&(tm));

		if(-1 != GetEntryNumById(lcuno.stnid,lcuno.devid,ANA_TYPE,&ai_num))
	   	{
	    		if(ai_num>0)
			{

				for(i=0;i<ai_num;i++) dpp->ai_all[i].status.chan_fail  = 1 ;

				AllSend_ANA(lcuno,DPS_ANA_ALL,ANA_TYPE,
						  0, 
						  ai_num, 
						  dpp->ai_all,				  
						  tm); 
				
			}
				  
		}

		if(-1 != GetEntryNumById(lcuno.stnid,lcuno.devid,IMP_TYPE,&imp_num))
	   	{
			if(imp_num>0)
			{
				for(i=0;i<imp_num;i++) dpp->pi_all[i].status.chan_fail  = 1 ;

				AllSend_IMP(lcuno,DPS_IMP_ALL,IMP_TYPE,
						  0,
						  imp_num, 
						  dpp->pi_all,			
						  tm);
			}
				 
		}

		if(-1 != GetEntryNumById(lcuno.stnid,lcuno.devid,SOE_TYPE,&soe_num))
	   	{
	    		if(soe_num>0)
			{	
				for(i=0;i<soe_num;i++) dpp->dii_all[i].chan_fail  = 1 ;

	    			AllSend_IND(lcuno,SOE_TYPE,
				  				0,
				  				soe_num,
				  				dpp->dii_all,
				 				tm);
			}
				  
		}

		if(-1 != GetEntryNumById(lcuno.stnid,lcuno.devid,POL_TYPE,&pol_num))
	   	{
	    		if(pol_num>0)
			{
	    			for(i=0;i<pol_num;i++) dpp->din_all[i].chan_fail  = 1 ;

				AllSend_IND(lcuno,POL_TYPE,
				  				0,
				  				pol_num,
				  				dpp->din_all,
				 				tm);
			}
				  
		}


		if(-1 != GetEntryNumById(lcuno.stnid,lcuno.devid,SYSMSG_TYPE,&msg_num))
	   	{
	    		if(msg_num>0)
			{
					for(i=0;i<msg_num;i++) dpp->msg_all[i].chan_fail  = 1 ;

	    				AllSend_IND(lcuno,SYSMSG_TYPE,
				  				0,
				  				640,
				  				dpp->msg_all,
				 				tm);
			}
		}

		if(-1 != GetEntryNumById(lcuno.stnid,lcuno.devid,OUTA_TYPE,&ao_num))
	   	{
	    		if(ao_num>0)
			{
				for(i=0;i<ao_num;i++) dpp->ao_all[i].status.chan_fail  = 1 ;

				AllSend_ANA(lcuno,DPS_ANA_ALL,OUTA_TYPE,
						  0, 
						  ao_num, 
						  dpp->ao_all,				  
						  tm); 
			}
				  
		}

		if(-1 != GetEntryNumById(lcuno.stnid,lcuno.devid,OUTS_TYPE,&dout_num))
	   	{
	    		if(dout_num >0)
			{
				for(i=0;i<dout_num;i++) dpp->dout_all[i].chan_fail  = 1 ;

	    			AllSend_IND(lcuno,OUTS_TYPE,
				  				0,
				  				dout_num,
				  				dpp->dout_all,
				 				tm);
			}
		}

	/*modified by ycx 070420
	if(-1 != GetEntryNumById(lcuno.stnid,lcuno.devid,PARAM_TYPE,&param_num))
	   	{
	    		if(param_num >0)
			{
				for(i=0;i<param_num;i++) dpp->param_all[i].status.chan_fail  = 1 ;

			    	AllSend_ANA(lcuno,DPS_ANA_ALL,PARAM_TYPE,
								0, 
								param_num, 
				  				dpp->param_all,				  
				  				tm);
			}
		}*/

		
		
	

  
/*}	*/

