

/**************************************************
2007.06.22 : have been SYNC  by chichi;
LAST UPATE : 2007.06.22 by chichi ; 
last SYNC by hcl 2007.11.28
***************************************************/

#include <gtk/gtk.h>
#include	"../inc/gtk_ipm_k_def.h"

#include	"../inc/oix.h"
#include        "../../../dps/v4.0a/inc/dps_functions_def.h"
#include	"../inc/gtk_alarm_tab.h"

#define	DEBUG
extern BOOL  isStnInMask (UCHAR stn_id) ; 


int	alm_rec_fault(FILTER_T filter, INDEX *index)
{
	SHORT		rec_ptr;
	POINTER		point;
	GULONG		normalState;
	USHORT		almNoAck;
	USHORT		state;
	USHORT		count_rec;
	BOOL		faultPt, failPt;
	REC_ENTRY_UNION *rec;
	char		err_str[128];
	
	index->count= 0;      
	
	count_rec= filter.phead->count;
	if(count_rec > filter.phead->buf_size)
	{
		sprintf(err_str, "Event table count overrange: %d", count_rec);
		Err_log(err_str);
		return -1;
	}
	
	for(rec_ptr=filter.phead->header_ptr; count_rec>0; count_rec--,rec_ptr--,rec_ptr=(rec_ptr>=0)? rec_ptr:(filter.phead->buf_size-1) )
	{
		rec= &filter.pRec[rec_ptr];
		if(rec->ana_rec.type_id!=DPS_ANA_ALM && rec->ana_rec.type_id!=DPS_IND_ALM)
			continue;
		
		if(-1==GetPointFromRec(rec, &point))
			continue;
		
		if(-1==IsPtFaultById(&point, &faultPt))
			continue;
		
		if(!faultPt)
		{
			IsPtFailById(&point, &failPt);
			if(!failPt)
				continue;
		}
		IsPtAlmNotAckedById(&point, &almNoAck);
		if(!almNoAck)
			continue;

		if  (isStnInMask(rec->ana_rec.stn_id)==0)  //ADDED BY CHI 2010-01-11
			continue ;
		
		if(rec->ana_rec.type_id==DPS_ANA_ALM)
		{
			if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_LMT)
			{
				state= rec->ana_rec.status.ana_status.lmt_state;
				if(state==ANA_ALM_HI_ALARM || state==ANA_ALM_HI_WARN
					|| state==ANA_ALM_LO_WARN || state==ANA_ALM_LO_ALARM)
				{
					index->index_pt[index->count++].pt= rec_ptr;
					if (index->count >= index->buff_size) 
						return 0;
				}
			}
			else if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_TR)
			{
				if(rec->ana_rec.status.ana_status.trend_state==ANA_ALM_TR_WARN)
				{
					index->index_pt[index->count++].pt= rec_ptr;
					if (index->count >= index->buff_size) 
						return 0;
				}
			}
			else if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_CTRLBLOCK)
			{
				if(rec->ana_rec.status.ana_status.ctrl_block)
				{
					index->index_pt[index->count++].pt= rec_ptr;
					if (index->count >= index->buff_size) 
						return 0;
				}
			}
			else if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_CHANFAIL)
			{
				if(rec->ana_rec.status.ana_status.chan_fail)
				{
					index->index_pt[index->count++].pt= rec_ptr;
					if (index->count >= index->buff_size) 
						return 0;
				}
			}
			else if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_OVERFLOW)
			{
				if(rec->ana_rec.status.ana_status.overflow)
				{
					index->index_pt[index->count++].pt= rec_ptr;
					if (index->count >= index->buff_size) 
						return 0;
				}
			}
		}
		else if(rec->ana_rec.type_id==DPS_IND_ALM)
		{
			if(rec->ind_rec.ind_status.alm_type==IND_ALM_TYPE_CHANGE)
			{
				GetPtNormalStateById(&point, &normalState);
				if(rec->ind_rec.ind_status.rt_state!=normalState)
				{
					index->index_pt[index->count++].pt= rec_ptr;
					if (index->count >= index->buff_size) 
						return 0;
				}
			}
			else if(rec->ind_rec.ind_status.alm_type==IND_ALM_TYPE_MANFORCE)
			{
				if(rec->ind_rec.ind_status.man_set)
				{
					index->index_pt[index->count++].pt= rec_ptr;
					if (index->count >= index->buff_size) 
						return 0;
				}
			}
			else if(rec->ind_rec.ind_status.alm_type==IND_ALM_TYPE_CHANFAIL)
			{
				if(rec->ind_rec.ind_status.chan_fail)
				{
					index->index_pt[index->count++].pt= rec_ptr;
					if (index->count >= index->buff_size) 
						return 0;
				}
			}
		}
	}
	
	return 0;
}


BOOL IsMessageRec(REC_ENTRY_UNION *rec)
{
	if(rec->ana_rec.type_id == DPS_LMS || rec->ana_rec.type_id == DPS_SMS 
		|| rec->ana_rec.type_id == DPS_SM_ALM)
		return TRUE;
	else
		return FALSE;
}

int	alm_rec_filter(FILTER_T filter, INDEX *index)
{
    USHORT		i, count_rec, start_rec,almNoAck;
	time_t		time;
	ALM_STR		alm_str;
	BOOL		blink;
	COLOR_PAIR	color;
	char		err_str[128];
	POINTER  pt ;
	POINT_NAME pt_name;
	char            logName[64];
	USHORT		io_group;
	DMS_COMMON	alm_commom;

	index->count= 0;
	/*if(filter.hist_flag)
	return -1;
	*/
	if( filter.phead->buf_size == 0) 
	{  printf(" OIX: Alarm buffer size is Zeor  !!!! \n ");
	return 0 ;
	}
	count_rec= filter.phead->count;
	if(count_rec > filter.phead->buf_size)
	{
		sprintf(err_str, "Event table count overrange: %d", count_rec);
		Err_log(err_str);
		return -1;
	}
	
	start_rec= (filter.phead->header_ptr+ filter.phead->buf_size-count_rec)%filter.phead->buf_size;
	
	for(i=start_rec; count_rec>0; count_rec--,i++,i=(i>=filter.phead->buf_size)? 0:i )
	{       
		if (IsMessageRec(&filter.pRec[i]))
		{
			if (strstr(filter.pRec[i].sms_rec.message,"pt_id:") ||
				strstr(filter.pRec[i].sms_rec.message,"switch:"))
				continue;		
		}
		if(filter.cmd_type == 1)  /* <=  cmd type */
		{
			if(filter.pRec[i].ana_rec.type_id <= 23 ) /* command type */
				continue ;
		}
        if (filter.isNoAck  )
		{
			if(-1!= GetPointFromRec(&filter.pRec[i], &pt)  )
				if (-1 != IsPtAlmNotAckedById(  &pt, &almNoAck )) 			 
					if(!almNoAck)  //acknoleged!
						continue;
		}

		if (filter.cmd_type == 2) /* short or long message */
		{
			if(!IsMessageRec(&filter.pRec[i]))
				continue ;   
		}
		
		if(IsMessageRec(&filter.pRec[i]))
		{
			if (strcmp(filter.LogicName,"") != 0) /* LogicName!! */
				continue;
			if(filter.dev_mask[0]!=(UINT)-1)
				continue;
			if(filter.datatype_mask!=(UINT)-1)
				continue;
			goto  ADD ;   
		}
		
		if (strcmp(filter.LogicName,"") != 0) /* LogicName!! */
		{
			if(IsMessageRec(&filter.pRec[i]))
				continue ;   
			if(-1!= GetPointFromRec(&filter.pRec[i], &pt)  )
			{
				if (-1 != GetPtNameById(  &pt, logName, &pt_name )) 
				{
					if(!strstr(logName, filter.LogicName))
						continue;
				}
			}
		}
		
		if(filter.stn_id!=0)
		{
			if(filter.stn_id!= filter.pRec[i].ana_rec.stn_id)
				continue;
		}
		if(isStnInMask(filter.pRec[i].ana_rec.stn_id)==0)  //ADDED BY CHI 2010-01-11
			continue ;  
		
		if(filter.dev_mask[0]!=(UINT)-1)
		{
			if(!(0x01 & (filter.dev_mask[0]>>(filter.pRec[i].ana_rec.dev_id-1))))
				continue;
		}
		
		if(GetPointFromRec(&filter.pRec[i], &pt)!=-1)	//added by dm
		{
			if (-1!=GetPtIoGroupById(&pt,&io_group))	
			{
				if (filter.group_mask[filter.pRec[i].ana_rec.dev_id-1]!=(UINT)-1)
				{
					if(!(0x01 & (filter.group_mask[filter.pRec[i].ana_rec.dev_id-1]>>(io_group-1))))
						continue;
				}
			}
		}
		
		if(filter.datatype_mask!=(UINT)-1)
		{
			if(!(0x01 & (filter.datatype_mask>>(filter.pRec[i].ana_rec.data_type-1))))
				continue;
		}
		
		if(GetPointFromRec(&filter.pRec[i], &pt)!=-1)
		{
			if(ReadEntryById(&pt, &alm_commom)!=-1)
			{
				if (filter.attrib_mask[0]!=(UINT)-1 && filter.attrib_mask[1]!=(UINT)-1)
				{
					if (0x01 & (filter.attrib_mask[0]>>eAttribFault))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribFault)) && (alm_commom.fixed.ioattr.fault == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribFault)) && (alm_commom.fixed.ioattr.fault == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribFail))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribFail)) && (alm_commom.fixed.ioattr.fail == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribFail)) && (alm_commom.fixed.ioattr.fail == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribScanEnable))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribScanEnable)) && (alm_commom.fixed.iost.scanEnable == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribScanEnable)) && (alm_commom.fixed.iost.scanEnable == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribAlarmEnable))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribAlarmEnable)) && (alm_commom.fixed.iost.almEnable == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribAlarmEnable)) && (alm_commom.fixed.iost.almEnable == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribAlarmNoAck))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribAlarmNoAck)) && (alm_commom.fixed.iost.almNoAck == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribAlarmNoAck)) && (alm_commom.fixed.iost.almNoAck == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribCtrlLock))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribCtrlLock)) && (alm_commom.fixed.iost.ctrlLock == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribCtrlLock)) && (alm_commom.fixed.iost.ctrlLock == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribNoUpdate))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribNoUpdate)) && (alm_commom.fixed.iost.noUpdate == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribNoUpdate)) && (alm_commom.fixed.iost.noUpdate == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribAlmRepeat))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribAlmRepeat)) && (alm_commom.fixed.iost.almRepeat == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribAlmRepeat)) && (alm_commom.fixed.iost.almRepeat == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribTagState))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribTagState)) && (alm_commom.fixed.iost.tagState == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribTagState)) && (alm_commom.fixed.iost.tagState == 1))
							continue;
					}
				}
			}
		}
		
ADD:
		if(filter.start_time>=0)
		{
			time= GetTimefromRec(&filter.pRec[i]);
			if(-1==time)
			{
				Err_log( "Time convert failed");
				continue;
			}
			
			if(time < filter.start_time || time > filter.end_time)
				continue;
		}
		
		if(strcmp(filter.search_str,"")!= 0 )
		{
			GetAlmStrings(&filter.pRec[i], &alm_str, &blink, &color);
			
			if(!strstr(alm_str.longname, filter.search_str))
				continue;
		}
		index->index_pt[index->count++].pt= i;
		if (index->count >= index->buff_size) 
			break;
	}
	
	return 0;
}

int	alm_rec_filter_k(FILTER_T filter, INDEX *index)
{
    USHORT		i, count_rec, start_rec,almNoAck;
	time_t		time;
	ALM_STR		alm_str;
	BOOL		blink;
	COLOR_PAIR	color;
	char		err_str[128];
	POINTER  pt ;
	POINT_NAME pt_name;
	char            logName[64];
	USHORT		io_group;
	BOOL		keySignal;	
	DMS_COMMON	alm_commom;

	index->count= 0;
	/*if(filter.hist_flag)
	return -1;
	*/
	if( filter.phead->buf_size == 0) 
	{  printf(" OIX: Alarm buffer size is Zeor  !!!! \n ");
	return 0 ;
	}
	count_rec= filter.phead->count;
	if(count_rec > filter.phead->buf_size)
	{
		sprintf(err_str, "Event table count overrange: %d", count_rec);
		Err_log(err_str);
		return -1;
	}
	
	start_rec= (filter.phead->header_ptr+ filter.phead->buf_size-count_rec)%filter.phead->buf_size;
	
	for(i=start_rec; count_rec>0; count_rec--,i++,i=(i>=filter.phead->buf_size)? 0:i )
	{       
		if(filter.cmd_type == 1)  /* <=  cmd type */
		{
			if(filter.pRec[i].ana_rec.type_id <= 23 ) /* command type */
				continue ;
		}
        if (filter.isNoAck  )
		{
			if(-1!= GetPointFromRec(&filter.pRec[i], &pt)  )
				if (-1 != IsPtAlmNotAckedById(  &pt, &almNoAck )) 			 
					if(!almNoAck)  //acknoleged!
						continue;
		}

		if (filter.cmd_type == 2) /* short or long message */
		{
			if(!IsMessageRec(&filter.pRec[i]))
				continue ;   
		}

		if(GetPointFromRec(&filter.pRec[i], &pt)!=-1)
		{
			if (IsPtKeySignalById(&pt,&keySignal)!=-1)
				if (keySignal==FALSE)
					continue;
		}

		if(IsMessageRec(&filter.pRec[i]))
		{
			continue;
			if (strcmp(filter.LogicName,"") != 0) /* LogicName!! */
				continue;
			if(filter.dev_mask[0]!=(UINT)-1)
				continue;
			if(filter.datatype_mask!=(UINT)-1)
				continue;
			goto  ADD ;   
		}
		
		if (strcmp(filter.LogicName,"") != 0) /* LogicName!! */
		{
			if(IsMessageRec(&filter.pRec[i]))
				continue ;   
			if(-1!= GetPointFromRec(&filter.pRec[i], &pt)  )
			{
				if (-1 != GetPtNameById(  &pt, logName, &pt_name )) 
				{
					if(!strstr(logName, filter.LogicName))
						continue;
				}
			}
		}


		if(filter.stn_id!=0)
		{
			if(filter.stn_id!= filter.pRec[i].ana_rec.stn_id)
				continue;
		}
		if(isStnInMask(filter.pRec[i].ana_rec.stn_id)==0)  //ADDED BY CHI 2010-01-11
			continue ;  
		
		if(filter.dev_mask[0]!=(UINT)-1)
		{
			if(!(0x01 & (filter.dev_mask[0]>>(filter.pRec[i].ana_rec.dev_id-1))))
				continue;
		}
		
		if(GetPointFromRec(&filter.pRec[i], &pt)!=-1)	//added by dm
		{
			if (-1!=GetPtIoGroupById(&pt,&io_group))	
			{
				if (filter.group_mask[filter.pRec[i].ana_rec.dev_id-1]!=(UINT)-1)
				{
					if(!(0x01 & (filter.group_mask[filter.pRec[i].ana_rec.dev_id-1]>>(io_group-1))))
						continue;
				}
			}
		}
		
		
		if(filter.datatype_mask!=(UINT)-1)
		{
			if(!(0x01 & (filter.datatype_mask>>(filter.pRec[i].ana_rec.data_type-1))))
				continue;
		}
		
		if(GetPointFromRec(&filter.pRec[i], &pt)!=-1)
		{
			if(ReadEntryById(&pt, &alm_commom)!=-1)
			{
				if (filter.attrib_mask[0]!=(UINT)-1 && filter.attrib_mask[1]!=(UINT)-1)
				{
					if (0x01 & (filter.attrib_mask[0]>>eAttribFault))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribFault)) && (alm_commom.fixed.ioattr.fault == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribFault)) && (alm_commom.fixed.ioattr.fault == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribFail))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribFail)) && (alm_commom.fixed.ioattr.fail == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribFail)) && (alm_commom.fixed.ioattr.fail == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribScanEnable))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribScanEnable)) && (alm_commom.fixed.iost.scanEnable == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribScanEnable)) && (alm_commom.fixed.iost.scanEnable == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribAlarmEnable))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribAlarmEnable)) && (alm_commom.fixed.iost.almEnable == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribAlarmEnable)) && (alm_commom.fixed.iost.almEnable == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribAlarmNoAck))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribAlarmNoAck)) && (alm_commom.fixed.iost.almNoAck == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribAlarmNoAck)) && (alm_commom.fixed.iost.almNoAck == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribCtrlLock))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribCtrlLock)) && (alm_commom.fixed.iost.ctrlLock == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribCtrlLock)) && (alm_commom.fixed.iost.ctrlLock == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribNoUpdate))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribNoUpdate)) && (alm_commom.fixed.iost.noUpdate == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribNoUpdate)) && (alm_commom.fixed.iost.noUpdate == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribAlmRepeat))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribAlmRepeat)) && (alm_commom.fixed.iost.almRepeat == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribAlmRepeat)) && (alm_commom.fixed.iost.almRepeat == 1))
							continue;
					}
					if (0x01 & (filter.attrib_mask[0]>>eAttribTagState))
					{
						if((0x01 & (filter.attrib_mask[1]>>eAttribTagState)) && (alm_commom.fixed.iost.tagState == 0))
							continue;
						if(!(0x01 & (filter.attrib_mask[1]>>eAttribTagState)) && (alm_commom.fixed.iost.tagState == 1))
							continue;
					}
				}
			}
		}


ADD:
		if(filter.start_time>=0)
		{
			time= GetTimefromRec(&filter.pRec[i]);
			if(-1==time)
			{
				Err_log( "Time convert failed");
				continue;
			}
			
			if(time < filter.start_time || time > filter.end_time)
				continue;
		}
		
		if(strcmp(filter.search_str,"")!= 0 )
		{
			GetAlmStrings(&filter.pRec[i], &alm_str, &blink, &color);
			
			if(!strstr(alm_str.longname, filter.search_str))
				continue;
		}
		index->index_pt[index->count++].pt= i;
		if (index->count >= index->buff_size) 
			break;
	}
	
	return 0;
}





