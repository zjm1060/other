/************************************************************************************************************************************************
FILENAME:	ProcessDii.c
FUNCTION:	
		functions of  processing dii ditmap sended from plc to form alarm packet :Process_DII()
		functions of  processing soe alarm buffer sended from plc to form  alarm packet :Process_SOE()
		
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


void Process_DII(LCUID lcuno,USHORT offset,USHORT length,DPP_DATA *dpp,DATE_TIME tm)
{
	unsigned short          i0,i,j,m,n,off,num,fault,force,wordlen;
	unsigned short 		vary,vary1,vary2;
	DI_STRUCT		di_object[DIN_SIZE];
	
	if(length>DII_SIZE) 	{Err_log("soe length > DII_SIZE\n");return;}
	
	num=0;
	
	/*222222222*/
	
	for (i=offset;i<offset+length;i++)
	{
		j=i/16;
		i0=i-16*j; 
				
		vary2 = dpp->dii_fault_old[j]  ^  dpp->dii_fault[j]; 

		if (((( vary2>>i0) & 0x01) == 1)&&(dpp->ind_db[i].iost.scanEnable  ==  1))
	       {
			memset(&di_object[num], 0, sizeof(di_object[num]));
	    	    memcpy(&di_object[num].status,&(dpp->ind_db[i].status),2);
	    	    
	    	    fault = (dpp->dii_fault[j] >> i0) & 0x01 ;
	    	    	  
			di_object[num].point_id=i;
		    di_object[num].status.alm_type  = IND_ALM_TYPE_CHANFAIL;
		    di_object[num].status.chan_fail = fault;
		    di_object[num].status.rt_state  = (dpp->dii_new[j] >> i0) & 0x01;
		    num++;		  							  
	       }

	}

	/*3333333333*/

	for (i=offset;i<offset+length;i++)
	{
		j=i/16;
		i0=i-16*j; 
				
		vary1 = dpp->dii_force_old[j]  ^  dpp->dii_force[j];  

		if (((( vary1>>i0) & 0x01) == 1)&&(dpp->ind_db[i].iost.scanEnable  ==  1))
	  	{
			memset(&di_object[num], 0, sizeof(di_object[num]));
			memcpy(&di_object[num].status,&(dpp->ind_db[i].status),2);
	    	    
	    	    force = (dpp->dii_force[j]>>i0) & 0x01;
	    	    
		    di_object[num].point_id=i;
		    
		    di_object[num].status.alm_type  = IND_ALM_TYPE_MANFORCE;
		    di_object[num].status.man_set   = force;
		    di_object[num].status.rt_state  = (dpp->dii_new[j] >> i0) & 0x01;
		   
		    
		    num++;		  
		 }	
					
		  
	 }

	


	m = num;off=0;/*printf("111111111111 num =%d\n",num);*/
	
	if( num>0)
	{
		do{
	       
		if ( m > IND_ALM_MAX) n = IND_ALM_MAX;
		else n=m;

		memset(&dpp->diibuf[dpp->dii_ptr1], 0, sizeof(dpp->diibuf[dpp->dii_ptr1]));
		
		dpp->diibuf[dpp->dii_ptr1].type_id = DPS_IND_ALM;

		dpp->diibuf[dpp->dii_ptr1].stn_id =lcuno.stnid;

		dpp->diibuf[dpp->dii_ptr1].dev_id =lcuno.devid;

		dpp->diibuf[dpp->dii_ptr1].data_type=SOE_TYPE;

		dpp->diibuf[dpp->dii_ptr1].number = n;

		memcpy(dpp->diibuf[dpp->dii_ptr1].di_object,&di_object[off],n*sizeof(DI_STRUCT));

		dpp->diibuf[dpp->dii_ptr1].ms=tm.ms;

		GetLocalHostId(&dpp->diibuf[dpp->dii_ptr1].host_id);

		dpp->diibuf[dpp->dii_ptr1].second=tm.second;

		dpp->diibuf[dpp->dii_ptr1].minute=tm.minute;

		dpp->diibuf[dpp->dii_ptr1].hour=tm.hour;

		dpp->diibuf[dpp->dii_ptr1].day=tm.day;

		dpp->diibuf[dpp->dii_ptr1].month=tm.month;

		dpp->diibuf[dpp->dii_ptr1].year=tm.year;
		
		Record_IND(dpp->dii_ptr1,dpp->diibuf);
		  
		 
		if( dpp->dii_ptr1 == ALM_BUF_SIZE-1)	dpp->dii_ptr1 = 0;

		else dpp->dii_ptr1++;

		if( dpp->dii_ptr1 == dpp->dii_ptr2)
		{

		  dpp->dii_ptr2++;

		  if ( dpp->dii_ptr2 == ALM_BUF_SIZE)     dpp->dii_ptr2=0;
		}

		m = m - n; off = off+n;

	
		}while(m!=0);
		
  	}
  	
  	
	num=0;
		
	for (i=offset;i<offset+length;i++)
	{
		j=i/16;
		i0=i-16*j; 
				
	  	vary = dpp->dii_old[j]  ^  dpp->dii_new[j];   		/* Find the rt_state Change bit */
		vary1 = dpp->dii_force_old[j]  ^  dpp->dii_force[j];
		vary2 = dpp->dii_fault_old[j]  ^  dpp->dii_fault[j];
		
		
	  	if (((vary>>i0) & 0x01) == 1 && ((vary1>>i0) & 0x01) == 0 && ((vary2>>i0) & 0x01) == 0 && dpp->ind_db[i].iost.scanEnable  ==  1)
	  	{
			memset(&di_object[num], 0, sizeof(di_object[num]));
			memcpy(&di_object[num].status,&(dpp->ind_db[i].status),2);  			   	   
			  			   			   
			   di_object[num].point_id=i;
			   
			   di_object[num].status.alm_type  = IND_ALM_TYPE_CHANGE;
			   di_object[num].status.chan_fail = (dpp->dii_fault[j] >> i0) & 0x01;
			   di_object[num].status.man_set   = (dpp->dii_force[j] >> i0) & 0x01;
			   di_object[num].status.rt_state  = (dpp->dii_new[j] >> i0) & 0x01;
			   
			   
			   num++; 
			   printf("num=%d \n",num);
			   
			   /*  printf("point_id=%d  status=%d\n",di_object[num].point_id,di_object[num].status);*/

	  	} 				
		  
	 } 
	 
	 if(num>0) 
	 {
	 	memcpy(dpp->dii_object,di_object,num*sizeof(DI_STRUCT));
	 	
	 	dpp->dii_num = num;
	 	
	 	dpp->soeallsend_flag = 1;
	 }


    j = offset/16;	
    wordlen = Mod16(offset+length);
        
    for (i=j;i<wordlen;i++)		
	{
		dpp->dii_old[i] = dpp->dii_new[i];
		dpp->dii_force_old[i] = dpp->dii_force[i];
		dpp->dii_fault_old[i] = dpp->dii_fault[i];
	}
	

}

UCHAR BcdToCHAR(UCHAR BcdCode)
{
	return ((BcdCode&0x0f) + ((BcdCode>>4)&0x000f)*10);
}

USHORT BcdToSHORT(USHORT BcdCode)
{
	return ((BcdCode&0x000f) + ((BcdCode>>4)&0x000f)*10 
		+ ((BcdCode>>8)&0x000f)*100 + ((BcdCode>>12)&0x000f)*1000);
}



/*
void Process_SOE(LCUID lcuno,USHORT endPtr,USHORT soenum_max,UCHAR *soebuf, DPP_DATA *dpp)
{
	      
	unsigned short i0,j,i,soe_id,state,ms;
	char		typestr[12];

	GetGroupTypeNameById( lcuno.stnid,lcuno.devid, typestr );

	if(strstr(typestr,"QUANTUM")!=NULL || strstr(typestr,"PREMIUM")!=NULL) 	
	{
		 i = endPtr * 14;

		 if((((*(soebuf+i+3)) >> 6) & 0x03) == 0x01)

			 soe_id  = (unsigned short)((*(soebuf+i+2))) * 32
					  +(unsigned short)(((*(soebuf+i+3)) & 0x3f)-1);

		 state = (unsigned short)(*(soebuf+i+4));

		 j=soe_id/16;

		i0=soe_id-16*j; 

		if((dpp->ind_db[soe_id].iost.scanEnable == 1)&&(((dpp->dii_fault[j] >> i0) & 0x01) ==0)&&(((dpp->dii_force[j] >> i0) & 0x01) ==0))
		{
			dpp->soebuf[dpp->soe_ptr1].type_id    = DPS_IND_ALM;

			dpp->soebuf[dpp->soe_ptr1].stn_id     = lcuno.stnid;

			dpp->soebuf[dpp->soe_ptr1].dev_id      	= *(soebuf+i);

			dpp->soebuf[dpp->soe_ptr1].data_type	= *(soebuf+i+1);

			dpp->soebuf[dpp->soe_ptr1].number = 1;

			dpp->soebuf[dpp->soe_ptr1].di_object[0].point_id =soe_id;
			
			dpp->soebuf[dpp->soe_ptr1].di_object[0].status.rt_state = state;				   
			dpp->soebuf[dpp->soe_ptr1].di_object[0].status.alm_type = IND_ALM_TYPE_CHANGE;
			
			ms = (unsigned short)(*(soebuf+i+5)) +(unsigned short)((*(soebuf+i+6))<<8);
			
			GetLocalHostId(&dpp->soebuf[dpp->soe_ptr1].host_id);
							  
			dpp->soebuf[dpp->soe_ptr1].ms        =  ms%1000;

			dpp->soebuf[dpp->soe_ptr1].second    =  ms/1000;

			dpp->soebuf[dpp->soe_ptr1].minute	   = (*(soebuf+i+7)) & 0x3f;

			dpp->soebuf[dpp->soe_ptr1].hour	   = (*(soebuf+i+8)) & 0x1f;

			dpp->soebuf[dpp->soe_ptr1].day	   = (*(soebuf+i+9)) & 0x1f;

			dpp->soebuf[dpp->soe_ptr1].month	   = *(soebuf+i+11);

			dpp->soebuf[dpp->soe_ptr1].year	   = (unsigned short)(*(soebuf+i+12)) +(unsigned short)((*(soebuf+i+13))<<8);

		}

		dpp->soe_ptr1++;
	 	
		if( dpp->soe_ptr1 == ALM_BUF_SIZE )   dpp->soe_ptr1 = 0;  

		
	}

	else if(strstr(typestr,"SIEMENS")!=NULL)
	{
		 i = endPtr * 14;

		 soe_id  = (unsigned short)(*(soebuf+i+2)) * 16
				  +(unsigned short)(*(soebuf+i+3) -1);

		 if ((*(soebuf+i+4))==0x80)		 state = 1;
		 if ((*(soebuf+i+4))==0x00)		 state = 0;

		 j=soe_id/16;

		 i0=soe_id-16*j; 

		if((dpp->ind_db[soe_id].iost.scanEnable == 1)&&(((dpp->dii_fault[j] >> i0) & 0x01) ==0)&&(((dpp->dii_force[j] >> i0) & 0x01) ==0))
		{
			dpp->soebuf[dpp->soe_ptr1].type_id    = DPS_IND_ALM;

			dpp->soebuf[dpp->soe_ptr1].stn_id     = lcuno.stnid;

			dpp->soebuf[dpp->soe_ptr1].dev_id      	= *(soebuf+i);

			dpp->soebuf[dpp->soe_ptr1].data_type	= *(soebuf+i+1);

			dpp->soebuf[dpp->soe_ptr1].number = 1;

			dpp->soebuf[dpp->soe_ptr1].di_object[0].point_id =soe_id;
			
			dpp->soebuf[dpp->soe_ptr1].di_object[0].status.rt_state = state;				   
			dpp->soebuf[dpp->soe_ptr1].di_object[0].status.alm_type = IND_ALM_TYPE_CHANGE;
			
			ms = (unsigned short)(*(soebuf+i+5)) +(unsigned short)((*(soebuf+i+6))<<8);
			
			GetLocalHostId(&dpp->soebuf[dpp->soe_ptr1].host_id);
							  
			dpp->soebuf[dpp->soe_ptr1].ms      = BcdToCHAR(*(soebuf+i+12))*10 + ((*(soebuf+i+13))&0xf0)>>4);

			dpp->soebuf[dpp->soe_ptr1].second  = BcdToCHAR(*(soebuf+i+11));

			dpp->soebuf[dpp->soe_ptr1].minute  = BcdToCHAR(*(soebuf+i+10));

			dpp->soebuf[dpp->soe_ptr1].hour	   = BcdToCHAR(*(soebuf+i+9));

			dpp->soebuf[dpp->soe_ptr1].day	   = BcdToCHAR(*(soebuf+i+8));

			dpp->soebuf[dpp->soe_ptr1].month   = BcdToCHAR(*(soebuf+i+7));

			dpp->soebuf[dpp->soe_ptr1].year	   = BcdToCHAR(*(soebuf+i+6))+2000;
				
		}

		dpp->soe_ptr1++;
	 	
		if( dpp->soe_ptr1 == ALM_BUF_SIZE )   dpp->soe_ptr1 = 0;  		
	}
	
	else if(strstr(typestr,"GE")!=NULL)
	{
		Err_log("LCU type is GE, no process\n");
	}

	else
	{
		Err_log("LCU type define error\n");
	}	        
	 
}
*/
	
/*added by wzg 20070802*/
typedef struct
{
	UCHAR	dev_id;
	UCHAR	data_type;

#ifdef	_BIG_ENDIAN_
	UCHAR	res1:1;
	UCHAR	module_no:7;
#else
	UCHAR	module_no:7;
	UCHAR	res1:1;
#endif

#ifdef	_BIG_ENDIAN_
	UCHAR	group_mode:2;
	UCHAR	group_no:6;
#else
	UCHAR	group_no:6;
	UCHAR	group_mode:2;
#endif

	UCHAR	state;
	UCHAR	lo_ms;
	UCHAR	hi_ms;

#ifdef	_BIG_ENDIAN_
	UCHAR	time_valid:1;
	UCHAR	res2:1;
	UCHAR	min:6;
#else
	UCHAR	min:6;
	UCHAR	res2:1;
	UCHAR	time_valid:1;
#endif

#ifdef	_BIG_ENDIAN_
	UCHAR	DST_valid:1; /*夏令时*/
	UCHAR	res3:2;
	UCHAR	hour:5;
#else
	UCHAR	hour:5;
	UCHAR	res3:2;
	UCHAR	DST_valid:1; /*夏令时*/
#endif

#ifdef	_BIG_ENDIAN_
	UCHAR	weekday:3;
	UCHAR	day:5;
#else
	UCHAR	day:5;
	UCHAR	weekday:3;
#endif

	UCHAR	res4;
	UCHAR	month;
	USHORT	year;
} QUANTUM_SOE_INFO;

void Process_QUANTUM_SOE(LCUID lcuno,USHORT endPtr,USHORT dii_pt_num,UCHAR *soebuf, DPP_DATA *dpp, USHORT pt_num_in_module)
{
	QUANTUM_SOE_INFO	soe_info;	      
	unsigned short		i0,j0,soe_id,ms;
	char				err_buf[256];

	if(sizeof(soe_info)!=14)
	{
		Err_log("soe data structure defined error");
		return;
	}

	memcpy(&soe_info, soebuf+endPtr*sizeof(soe_info), sizeof(soe_info));
	Swap2Byte(&soe_info.year);

	if(soe_info.group_mode!=1)
		return;
	soe_id= soe_info.module_no*pt_num_in_module+soe_info.group_no-1;
	if(soe_id<0 || soe_id>=dii_pt_num || soe_id>=DII_SIZE || soe_info.group_no>pt_num_in_module)
	{
		sprintf(err_buf, "LCU %d.%d received soe pt_id error: module_no= %d, group_no= %d", 
			soe_info.module_no, soe_info.group_no);
		Err_log(err_buf);
		Err_log("module_no starts from 0, group_no starts from 1");
		return;
	}

	if(dpp->ind_db[soe_id].iost.scanEnable != 1)
		return;

	j0=soe_id/16;
	i0=soe_id%16; 
	if(((dpp->dii_fault[j0] >> i0) & 0x01))
		return;
	if(((dpp->dii_force[j0] >> i0) & 0x01))
		return;

	if(soe_info.dev_id != lcuno.devid)
	{
		sprintf(err_buf, "LCU %d.%d received other lcu soe info dev_id= %d", 
			lcuno.stnid, lcuno.devid, soe_info.dev_id);
		Err_log(err_buf);
		return;
	}
	if(soe_info.data_type != SOE_TYPE)
	{
		sprintf(err_buf, "LCU %d.%d received soe info data_type error: %d", 
			lcuno.stnid, lcuno.devid, soe_info.data_type);
		Err_log(err_buf);
		return;
	}

	dpp->soebuf[dpp->soe_ptr1].type_id		= DPS_IND_ALM;
	dpp->soebuf[dpp->soe_ptr1].stn_id		= lcuno.stnid;
	dpp->soebuf[dpp->soe_ptr1].dev_id		= soe_info.dev_id;
	dpp->soebuf[dpp->soe_ptr1].data_type	= soe_info.data_type;
	dpp->soebuf[dpp->soe_ptr1].number		= 1;
	dpp->soebuf[dpp->soe_ptr1].di_object[0].point_id =soe_id;
	dpp->soebuf[dpp->soe_ptr1].di_object[0].status.rt_state = soe_info.state & 0x01;				   
	dpp->soebuf[dpp->soe_ptr1].di_object[0].status.alm_type = IND_ALM_TYPE_CHANGE;
	GetLocalHostId(&dpp->soebuf[dpp->soe_ptr1].host_id);
	ms= soe_info.lo_ms | (soe_info.hi_ms <<8);
	dpp->soebuf[dpp->soe_ptr1].ms			= ms%1000;
	dpp->soebuf[dpp->soe_ptr1].second		= ms/1000;
	dpp->soebuf[dpp->soe_ptr1].minute		= soe_info.min;
	dpp->soebuf[dpp->soe_ptr1].hour			= soe_info.hour;
	dpp->soebuf[dpp->soe_ptr1].day			= soe_info.day;
	dpp->soebuf[dpp->soe_ptr1].month		= soe_info.month;
	dpp->soebuf[dpp->soe_ptr1].year			= soe_info.year;

	dpp->soe_ptr1++;
	if( dpp->soe_ptr1 == ALM_BUF_SIZE )   
		dpp->soe_ptr1 = 0;  
}

		                  
typedef struct
{
	UCHAR	dev_id;
	UCHAR	data_type;
	UCHAR	module_no;
	UCHAR	ch_no;
	UCHAR	state;
	UCHAR	res1;
	UCHAR	year;
	UCHAR	month;
	UCHAR	day;
	UCHAR	hour;
	UCHAR	min;
	UCHAR	sec; 
	USHORT	ms;
} SIEMENS_SOE_INFO;

void Process_SIEMENS_SOE(LCUID lcuno,USHORT endPtr,USHORT dii_pt_num,UCHAR *soebuf, DPP_DATA *dpp, USHORT pt_num_in_module)
{
	SIEMENS_SOE_INFO	soe_info;	      
	unsigned short		i,i0,j0,soe_id;
	char				err_buf[256];

	if(sizeof(soe_info)!=14)
	{
		Err_log("soe data structure defined error");
		return;
	}

	memcpy(&soe_info, soebuf+endPtr*sizeof(soe_info), sizeof(soe_info));
//	for(i=0; i<sizeof(soe_info); i+=2) 
//		Swap2Byte(((char*)&soe_info)+i);
	Swap2Byte(&soe_info.ms);
	
	soe_id= (soe_info.module_no-1)*pt_num_in_module+soe_info.ch_no-1;
	if(soe_id<0 || soe_id>=dii_pt_num || soe_id>=DII_SIZE || soe_info.ch_no>pt_num_in_module)
	{
		sprintf(err_buf, "LCU %d.%d received soe pt_id error: module_no= %d, ch_no= %d", 
			soe_info.module_no, soe_info.ch_no);
		Err_log(err_buf);
		Err_log("module_no starts from 1, ch_no starts from 1");
		return;
	}

	if(dpp->ind_db[soe_id].iost.scanEnable != 1)
		return;

	j0=soe_id/16;
	i0=soe_id%16; 
	if(((dpp->dii_fault[j0] >> i0) & 0x01))
		return;
	if(((dpp->dii_force[j0] >> i0) & 0x01))
		return;

	if(soe_info.dev_id != lcuno.devid)
	{
		sprintf(err_buf, "LCU %d.%d received other lcu soe info dev_id= %d", 
			lcuno.stnid, lcuno.devid, soe_info.dev_id);
		Err_log(err_buf);
		return;
	}
	if(soe_info.data_type != SOE_TYPE)
	{
		sprintf(err_buf, "LCU %d.%d received soe info data_type error: %d", 
			lcuno.stnid, lcuno.devid, soe_info.data_type);
		Err_log(err_buf);
		return;
	}

	dpp->soebuf[dpp->soe_ptr1].type_id		= DPS_IND_ALM;
	dpp->soebuf[dpp->soe_ptr1].stn_id		= lcuno.stnid;
	dpp->soebuf[dpp->soe_ptr1].dev_id		= soe_info.dev_id;
	dpp->soebuf[dpp->soe_ptr1].data_type	= soe_info.data_type;
	dpp->soebuf[dpp->soe_ptr1].number		= 1;
	dpp->soebuf[dpp->soe_ptr1].di_object[0].point_id =soe_id;
	if(soe_info.state==0x80)		 
		dpp->soebuf[dpp->soe_ptr1].di_object[0].status.rt_state = 1;
	if(soe_info.state==0x00)		 
		dpp->soebuf[dpp->soe_ptr1].di_object[0].status.rt_state = 0;

	dpp->soebuf[dpp->soe_ptr1].di_object[0].status.alm_type = IND_ALM_TYPE_CHANGE;
	GetLocalHostId(&dpp->soebuf[dpp->soe_ptr1].host_id);
	dpp->soebuf[dpp->soe_ptr1].ms			= BcdToSHORT((unsigned short)(soe_info.ms>>4));
	dpp->soebuf[dpp->soe_ptr1].second		= BcdToCHAR(soe_info.sec);
	dpp->soebuf[dpp->soe_ptr1].minute		= BcdToCHAR(soe_info.min);
	dpp->soebuf[dpp->soe_ptr1].hour			= BcdToCHAR(soe_info.hour);
	dpp->soebuf[dpp->soe_ptr1].day			= BcdToCHAR(soe_info.day);
	dpp->soebuf[dpp->soe_ptr1].month		= BcdToCHAR(soe_info.month);
	dpp->soebuf[dpp->soe_ptr1].year			= BcdToCHAR(soe_info.year)+2000;

	dpp->soe_ptr1++;
	if( dpp->soe_ptr1 == ALM_BUF_SIZE )   
		dpp->soe_ptr1 = 0;  
}


typedef struct
{
	UCHAR	dev_id;
	UCHAR	data_type;
	UCHAR	module_no;
	UCHAR	ch_no;
	USHORT	ms;
	UCHAR	sec; 
	UCHAR	min;
	UCHAR	hour;
	UCHAR	state;
	UCHAR	day;
	UCHAR	month;
	USHORT	year;
} GE90_SOE_INFO;



void Process_GE90_SOE(LCUID lcuno,USHORT endPtr,USHORT dii_pt_num,UCHAR *soebuf, DPP_DATA *dpp, USHORT pt_num_in_module)
{
	GE90_SOE_INFO			soe_info;	      
	unsigned short		i,i0,j0,soe_id;
	char				err_buf[256];

	if(sizeof(soe_info)!=14)
	{
		Err_log("soe data structure defined error");
		return;
	}

	memcpy(&soe_info, soebuf+endPtr*sizeof(soe_info), sizeof(soe_info));
//	for(i=0; i<sizeof(soe_info); i+=2) //???可能存在与SIEMENS同样的问题：PLC采用了BYTE定义
//		Swap2Byte(((char*)&soe_info)+i);
	Swap2Byte(&soe_info.ms);
	Swap2Byte(&soe_info.year);

	soe_id= (soe_info.module_no-1)*pt_num_in_module+soe_info.ch_no-1;
	if(soe_id<0 || soe_id>=dii_pt_num || soe_id>=DII_SIZE || soe_info.ch_no>pt_num_in_module)
	{
		sprintf(err_buf, "LCU %d.%d received soe pt_id error: module_no= %d, ch_no= %d", 
			soe_info.module_no, soe_info.ch_no);
		Err_log(err_buf);
		Err_log("module_no starts from 1, ch_no starts from 1");
		return;
	}


	if(dpp->ind_db[soe_id].iost.scanEnable != 1)
		return;

	j0=soe_id/16;
	i0=soe_id%16; 
	if(((dpp->dii_fault[j0] >> i0) & 0x01))
		return;
	if(((dpp->dii_force[j0] >> i0) & 0x01))
		return;

	if(soe_info.dev_id != lcuno.devid)
	{
		sprintf(err_buf, "LCU %d.%d received other lcu soe info dev_id= %d", 
			lcuno.stnid, lcuno.devid, soe_info.dev_id);
		Err_log(err_buf);
		return;
	}
	if(soe_info.data_type != SOE_TYPE)
	{
		sprintf(err_buf, "LCU %d.%d received soe info data_type error: %d", 
			lcuno.stnid, lcuno.devid, soe_info.data_type);
		Err_log(err_buf);
		return;
	}

	dpp->soebuf[dpp->soe_ptr1].type_id		= DPS_IND_ALM;
	dpp->soebuf[dpp->soe_ptr1].stn_id		= lcuno.stnid;
	dpp->soebuf[dpp->soe_ptr1].dev_id		= soe_info.dev_id;
	dpp->soebuf[dpp->soe_ptr1].data_type	= soe_info.data_type;
	dpp->soebuf[dpp->soe_ptr1].number		= 1;
	dpp->soebuf[dpp->soe_ptr1].di_object[0].point_id =soe_id;
	if(soe_info.state==0x04)		 
		dpp->soebuf[dpp->soe_ptr1].di_object[0].status.rt_state = 1;
	if(soe_info.state==0x00)		 
		dpp->soebuf[dpp->soe_ptr1].di_object[0].status.rt_state = 0;

	dpp->soebuf[dpp->soe_ptr1].di_object[0].status.alm_type = IND_ALM_TYPE_CHANGE;
	GetLocalHostId(&dpp->soebuf[dpp->soe_ptr1].host_id);
	dpp->soebuf[dpp->soe_ptr1].ms			= soe_info.ms;
	dpp->soebuf[dpp->soe_ptr1].second		= soe_info.sec;
	dpp->soebuf[dpp->soe_ptr1].minute		= soe_info.min;
	dpp->soebuf[dpp->soe_ptr1].hour			= soe_info.hour;
	if(soe_info.day==0 && soe_info.month==0 && soe_info.year==0)
	{
		DATE_TIME	cur_time;
		GetSysTime(&cur_time);
		dpp->soebuf[dpp->soe_ptr1].day			= cur_time.day;
		dpp->soebuf[dpp->soe_ptr1].month		= cur_time.month;
		dpp->soebuf[dpp->soe_ptr1].year			= cur_time.year;
	}
	else
	{
		dpp->soebuf[dpp->soe_ptr1].day			= soe_info.day;
		dpp->soebuf[dpp->soe_ptr1].month		= soe_info.month;
		dpp->soebuf[dpp->soe_ptr1].year			= soe_info.year;
	}

	dpp->soe_ptr1++;
	if( dpp->soe_ptr1 == ALM_BUF_SIZE )   
		dpp->soe_ptr1 = 0;  
}


typedef struct
{
	UCHAR	dev_id;
	UCHAR	data_type;
	UCHAR	module_no;
	UCHAR	ch_no;
	UCHAR	state;
	UCHAR	res;
	UCHAR	uct[8];
} ABCONTROL_SOE_INFO;

void Process_ABCONTROL_SOE(LCUID lcuno,USHORT endPtr,USHORT dii_pt_num,UCHAR *soebuf, DPP_DATA *dpp, USHORT pt_num_in_module)
{
	ABCONTROL_SOE_INFO	soe_info;	      
	unsigned short		i,i0,j0,soe_id,ms;
	time_t				time_sec;
	struct tm			time_tm;
#ifdef WINDOWS_OS
	INT64				uct_time;
#else
	long long			uct_time;
#endif
	char				err_buf[256];

	if(sizeof(soe_info)!=14)
	{
		Err_log("soe data structure defined error");
		return;
	}
	
	memcpy(&soe_info, soebuf+endPtr*sizeof(soe_info), sizeof(soe_info));
	

#ifdef	_BIG_ENDIAN_
	for(i=0;i<4;i++)
	{
		UCHAR	temp;
		temp= soe_info.uct[i];
		soe_info.uct[i]= soe_info.uct[7-i];
		soe_info.uct[7-i]= temp;
	}
#endif 

	
	soe_id= soe_info.module_no*pt_num_in_module+soe_info.ch_no;
	if(soe_id<0 || soe_id>=dii_pt_num || soe_id>=DII_SIZE || soe_info.ch_no>=pt_num_in_module)
	{
		sprintf(err_buf, "LCU %d.%d received soe pt_id error: module_no= %d, ch_no= %d", 
			soe_info.module_no, soe_info.ch_no);
		Err_log(err_buf);
		return;
	}
	if(dpp->ind_db[soe_id].iost.scanEnable != 1)
		return;

	j0=soe_id/16;
	i0=soe_id%16; 
	if(((dpp->dii_fault[j0] >> i0) & 0x01))
		return;
	if(((dpp->dii_force[j0] >> i0) & 0x01))
		return;
	
	if(soe_info.dev_id != lcuno.devid)
	{
		sprintf(err_buf, "LCU %d.%d received other lcu soe info dev_id= %d", 
			lcuno.stnid, lcuno.devid, soe_info.dev_id);
		Err_log(err_buf);
		return;
	}
	if(soe_info.data_type != SOE_TYPE)
	{
		sprintf(err_buf, "LCU %d.%d received soe info data_type error: %d", 
			lcuno.stnid, lcuno.devid, soe_info.data_type);
		Err_log(err_buf);
		return;
	}
	
	dpp->soebuf[dpp->soe_ptr1].type_id		= DPS_IND_ALM;
	dpp->soebuf[dpp->soe_ptr1].stn_id		= lcuno.stnid;
	dpp->soebuf[dpp->soe_ptr1].dev_id		= soe_info.dev_id;
	dpp->soebuf[dpp->soe_ptr1].data_type	= soe_info.data_type;
	dpp->soebuf[dpp->soe_ptr1].number		= 1;
	dpp->soebuf[dpp->soe_ptr1].di_object[0].point_id =soe_id;
	dpp->soebuf[dpp->soe_ptr1].di_object[0].status.rt_state = soe_info.state & 0x01;				   
	dpp->soebuf[dpp->soe_ptr1].di_object[0].status.alm_type = IND_ALM_TYPE_CHANGE;
	GetLocalHostId(&dpp->soebuf[dpp->soe_ptr1].host_id);

	memcpy(&uct_time, soe_info.uct, sizeof(uct_time));
	
	uct_time=uct_time-28800000000; /*8 hours*/
	
	ms= (unsigned short)((uct_time/1000)%1000);
	time_sec= (time_t)(uct_time/1000000);
	time_tm= *localtime(&time_sec);
	dpp->soebuf[dpp->soe_ptr1].ms			= ms;
	dpp->soebuf[dpp->soe_ptr1].second		= time_tm.tm_sec;
	dpp->soebuf[dpp->soe_ptr1].minute		= time_tm.tm_min;
	dpp->soebuf[dpp->soe_ptr1].hour			= time_tm.tm_hour;
	dpp->soebuf[dpp->soe_ptr1].day			= time_tm.tm_mday;
	dpp->soebuf[dpp->soe_ptr1].month		= time_tm.tm_mon+1;
	dpp->soebuf[dpp->soe_ptr1].year			= time_tm.tm_year+1900;
	
	dpp->soe_ptr1++;
	if( dpp->soe_ptr1 == ALM_BUF_SIZE )   
		dpp->soe_ptr1 = 0;  
}




/*#define	QUANTUM_SOE_PT_NUM_IN_MODULE	32
#define	SIEMENS_SOE_PT_NUM_IN_MODULE	16
#define	GE9030_SOE_PT_NUM_IN_MODULE		32
#define	GE9070_SOE_PT_NUM_IN_MODULE		16
#define	ABCONTROL_SOE_PT_NUM_IN_MODULE	16
*/ //cxs 20110607
void Process_SOE(LCUID lcuno,USHORT endPtr,USHORT dii_pt_num,UCHAR *soebuf, DPP_DATA *dpp)
{
	char	lcu_type[36];
	char	err_buf[256];
	
	if(-1==GetGroupTypeNameById( lcuno.stnid,lcuno.devid, lcu_type ))
		return;
	
	if(strstr(lcu_type,"QUANTUM") || strstr(lcu_type,"PREMIUM")) 	
		Process_QUANTUM_SOE(lcuno,endPtr,dii_pt_num,soebuf,dpp, QUANTUM_SOE_PT_NUM_IN_MODULE);
	else if(strstr(lcu_type,"SIEMENS"))
		Process_SIEMENS_SOE(lcuno,endPtr,dii_pt_num,soebuf,dpp, SIEMENS_SOE_PT_NUM_IN_MODULE);
	else if(strstr(lcu_type,"GE9030"))
		Process_GE90_SOE(lcuno,endPtr,dii_pt_num,soebuf,dpp, GE9030_SOE_PT_NUM_IN_MODULE);
	else if(strstr(lcu_type,"GE9070") || !strcmp(lcu_type, "GE"))
		Process_GE90_SOE(lcuno,endPtr,dii_pt_num,soebuf,dpp, GE9070_SOE_PT_NUM_IN_MODULE);
	else if(strstr(lcu_type,"ABCONTROL"))
		Process_ABCONTROL_SOE(lcuno,endPtr,dii_pt_num,soebuf,dpp, ABCONTROL_SOE_PT_NUM_IN_MODULE);
	else
	{
		sprintf(err_buf, "LCU %d.%d type error: %s", lcuno.stnid, lcuno.devid, lcu_type);
		Err_log(err_buf);
	}
}
	        

