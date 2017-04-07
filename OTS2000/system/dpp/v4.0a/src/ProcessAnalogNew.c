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



typedef struct 
{
	USHORT				point_id;
	ANLG_STATUS			status;
	GLONG				data;
} ANA_DATA_UNIT;

typedef struct
{
	IOST			iost;	
	IOATTR      	ioattr;	
	ANLG_STATUS		status;
	GLONG			data;
	float			fValue;
	float			d_band;
	float			hh_lim;
	float			h_lim;
	float			hr_lmt;
	float			ll_lim;
	float			l_lim;
	float			lr_lmt;
	float			hi_trlmt;
	float			hr_trlmt;
	float			eng_cvt;
	float			hi_range;
	float			lo_range;
	float			hi_code;
	float			lo_code;
	float			zero;
	float			compensate;
	UCHAR			hh_flag;
	UCHAR			h_flag;
	UCHAR			l_flag;
	UCHAR			ll_flag;
}ANA_PARAM_TABLE;


typedef struct
{
	ANA_DATA_UNIT		*norm_unit_ptr;
	ANA_DATA_UNIT		*alm_unit_ptr;

	USHORT				lmt_state, lmt_state_old;
	USHORT				overflow, overflow_old;
	USHORT				fault, fault_old;
	USHORT				force, force_old;

	USHORT				code_value;
	USHORT				vary_flag;
	float				value, value_old;      

	
	DATE_TIME			ai_time;


	USHORT				ai_ptr1,ai_ptr2;  
	USHORT				aialm_ptr1,aialm_ptr2; 

	UCHAR 				ana_update_flag;
	UCHAR				ai_allsend_flag;
	UCHAR				first_scan;
	
	USHORT				trendAlm_old;
	USHORT				trend_id;
	TREND_STRUCT		trend;
	
			
}ANA_DATA_TABLE;


int	FillAnaDataMsg(POINTER *pt, float value, ANLG_STATUS status, USHORT alm_type, USHORT state, ANA_DATA_UNIT *buff)
{
	memset(buff, 0, sizeof(ANA_DATA_UNIT));

	buff->point_id= pt->pt_id;
	buff->data = (int)(value*1000.);	
	status.alm_type  = alm_type;
	switch(alm_type)
	{
	case	ANA_ALM_TYPE_CHANFAIL:
		status.chan_fail= state;
		break;
	case	ANA_ALM_TYPE_CTRLBLOCK:
		status.ctrl_block= state;
		break;
	case	ANA_ALM_TYPE_OVERFLOW:
		status.overflow= state;
		break;
	case	ANA_ALM_TYPE_NORM:
	case	ANA_ALM_TYPE_LMT:
		status.lmt_state= state;
		break;
	case	ANA_ALM_TYPE_TR:
		status.trend_state= state;
		break;
	default:
		return -1;
	}
	
	buff->status= status;
	return 0;
}


void AnaPtDataMsgPro(POINTER *pt, ANA_PARAM_TABLE *param_table, ANA_DATA_TABLE *dpp, DATE_TIME *tm)
{
	ANA_DATA_UNIT		ana_norm_unit[10];		/* normal change array */
	ANA_DATA_UNIT		ana_alm_unit[10];		/* alarm change array */
	int					alm_unit_num=0, norm_unit_num=0;
	
	ANLG_STATUS			status;
	USHORT				real_lmt_state= dpp->lmt_state;
	
	status= param_table->status;
	
	if(dpp->fault != dpp->fault_old)
		FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_CHANFAIL, dpp->fault, &ana_alm_unit[alm_unit_num++]);

	if(dpp->force != dpp->force_old)
		FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_CTRLBLOCK, dpp->force, &ana_alm_unit[alm_unit_num++]);
	
	/*****  AI overflow NOT alarm, but change database,modified by ycx 070529 ******/ 		
	if(dpp->fault==0 && dpp->overflow!=dpp->overflow_old)
	{
		status.overflow= dpp->overflow;
		FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_NORM, ANA_ALM_NORMAL, &ana_norm_unit[norm_unit_num++]);
		if(dpp->overflow==0)
			dpp->lmt_state =0;	/******** add by ycx 070529 ******/
	}

	dpp->lmt_state_old= dpp->lmt_state;
	
	/******  AI  normal change send when channel fault or force value *******/
	if(dpp->fault_old==1 && dpp->fault == 1) 
	{
		if(dpp->vary_flag)
		{
			status.chan_fail= dpp->fault;
			status.ctrl_block= dpp->force;
			status.overflow= dpp->overflow;
			FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_NORM, ANA_ALM_NORMAL, &ana_norm_unit[norm_unit_num++]);
		}
	}

	if(dpp->vary_flag==0)
		return;
	
	/******  AI channel is always good *******/
	if(dpp->fault==0 && dpp->fault_old==0 && dpp->overflow==0)
	{
		if(dpp->lmt_state_old==ANA_ALM_NORMAL) //原来正常
		{
			if(dpp->value > param_table->hh_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_WARN, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_ALARM, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_HI_ALARM;
			}
			else if(dpp->value > param_table->h_lim) 
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_WARN, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_HI_WARN;
			}
			else if(dpp->value >= param_table->l_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_NORM, ANA_ALM_NORMAL, &ana_norm_unit[norm_unit_num++]);
				dpp->lmt_state = ANA_ALM_NORMAL;				
			}
			else if(dpp->value >= param_table->ll_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_WARN, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_LO_WARN;
			}
			else //if(dpp->value < param_table->ll_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_WARN, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_ALARM, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_LO_ALARM;
			}
		}
		else if(dpp->lmt_state_old==ANA_ALM_HI_WARN) //原来越上限
		{
			if(dpp->value > param_table->hh_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_ALARM, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_HI_ALARM;
			}
			else if(dpp->value > (param_table->h_lim-param_table->hr_lmt)) 
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_NORM, ANA_ALM_HI_WARN, &ana_norm_unit[norm_unit_num++]);
				dpp->lmt_state = ANA_ALM_HI_WARN;
			}
			else if(dpp->value >= param_table->l_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_RECOVER, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_NORMAL;				
			}
			else if(dpp->value >= param_table->ll_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_WARN, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_LO_WARN;				
			}
			else //if(dpp->value < param_table->ll_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_WARN, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_ALARM, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_LO_ALARM;				
			}
		}
		else if(dpp->lmt_state_old==ANA_ALM_HI_ALARM) //原来越上上限
		{
			if(dpp->value > (param_table->hh_lim-param_table->hr_lmt))
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_NORM, ANA_ALM_HI_ALARM, &ana_norm_unit[norm_unit_num++]);
				dpp->lmt_state = ANA_ALM_HI_ALARM;
			}
			else if(dpp->value > (param_table->h_lim-param_table->hr_lmt))
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HH_RECOVER, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_HI_WARN;
			}
			else if(dpp->value >= param_table->l_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HH_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_RECOVER, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_NORMAL;
			}
			else if(dpp->value >= param_table->ll_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HH_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_WARN, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_LO_WARN;
			}
			else //if(dpp->value < param_table->ll_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HH_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_WARN, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_ALARM, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_LO_ALARM;
			}			
		}
		else if(dpp->lmt_state_old==ANA_ALM_LO_WARN) //原来越下限
		{
			if(dpp->value < param_table->ll_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_ALARM, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_LO_ALARM;
			}
			else if(dpp->value < (param_table->l_lim+param_table->lr_lmt)) 
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_NORM, ANA_ALM_LO_WARN, &ana_norm_unit[norm_unit_num++]);
				dpp->lmt_state = ANA_ALM_LO_WARN;
			}
			else if(dpp->value <= param_table->h_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_RECOVER, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_NORMAL;				
			}
			else if(dpp->value <= param_table->hh_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_WARN, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_HI_WARN;				
			}
			else //if(dpp->value > param_table->hh_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_WARN, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_ALARM, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_HI_ALARM;				
			}
		}
		else if(dpp->lmt_state_old==ANA_ALM_LO_ALARM) //原来越下下限
		{
			if(dpp->value < (param_table->ll_lim+param_table->lr_lmt))
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_NORM, ANA_ALM_LO_ALARM, &ana_norm_unit[norm_unit_num++]);
				dpp->lmt_state = ANA_ALM_LO_ALARM;
			}
			else if(dpp->value < (param_table->l_lim+param_table->lr_lmt))
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LL_RECOVER, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_LO_WARN;
			}
			else if(dpp->value <= param_table->h_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LL_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_RECOVER, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_NORMAL;
			}
			else if(dpp->value <= param_table->hh_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LL_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_WARN, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_HI_WARN;
			}
			else //if(dpp->value > param_table->hh_lim)
			{
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LL_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_RECOVER, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_WARN, &ana_alm_unit[alm_unit_num++]);
				FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_ALARM, &ana_alm_unit[alm_unit_num++]);
				dpp->lmt_state = ANA_ALM_HI_ALARM;
			}			
		}
	}	//end of if(dpp->fault==0 && dpp->fault_old==0 && dpp->overflow==0)


	
	 /******  AI channel is from fault to good *******/

	if(dpp->fault==0 && dpp->fault_old!=0 && dpp->overflow==0)
	{
		if(dpp->value > param_table->hh_lim)
		{
			FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_WARN, &ana_alm_unit[alm_unit_num++]);
			FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_ALARM, &ana_alm_unit[alm_unit_num++]);
			dpp->lmt_state = ANA_ALM_HI_ALARM;
		}
		else if(dpp->value > param_table->h_lim) 
		{
			FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_HI_WARN, &ana_alm_unit[alm_unit_num++]);
			dpp->lmt_state = ANA_ALM_HI_WARN;
		}
		else if(dpp->value >= param_table->l_lim)
		{
			FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_NORM, ANA_ALM_NORMAL, &ana_norm_unit[norm_unit_num++]);
			dpp->lmt_state = ANA_ALM_NORMAL;				
		}
		else if(dpp->value >= param_table->ll_lim)
		{
			FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_WARN, &ana_alm_unit[alm_unit_num++]);
			dpp->lmt_state = ANA_ALM_LO_WARN;
		}
		else //if(dpp->value < param_table->ll_lim)
		{
			FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_WARN, &ana_alm_unit[alm_unit_num++]);
			FillAnaDataMsg(pt, dpp->value, status, ANA_ALM_TYPE_LMT, ANA_ALM_LO_ALARM, &ana_alm_unit[alm_unit_num++]);
			dpp->lmt_state = ANA_ALM_LO_ALARM;
		}
	}
}



void AnaPtDataPro(POINTER *pt, ANA_DATA_TABLE *dpp, DATE_TIME *tm)
{
	float				xcode;
	float				value_diff;
	ANA_PARAM_TABLE		param_table;

	//init param_table() wait

	
	if(param_table.lo_code<0) /*dpp->ana_new should be signed */
		xcode= (short)dpp->code_value;
	else
		xcode= dpp->code_value;
	
	if(xcode > param_table.hi_code)	 
		dpp->value= param_table.hi_range; 
	else if(xcode < param_table.lo_code) 
		dpp->value= param_table.lo_range; 
	else
		dpp->value=(( xcode- param_table.lo_code ) * (param_table.hi_range - param_table.lo_range)
				/(param_table.hi_code- param_table.lo_code)+param_table.lo_range)* (param_table.eng_cvt) 
				+ (param_table.compensate) + (param_table.zero);

                            
	if(dpp->fault ==0)
	{
		if( xcode > param_table.hi_code || xcode < param_table.lo_code)	  
			dpp->overflow=1;		
		else	
			dpp->overflow=0;
	}
	else   
		dpp->overflow=0;
			
	if(dpp->overflow==0)
	{
		if(dpp->value <= param_table.h_lim && dpp->value >= param_table.l_lim)
			dpp->lmt_state = ANA_ALM_NORMAL;
		else if(dpp->value > param_table.h_lim && dpp->value <= param_table.hh_lim) 
			dpp->lmt_state = ANA_ALM_HI_WARN;
		else if(dpp->value < param_table.l_lim && dpp->value >= param_table.ll_lim)
			dpp->lmt_state = ANA_ALM_LO_WARN;
		else if(dpp->value > param_table.hh_lim)
			dpp->lmt_state = ANA_ALM_HI_ALARM;
		else
			dpp->lmt_state = ANA_ALM_LO_ALARM;
	}
	else
		dpp->lmt_state= ANA_ALM_NORMAL;

	value_diff= fabs(dpp->value - param_table.fValue);
	if(value_diff>0 && value_diff>=param_table.d_band)
		dpp->vary_flag= 1;
	else
		dpp->vary_flag= 0;


	if(param_table.iost.scanEnable ==1)
	{
		if(!dpp->first_scan)
			AnaPtDataMsgPro(pt, &param_table, dpp, tm);

		/*************************** Analog input TREND alarm form processing ********************************************/
		AnaPtTrendAlarmPro(pt, dpp, tm);
		
		
		
		/********************************************* combine  aialm change compacket **********************************************************************/	
		/********************************************* combine  ai change compacket **********************************************************************/	
		
		dpp->value_old = dpp->value; 
		dpp->fault_old = dpp->fault;
		dpp->force_old = dpp->force;
		dpp->overflow_old = dpp->overflow;
		dpp->lmt_state_old = dpp->lmt_state;		
	}		
}



/*
int AnaPtTrendAlarmPro(POINTER *pt, ANA_DATA_TABLE *dpp, DATE_TIME *tm)
{      
	USHORT i,j,i0,j0,num,alm_unit_num,m,n,off,wordlen;
	float z;
	float xcode;
	
	USHORT         fault,force,fault_old,force_old;  
	AISTRUCT		ana_norm_unit[alm_unit_num];     / * normal change array * /
	AISTRUCT		ana_alm_unit;   / * alarm change array * /
	
	struct tm  atime;
	time_t     atime_t;
	float	    dt;
	float      grad[SAMPLE_NUM];

	atime.tm_sec = tm->second;
	atime.tm_min = tm->minute;
	atime.tm_hour= tm->hour;
	atime.tm_mday= tm->day;
	atime.tm_mon = tm->month;
	atime.tm_year= tm->year - 1900;
	atime.tm_isdst = 0;
	atime_t = mktime(&atime);
	
	if(!dpp->first_scan)
	{
	
			if((param_table->ioattr.trendAlm ==0)&&(dpp->trendAlm_old==1))	/ * trendAlm flag modified by oix * /
			{
				dpp->trendAlm_old =0;
				dpp->trend_id=0;
			}
	
	
			
			if(param_table->ioattr.trendAlm ==1)	
			{	 		
		 	
				fault = (dpp->ai_fault[j0] >> i0) & 0x01;  fault_old = (dpp->ai_fault_old[j0] >> i0) & 0x01;
		
    				/ *force = (dpp->ai_force[j0] >> i0) & 0x01;  force_old = (dpp->ai_force_old[j0] >> i0) & 0x01;* /

			 	if(dpp->trend_id == 0xffff && fault ==0 && fault_old ==0)
			 	{
					/ *printf("\nttttttttttttttttttttttt,i=%d\n",i);* /
				
					/ * slide data and time * /			
					dpp->value0 = dpp->value1; 
					dpp->value1 = dpp->value2; 
					dpp->value2 = dpp->value3; 
					dpp->value3 = dpp->value4; 
					dpp->value4 = dpp->value;
			
					dpp->tm0 = dpp->tm1;
					dpp->tm1 = dpp->tm2;
					dpp->tm2 = dpp->tm3;
					dpp->tm3 = dpp->tm4 ;
					dpp->tm4 = atime_t;
				
					dpp->ms0 = dpp->ms1;
					dpp->ms1 = dpp->ms2;
					dpp->ms2 = dpp->ms3;
					dpp->ms3 = dpp->ms4;
					dpp->ms4 = tm->ms;
			
					/ * move data and time to calcu grad * /
			
					dpp->trend.value[0] = dpp->value0;
					dpp->trend.value[1] = dpp->value1;
					dpp->trend.value[2] = dpp->value2;
					dpp->trend.value[3] = dpp->value3;
					dpp->trend.value[4] = dpp->value4;
			
					dpp->trend.ai_tm[0] = dpp->tm0;
					dpp->trend.ai_tm[1] = dpp->tm1;
					dpp->trend.ai_tm[2] = dpp->tm2;
					dpp->trend.ai_tm[3] = dpp->tm3;
					dpp->trend.ai_tm[4] = dpp->tm4;	
							
					dpp->trend.ms[0]=dpp->ms0 ;
			 		dpp->trend.ms[1]=dpp->ms1 ;
			 		dpp->trend.ms[2]=dpp->ms2 ;
			 		dpp->trend.ms[3]=dpp->ms3 ;
			 		dpp->trend.ms[4]=dpp->ms4 ;

					for(j=0;j<SAMPLE_NUM-1;j++)
		
					{
						dt =((float)dpp->trend.ai_tm[j+1]+((float)dpp->trend.ms[j+1]/1000.0))-((float)dpp->trend.ai_tm[j]+((float)dpp->trend.ms[j]/1000.0));
					
						/ *printf("tm2=%d,tm1=%d,ms2=%d,ms1=%d,dt=%f\n",dpp->trend.ai_tm[j+1],dpp->trend.ai_tm[j],dpp->trend.ms[j+1],dpp->trend.ms[j],dt);* /
					
						grad[j] = (dpp->trend.value[j+1] - dpp->trend.value[j])/dt;
					
						/ *printf("v2=%f,v1=%f\n",dpp->trend.value[j+1],dpp->trend.value[j]);* /
					
						/ *printf("grad[%d]=%f  ,dt=%f\n",j,grad[j],dt);* /
					}
			
					dpp->trend.t_value = (float)FACTOR4 * grad[3] + (float)FACTOR3 * grad[2] + (float)FACTOR2 * grad[1] + (float)FACTOR1 * grad[0];
				
					/ *dpp->trend.t_value = (float)(fabs((double)dpp->trend.t_value ));* /						
				
					/ *printf("hi_trlmt=%f,t_value=%f,t_value_old=%f\n",param_table->hi_trlmt,dpp->trend.t_value,dpp->trend.t_value_old);* /
				
					if ((dpp->trend.t_flag == 0)&&(dpp->trend.t_value > param_table->hi_trlmt)&&(dpp->trend.t_value_old > param_table->hi_trlmt))
					{
						/ *printf("\n %d over-trend-limit alarm,t_value=%f\n",i,dpp->trend.t_value);* /
					
						ana_alm_unit[alm_unit_num].point_id = i;
						ana_alm_unit[alm_unit_num].data   = (int)(dpp->trend.t_value*1000);
					
						memcpy(&(status),&(param_table->status),2);
						status.alm_type    =  ANA_ALM_TYPE_TR;
						status.trend_state =  ANA_ALM_TR_WARN;
			
						dpp->trend.t_flag = 1;
	    	        
	    	        			alm_unit_num++;
					}

					if((dpp->trend.t_flag == 1)&&(dpp->trend.t_value < (param_table->hi_trlmt-param_table->hr_trlmt)))
					{
						/ *printf("\n %d  back-trend-limit alarm,t_value=%f\n",i,dpp->trend.t_value);* /
					
						ana_alm_unit[alm_unit_num].point_id = i;
						ana_alm_unit[alm_unit_num].data   = (int)(dpp->trend.t_value*1000);
					
						memcpy(&(status),&(param_table->status),2);
						status.alm_type    =  ANA_ALM_TYPE_TR;
						status.trend_state =  ANA_ALM_TR_RECOVER;
			
						dpp->trend.t_flag = 0;
	    	        
	    	        			alm_unit_num++;
	    	 			}
	    	 		
	    	 			dpp->trend.t_value_old = dpp->trend.t_value;
	    	 	 	}	
		 	}
	 	
		 	
			fault = (dpp->ai_fault[j0] >> i0) & 0x01;  / *fault_old = (dpp->ai_fault_old[j0] >> i0) & 0x01;* /

			if((param_table->ioattr.trendAlm ==1)&&(dpp->trendAlm_old==0) && fault==0)	
			{
				if(dpp->trend_id < SAMPLE_NUM)
				{
			
					dpp->trend.value[dpp->trend_id]= dpp->value;
			
					dpp->trend.ai_tm[dpp->trend_id] = atime_t;
				
					dpp->trend.ms[dpp->trend_id] = tm->ms;
				}
			
				dpp->trend_id++;
			}
		
	
			if(param_table->ioattr.trendAlm ==1)	
			{	

		      		if(dpp->trend_id == SAMPLE_NUM)
		      		{
		      			/ *printf("\n00000000000000000000000,i=%d\n",i);* /
		      	
					dpp->value0 = dpp->trend.value[0];
					dpp->value1 = dpp->trend.value[1];
					dpp->value2 = dpp->trend.value[2];
					dpp->value3 = dpp->trend.value[3];
					dpp->value4 = dpp->trend.value[4];
				
					dpp->tm0 = dpp->trend.ai_tm[0];
					dpp->tm1 = dpp->trend.ai_tm[1];
					dpp->tm2 = dpp->trend.ai_tm[2];
					dpp->tm3 = dpp->trend.ai_tm[3];
					dpp->tm4 = dpp->trend.ai_tm[4];
			
					dpp->ms0 = dpp->trend.ms[0];
					dpp->ms1 = dpp->trend.ms[1];
					dpp->ms2 = dpp->trend.ms[2];
					dpp->ms3 = dpp->trend.ms[3];
					dpp->ms4 = dpp->trend.ms[4];
			
					dpp->trend_id= 0xffff;
					dpp->trendAlm_old = 1;
		      
		        		for(j=0;j<SAMPLE_NUM-1;j++)
		
					{
						dt =((float)dpp->trend.ai_tm[j+1]+((float)dpp->trend.ms[j+1]/1000.0))-((float)dpp->trend.ai_tm[j]+((float)dpp->trend.ms[j]/1000.0));				
				
						/ *printf("tm2=%d,tm1=%d,ms2=%d,ms1=%d,dt=%f\n",dpp->trend.ai_tm[j+1],dpp->trend.ai_tm[j],dpp->trend.ms[j+1],dpp->trend.ms[j],dt);* /
				
						grad[j] = (dpp->trend.value[j+1] - dpp->trend.value[j])/dt;
					
						/ *printf("v2=%f,v1=%f\n",dpp->trend.value[j+1],dpp->trend.value[j]);
						printf("grad[%d]=%f,dt=%f\n",j,grad[j],dt);* /
					}
			
					dpp->trend.t_value = (float)FACTOR4 * grad[3] + (float)FACTOR3 * grad[2] + (float)FACTOR2 * grad[1] + (float)FACTOR1 * grad[0];
			
					/ *dpp->trend.t_value = (float)(fabs((double)dpp->trend.t_value ));* /
			
					/ *printf("t_value=%f\n\n",dpp->trend.t_value);* /
	             		}					
			}
		
	}/ *end of if* /
	
}*/
