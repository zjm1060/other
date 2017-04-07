/************************************************************************************************************************************************

FILENAME:	dpplib.c

FUNCTION:	

		main thread functions of dpp:scada()

		initialization function of dpp_data:init()



VERSION:	v4.0a
OPERATE	SYSTEM:	window	2000,	Unix
HISTORY:	2006.04.20	ycx
************************************************************************************************************************************************/



#include "../../../dms/v4.0a/inc/dms_com.h"

#include "../../../dps/v4.0a/inc/dps_rec_def.h"

#include "../../../dps/v4.0a/inc/dps_type_def.h"

#include "../../../dps/v4.0a/inc/ipc_common.h"

#include "../../../lan/v4.0a/inc/lan_common.h"

#include "../../../plc/v4.0a/inc/lcu_data.h"

#include "../../../dpp/v4.0a/inc/dppdef.h"

#include "../../../dpp/v4.0a/inc/dpp_functions.h"



void init(DPP_DATA *dpp)

{

  	unsigned  short	i;



 	for(i=0;i<MSG_SIZE/16+1;i++) 

	{dpp->msg_new[i]=0; dpp->msg_force[i]=0;dpp->msg_fault[i]=0;dpp->msg_old[i]=0; dpp->msg_force_old[i]=0;dpp->msg_force_old[i]=0;}



	for(i=0;i<DII_SIZE/16+1;i++) 

	{dpp->dii_new[i]=0; dpp->dii_force[i]=0;dpp->dii_fault[i]=0;dpp->dii_old[i]=0;}



	for(i=0;i<DIN_SIZE/16+1;i++) 

	{dpp->pol_new[i]=0; dpp->pol_force[i]=0;dpp->pol_fault[i]=0;dpp->pol_old[i]=0; dpp->pol_force_old[i]=0;dpp->pol_force_old[i]=0;}



	for(i=0;i<DOUT_SIZE/16+1;i++) 

	{dpp->dout_new[i]=0; dpp->dout_force[i]=0;dpp->dout_fault[i]=0;dpp->dout_old[i]=0; dpp->dout_force_old[i]=0;dpp->dout_force_old[i]=0;}



	for(i=0;i<AIALL_SIZE/16+1;i++) 

	{dpp->ai_force[i]=0;dpp->ai_fault[i]=0;dpp->ai_force_old[i]=0;dpp->ai_fault_old[i]=0;}



	for(i=0;i<AO_SIZE/16+1;i++) 

	{dpp->ao_force[i]=0;dpp->ao_fault[i]=0;dpp->ao_force_old[i]=0;dpp->ao_fault_old[i]=0;}



	for(i=0;i<PI_SIZE/16+1;i++) { dpp->pi_force[i]=0;dpp->pi_fault[i]=0;}  



	for(i=0;i<AIALL_SIZE;i++) 

	{	dpp->ai_value[i]=0;

		dpp->ai_value_old[i]=0;

		dpp->flag_aialm[i]=0;

		dpp->flag_overflow[i]=0;

		

		dpp->trend[i].t_flag = 0;

		dpp->trend_id[i] = 0;

		dpp->trendAlm_old[i]=0;

		dpp->trend[i].t_value_old=0;

	}

	

	for(i=0;i<AO_SIZE;i++) 		{dpp->ao_value[i]=0;dpp->ao_value_old[i]=0;}

	

	for(i=0;i<OBJ_SIZE;i++) 	memset(&dpp->obj_new[i],0,6);

	

	dpp->soe_ptr2=dpp->soe_ptr1;

				

	dpp->obj_ptr2=dpp->obj_ptr1;

				

	dpp->dii_ptr2=dpp->dii_ptr1;



	dpp->msg_ptr2=dpp->msg_ptr1;

				

	dpp->din_ptr2=dpp->din_ptr1;



	dpp->dout_ptr2 =dpp->dout_ptr1;

				

	dpp->aialm_ptr2=dpp->aialm_ptr1;

				

	dpp->aoalm_ptr2=dpp->aoalm_ptr1;

					

	dpp->pialm_ptr2=dpp->pialm_ptr1;

				

	dpp->ai_ptr2=dpp->ai_ptr1;



	dpp->ao_ptr2=dpp->ao_ptr1;



	dpp->pi_ptr2 = dpp->pi_ptr1;

	

	dpp->obj_ptr2=dpp->obj_ptr1;

		

}


//addby wzg 20120331
extern SYS_COMM_TABLE	*shm_syscomm_addr;

  

//extern	LCUID		lcu_list[];
extern	LCUID_INC_IDX		lcu_list[];
  
void* scada(void *lcu_pos)
{

	struct plc_data_seg	*seg;

	struct plc_send_seg *send_seg;

	DPP_DATA 		*dpp;
	
	unsigned short 	no;



	//LCUID lcuno;
	LCUID_INC_IDX lcuno;
	//LCUID lcuid_tmp;

 
	/*wzg 070507*/

	//lcuno.stnid= lcu_list[(int)lcu_pos].stnid;

	//lcuno.devid= lcu_list[(int)lcu_pos].devid;

	lcuno.lcu_id.stnid= lcu_list[(int)lcu_pos].lcu_id.stnid;//cxs
	
	lcuno.lcu_id.devid= lcu_list[(int)lcu_pos].lcu_id.devid;//cxs

	lcuno.stn_idx= lcu_list[(int)lcu_pos].stn_idx;//cxs
	
	lcuno.dev_idx= lcu_list[(int)lcu_pos].dev_idx;//cxs
				
	//lcuid_tmp.stnid=lcuno.lcu_id.stnid;
	//lcuid_tmp.devid=lcuno.lcu_id.devid;

//	dpp = & dpp_data[(int)lcu_pos];

	if((dpp=malloc(sizeof(DPP_DATA)))==NULL)

	{

		err_write("Fail to allocate needed memory");

		exit(-1);

	}

	memset(dpp, 0, sizeof(DPP_DATA));

	//moved to here for each lcu by wzg 20120312
	dpp->dpp_sendata_flag = ReadDefineFile(lcuno.lcu_id, &dpp->dpp_sendata);

	

	dpp ->first_scan = 1;

	

	dpp->isdataflag_old =1;	dpp->isdataflag =0;



	
	 
	dpp->dpp_sendata.otime=0;//cxs wzg20120319
	
 
	seg = &shm_plc_data->seg[segid[lcuno.stn_idx][lcuno.dev_idx]];//cxs
	
	
 
	send_seg =&(shm_plc_send->seg[lcuno.stn_idx][lcuno.dev_idx]);//cxs
          

        send_seg->send_update_flag =0; 

        			

        seg->com_port_no=0;



        for(no=0;no<seg->com_port_no;no++)	seg->com_data[no].firstscan =0;

        

        dpp->fault_count=0; 

        

        MySleep(3000); /*add by yh 070513 ,wait for plc data*/

        

        dpp->soeptr = seg->soealm_ptr ;

       

        dpp->polptr = seg->pol_seg_ptr ;

        

        dpp->objptr = seg->objalm_ptr;

  

	/*added by wzg 20080107*/

	dpp->ana_update_flag= seg->ana_update_flag; 

	dpp->rtd_update_flag= seg->rtd_update_flag; 

	dpp->obj_update_flag= seg->obj_update_flag; 

	dpp->imp_update_flag= seg->imp_update_flag;

	dpp->sysmsg_update_flag= seg->sysmsg_update_flag;

	//addby wzg 20120331
	dpp->rtd_otime=dpp->ai_otime=dpp->ao_otime=dpp->imp_otime=dpp->dii_otime=dpp->pol_otime=dpp->dout_otime=dpp->msg_otime=dpp->obj_otime=time(NULL);
	dpp->stnid= lcuno.lcu_id.stnid;
	dpp->devid= lcuno.lcu_id.devid;
	shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag=0;

	while (1)

	{   	

		MySleep(200);  /* 200ms */

		if(-1!=ReadLcuLinkStat(lcuno,seg,dpp))
		{
			if ( dpp->isdataflag==1) 
			{
				dpp->plc_firstrun = seg->head.host_firstrun & 0x00ff;

				if(dpp->plc_firstrun ==1 )/*(dpp->plc_firstrun ==0 && dpp->plc_firstrun_old==1) modified by ycx 090326*/
				{		
					char err_str[128];
		
					 dpp->soeptr = seg->soealm_ptr ;
       
       				 dpp->polptr = seg->pol_seg_ptr ;
      		        					 
					 dpp->objptr = seg->objalm_ptr;		/*add by ycx 090326 */       	 
       		 
					sprintf(err_str, "plc first run : seg->soealm_ptr= %d,seg->pol_seg_ptr= %d,seg->objalm_ptr=%d", seg->soealm_ptr,seg->pol_seg_ptr,seg->objalm_ptr);
					err_write(err_str);

					dpp->first_scan =1;
				}

				if(seg->firstscan==1 )	
					dpp->first_scan =1;

				if(dpp->first_scan ==1)
					init(dpp);

				if(dpp->isdataflag_old == -1)	
					dpp->first_scan =1; /*modified ycx 050717*/

				ProcessPlcData(lcuno, seg, dpp, send_seg);

				/*	ProcessCommData(lcuno, seg, dpp);*/

				for(no=0;no<seg->com_port_no;no++)
				{
					if(seg->com_data[no].firstscan ==1)
						seg->com_data[no].firstscan =0;
				}

				dpp->plc_firstrun_old = dpp->plc_firstrun;
				dpp->isdataflag_old = dpp->isdataflag;

			}

	 		/*added by wzg 20071225*/
			else
			{
	 			dpp->ana_update_flag=seg->ana_update_flag; 

	 			dpp->rtd_update_flag=seg->rtd_update_flag; 

				dpp->obj_update_flag=seg->obj_update_flag; 

				dpp->imp_update_flag=seg->imp_update_flag;

				dpp->sysmsg_update_flag=seg->sysmsg_update_flag;

				dpp->polptr=seg->pol_seg_ptr;

				dpp->soeptr = seg->soealm_ptr;

				dpp->objptr = seg->objalm_ptr;
				//addby wzg 20120331
				dpp->rtd_otime=dpp->ai_otime=dpp->ao_otime=dpp->imp_otime=dpp->dii_otime=dpp->pol_otime=dpp->dout_otime=dpp->msg_otime=dpp->obj_otime=time(NULL);
				shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag=0;
			}

			if( dpp->isdataflag == -1)
			{
				if( dpp->isdataflag_old ==1)
				{
					ProcessData2(lcuno, dpp);
				}

				dpp->isdataflag_old = dpp->isdataflag;

			}					 			

		}

	 	/*added by wzg 20071225*/
		else
		{
 			dpp->ana_update_flag=seg->ana_update_flag; 

 			dpp->rtd_update_flag=seg->rtd_update_flag; 

			dpp->obj_update_flag=seg->obj_update_flag; 

			dpp->imp_update_flag=seg->imp_update_flag;

			dpp->sysmsg_update_flag=seg->sysmsg_update_flag;

			dpp->polptr=seg->pol_seg_ptr;

			dpp->soeptr = seg->soealm_ptr;

			dpp->objptr = seg->objalm_ptr;

			//addby wzg 20120331
			dpp->rtd_otime=dpp->ai_otime=dpp->ao_otime=dpp->imp_otime=dpp->dii_otime=dpp->pol_otime=dpp->dout_otime=dpp->msg_otime=dpp->obj_otime=time(NULL);
			shm_syscomm_addr->data_poll[lcuno.lcu_id.stnid][lcuno.lcu_id.devid].poll_flag=0;

			MySleep(3000);  /* 2000ms  cxs*/
		}

	}		

}



