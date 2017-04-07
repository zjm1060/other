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



void Process_OBJ(LCUID lcuno,USHORT offset,USHORT length,DPP_DATA *dpp)
{
 unsigned short i;  

 /***********************  obj processing *****************/

	 for (i= offset;i<offset+length;i++) 
	 {
	       
	     if(dpp->obj_db[i].iost.scanEnable ==1)
	     {
		 
		 /*****  obj all-send form processing *****/
		 
		 memcpy(&dpp->obj_all[i],&dpp->obj_new[i],6);	
	
	     }
	 } 

}


void Process_OBJAlm(LCUID lcuno,USHORT endPtr,USHORT objnum_max,UCHAR *objbuf, DPP_DATA *dpp,UCHAR timeflag,DATE_TIME tm)
{
	      
	unsigned short  i,obj_id,ms;
	unsigned short  st[4];

	i = endPtr * 16; 
	
	/*printf(" i= %d ,endPtr=%d\n",i,endPtr);printf(" i0=%2x i1=%2x\n",(*(objbuf+i)),(*(objbuf+i+1)));*/
	
	obj_id  = (unsigned short)(*(objbuf+i))+(unsigned short)(*(objbuf+i+1)<<8);/*printf(" id= %d \n",obj_id);*/

	if(dpp->obj_db[obj_id].iost.scanEnable ==1)
	 {
		dpp->objbuf[dpp->obj_ptr1].type_id = DPS_DEV_FB;
		
		dpp->objbuf[dpp->obj_ptr1].stn_id = lcuno.stnid;
		
		dpp->objbuf[dpp->obj_ptr1].dev_id = lcuno.devid;
		
		dpp->objbuf[dpp->obj_ptr1].data_type = DEVOBJ_TYPE;
		
		dpp->objbuf[dpp->obj_ptr1].number = 1;
		
		st[0] = obj_id;
		st[1] = *(objbuf+i+2)+(*(objbuf+i+3)<<8);
		st[2] = *(objbuf+i+4)+(*(objbuf+i+5)<<8);
		st[3] = *(objbuf+i+6)+(*(objbuf+i+7)<<8);	
		
		memcpy(&(dpp->objbuf[dpp->obj_ptr1].obj_object[0]),st,8);

		GetLocalHostId(&dpp->objbuf[dpp->obj_ptr1].host_id);
		
		/*printf(" timeflag= %d \n",timeflag);*/
		
		if(timeflag)
		{	
			/*printf(" ms1= %2x ,ms2=%2x\n",(*(objbuf+i+8)),(*(objbuf+i+9)));*/
		
			ms =(unsigned short) (*(objbuf+i+8))+(unsigned short)(*(objbuf+i+9)<<8);
							  
			dpp->objbuf[dpp->obj_ptr1].ms        =  ms%1000; 

			dpp->objbuf[dpp->obj_ptr1].second    =  ms/1000; 

			dpp->objbuf[dpp->obj_ptr1].minute    = *(objbuf+i+10);

			dpp->objbuf[dpp->obj_ptr1].hour	     = *(objbuf+i+11);

			dpp->objbuf[dpp->obj_ptr1].day	     = *(objbuf+i+12);

			dpp->objbuf[dpp->obj_ptr1].month     = *(objbuf+i+13);

			dpp->objbuf[dpp->obj_ptr1].year	     = (unsigned short)(*(objbuf+i+14)) +(unsigned short)(*(objbuf+i+15)<<8);
			
			/*printf("11111111\n");
			printf(" ms= %d \n",ms);printf(" ms=%d\n",dpp->objbuf[dpp->obj_ptr1].ms);printf(" second =%d\n",dpp->objbuf[dpp->obj_ptr1].second);
			printf(" min=%d\n",(*(objbuf+i+10)));printf(" hour=%d\n",(*(objbuf+i+11)));printf(" day=%d\n",(*(objbuf+i+12)));printf(" month=%d\n",(*(objbuf+i+13)));*/
		}else
		{
			
			dpp->objbuf[dpp->obj_ptr1].ms        =  tm.ms;

			dpp->objbuf[dpp->obj_ptr1].second    =  tm.second;

			dpp->objbuf[dpp->obj_ptr1].minute    =  tm.minute;

			dpp->objbuf[dpp->obj_ptr1].hour	     =  tm.hour;

			dpp->objbuf[dpp->obj_ptr1].day	     =  tm.day;

			dpp->objbuf[dpp->obj_ptr1].month     =  tm.month;

			dpp->objbuf[dpp->obj_ptr1].year	     =  tm.year;
			
			/*printf("000000\n");
			printf(" ms=%d\n",tm.ms);printf(" second =%d\n",tm.second);
			printf(" min=%d\n",tm.minute);printf(" hour=%d\n",tm.hour);printf(" day=%d\n",tm.day);printf(" month=%d\n",tm.month);printf(" year=%d\n",tm.year);*/

		
		}
	 
	 	dpp->obj_ptr1++;
	 	
		if( dpp->obj_ptr1 == ALM_BUF_SIZE )   dpp->obj_ptr1 = 0;  

	  }      
	 
}

