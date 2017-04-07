/************************************************************************************************************************************************
FILENAME:	dpptest.c
FUNCTION:	
	
	test functions of dpplib :PollSysRtu(),CommAnaAlmPro(),CommWriteShare()

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
2006.04.20		ycx
************************************************************************************************************************************************/

#include "../../../dms/v4.0a/inc/dms_com.h"

#include "../../../dps/v4.0a/inc/dps_rec_def.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../dps/v4.0a/inc/ipc_common.h"

#include "../../../lan/v4.0a/inc/lan_common.h"
#include "../../../plc/v4.0a/inc/lcu_data.h"

#include "../../../dpp/v4.0a/inc/dppdef.h"
#include "../../../dpp/v4.0a/inc/dpp_functions.h"


void main(void)
{	
	POINTER 		pt;
	float 			value,xcode;
	unsigned short 		i0,j,i,fault,res,port_no,offset,length,firstrun,count;
	DATE_TIME 		tm;
	
	LCUID 			lcuno;
	struct plc_data_seg	*seg;
	DPP_DATA 		dpp_data1;
	DPP_DATA 		*dpp;
	
	
	unsigned char ai_buf[AI_SIZE*2],ai_qlt[AI_SIZE/16*2],pi_buf[PI_SIZE*4],pi_qlt[PI_SIZE/16*2],pol_buf[DIN_SIZE/16*2],pol_qlt[DIN_SIZE/16*2];
	
	
	printf("dpptest.dxe is running............\n");	

	if(lan_out_init() == FALSE)        {printf("dpptest.dxe  call Lan_out_init() error.......\n");exit(0);}

	if(get_lcu_shm(0) == FALSE)        {printf("dpptest.dxe  call get_lcu_shm()  error.......\n");exit(0);}

	if(-1==MountDB(DATABASE_NAME))     {printf("dpptest.dxe call MountDB() error.......\n");exit(0);}

	if(InitPlcDataShm(0) == FALSE)     {printf("dpptest.dxe call InitPlcDataShm() error.......\n");exit(0);}
	
	
	if(0==PollSysRtu()) printf("dpptest.dxe::PollSysRtu() is OK\n");
	
	lcuno.stnid = 3;
	lcuno.devid = 1;
	
	seg = & shm_plc_data->seg[segid[lcuno.stnid][lcuno.devid]];
	dpp = & dpp_data1;
	
	firstrun =1;count=0;

	while(1)
	{
	
		/********************* test for CommAnaAlmPro() **********************/
		
		if(seg->head.timetag_flag !=0)
		{
		 tm.ms = seg->head.ai_sec_ms % 1000;
		 tm.second = seg->head.ai_sec_ms /1000;
		 tm.minute = seg->head.ai_hour_min & 0xff;
		 tm.hour = (seg->head.ai_hour_min >>8) & 0xff;
		 tm.day = seg->head.ai_month_day &0xff;
		 tm.month = (seg->head.ai_month_day >>8) & 0xff;
		 tm.year = seg->head.ai_year;
		}
		else
		 	GetSysTime(&tm);		
		
		

		ReadDb_ANA(lcuno,ANA_TYPE,
				seg->head.ai_off,
				seg->head.ai_size,				
				dpp->ana_db);
	
		/***** read AI new value *****/

		ReadNewValue_ANA(lcuno,ANA_TYPE,
						 seg->head.ai_off,
						 seg->head.ai_size,
						 seg->ai_data,seg->ai_qlt,seg->ai_en,
						 dpp);
					
		i=0;
		
	        if ((dpp->ana_db[i].lo_code)<0) /*dpp->ana_new should be signed */
		
			xcode=(short)*(dpp->ai_new+i) ;
		else
			xcode=*(dpp->ai_new+i) ;

		if( xcode> dpp->ana_db[i].hi_code)	 dpp->ai_value[i]=	dpp->ana_db[i].hi_range;
			
		else if( xcode < dpp->ana_db[i].lo_code) dpp->ai_value[i]=	dpp->ana_db[i].lo_range;

		else
				dpp->ai_value[i]=( ( xcode- dpp->ana_db[i].lo_code ) * (dpp->ana_db[i].hi_range- dpp->ana_db[i].lo_range)
					/(dpp->ana_db[i].hi_code- dpp->ana_db[i].lo_code)+dpp->ana_db[i].lo_range)* (dpp->ana_db[i].eng_cvt) 
					+ (dpp->ana_db[i].compensate) + (dpp->ana_db[i].zero);
					 
		value = dpp->ai_value[i];
		
		 j=i/16;
		 i0=i-j*16;

		fault = (dpp->ai_fault[j] >> i0) & 0x01;
		
		pt.stn_id 	= lcuno.stnid;
		pt.dev_id 	= lcuno.devid;
		pt.data_type 	= ANA_TYPE,
		pt.pt_id 	= i;
		
		res=CommAnaAlmPro(pt,value,fault,tm);

		if(1==res) printf("dpptest.dxe::CommAnaAlmPro() send alarm\n");
	
		if(0==res) printf("dpptest.dxe::CommAnaAlmPro() send no alarm\n");
		
		sleep(1);
		
		
		/********************* test for CommWriteANA() **********************/
		
		count++;
		
		offset = 856;
		length = 21;
		
		for(i=0;i<length*2;i=i+2) 
		{	
			if(count==1||count==3)
			{
				ai_buf[i] = 0xc8;
				ai_buf[i+1] = 0x32;
			}
			else
			{
				ai_buf[i] = 0x00;
				ai_buf[i+1] = 0x00;
				
			}
			
		}
		
		for(i=0;i<length/8+1;i++) 
		
		{
			if(count==1)   ai_qlt[i] = 0xff;
			else 	       ai_qlt[i] = 0x00;
			
		 /*	ai_qlt[i] =0;*/
		 	
			
		}
		
	
		
		CommWriteANA(lcuno,port_no,firstrun,offset,length,ai_buf,ai_qlt);
		
		/************* test for CommWriteIMP() *********************/
		
		offset = 32;
		length = 7;
		
		for(i=0;i<length*4;i=i+4) 
		
		{	
			if(count==1||count==3)
			{
				pi_buf[i] = 0x01;
				pi_buf[i+1] = 0x01;
				pi_buf[i+2] = 0x01;
				pi_buf[i+3] = 0x01;
			}
			else
			{
				pi_buf[i] = 0x02;
				pi_buf[i+1] = 0x02;
				pi_buf[i+2] = 0x02;
				pi_buf[i+3] = 0x02;
			}
			
		}
		
		for(i=0;i<length/8+1;i++) 
		
		{
			if(count==1)   pi_qlt[i] = 0xff;
			else 	       pi_qlt[i] = 0x00;
			
		 	/*pi_qlt[i] =0;*/
		 	
			
		}		
		
		
		CommWriteIMP(lcuno,port_no,firstrun,offset,length,pi_buf,pi_qlt);
		
		/************** test for CommWriteIND() ********************/
		
		offset = 1568;
		length = 45;
		
		for(i=0;i<length/8+1;i++) 
		
		{
			printf("count=%d\n",count);
			
			if(count%2 ==0) pol_buf[i] = 0x00;
			else 	        pol_buf[i] = 0xff;
			
			printf("pol_buf[%d]=%x\n",i,pol_buf[i]);
				
			/*if(count==1) pol_buf[i] = 0x00;
			else 	       pol_buf[i] = 0xff;
			
			if(count==1) pol_qlt[i] = 0xff;
			else 	       pol_qlt[i] = 0x00;
			
		 	pol_buf[i] =0;*/
		 	
			
		}
		
		
				
		CommWritePOL(lcuno,port_no,firstrun,offset,length,pol_buf,pol_qlt);
		
		if(count ==4) {firstrun=0;count=0;}
		
		printf("...............\n");		
	}
			
}

