#include	<time.h>
#include	<stdio.h>


#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dms/v4.0a/inc/dms_data_def.h"//cxs
#include	"../inc/dps_rec_def.h"
#include	"../inc/dps_type_def.h"
#include	"../inc/ipc_common.h"
#include	"../inc/fault_table_def.h"


#ifdef	WINDOWS_OS
#define		sleep(sec)		Sleep(sec*1000)
#endif



int		InitIPC(void);
void	InitFaultTable(void);
int	    GetAlmCbaState_FAULT(POINTER *point,int type_id,int ind_rt_state,int qlt_alm_flag);//chenxs add 20110801
void	UpdateFaultTable(REC_ENTRY_UNION *rec);
void	AddPointtoFaultTable(DMS_COMMON	*common_all, int alm_type);
void	ScanFaultTable(void);


SHM_RECORD			*shm_rec_addr;
FAULT_RECORD_TABLE *shm_fault_addr;




/*#define	DEBUG*/

#pragma	comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma	comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma	comment(lib, "../../../lan/v4.0a/lib/lan.lib")

int	main(int argc, char *argv[])
{
	USHORT		rec_ptr;

	CreateLockFile(".faultTable.lock");
	if(-1==InitSysLog(H9000_MODULE_FAULTTABLE, "FaultTable"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}

	if(-1==InitIPC())
	{
		Err_log("Fail to initialize IPCs!\n");
		exit(-1);
	}

	if(-1==MountDB(NULL))
	{
		Err_log("Fail to MountDB!\n");
		exit(-1);
	}

	InitFaultTable();

	rec_ptr= shm_rec_addr->head.header_ptr;
	for( ;; )
	{
		ScanFaultTable();
		while(rec_ptr != shm_rec_addr->head.header_ptr)
		{
#ifdef	DEBUG
			printf("\nDPS: rec_ptr= %d header_ptr= %d", rec_ptr, shm_rec_addr->head.header_ptr);
#endif
			if(shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_ANA_ALM
				||shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_IND_ALM
				||shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_IMP_ALM
				||shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_SM_ALM)
				UpdateFaultTable(&shm_rec_addr->rec[rec_ptr]);

			rec_ptr++;
			if(rec_ptr>=shm_rec_addr->head.buf_size)
				rec_ptr= 0;
		}
		sleep(1);
	}

}



int InitIPC()
{
	/*      打开未复归事故记录区共享内存      */
	shm_fault_addr= (FAULT_RECORD_TABLE*) LinktoShareMem( SHM_FAULT_TABLE_KEY_CHAR );
	if( shm_fault_addr == (FAULT_RECORD_TABLE*)-1 )
	{
		Err_log("Fail to link fault record shm");
		return -1;
	}
	shm_fault_addr->head.buf_size= MAX_FAULT_RECORD_NUM;
	shm_fault_addr->head.header_ptr= 0;
	shm_fault_addr->head.end_ptr= 0;
	shm_fault_addr->head.count= 0;

	
	/*      打开记录区共享内存      */
	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		Err_log("Fail to link record shm");
		return -1;
	}
	
	return 0;
}



void	InitFaultTable(void)
{
	GULONG		stn_num, grp_num, rec_num, entry_num;
	GULONG		i,j,k,m;
	POINTER		point;
	DMS_COMMON	common_all;
	USHORT		LinkSta;
	
	GetStationNum( &stn_num );
	for(i=1; i<=stn_num; i++)
	{
		UCHAR stnId;
		GetStnIdByIndex(i, &stnId);

		if(-1==GetGroupNumById(stnId, &grp_num))
			continue;
		for(j=1; j<=grp_num; j++)
		{
			point.stn_id= stnId;
			point.dev_id= (UCHAR)j;
			point.data_type= SYSMSG_TYPE;
			if(GetEntryIdByName("LINK_STA", &point)==-1)
				continue;
			if(GetIndPtRtStateById(&point, &LinkSta)==-1)
				continue;
			if(LinkSta!=1)
				continue;

			if(-1==GetRecordNumById(stnId, (UCHAR)j, &rec_num))
				continue;
			for(k=1; k<MAX_DATATYPE_NUM; k++)
			{
				if(--rec_num<=0)
					break;
				if(k!=ANA_TYPE && k!=OUTA_TYPE && k!=PARAM_TYPE && k!=POL_TYPE && k!=SOE_TYPE 
					&& k!=SYSMSG_TYPE && k!=OUTS_TYPE && k!=IMP_TYPE)
					continue;

				if(-1==GetEntryNumById(stnId, (UCHAR)j, (UCHAR)k, &entry_num))
					continue;

				for(m=0; m<entry_num; m++)
				{
					point.stn_id= stnId;
					point.dev_id= (UCHAR)j;
					point.data_type= (UCHAR)k;
					point.pt_id= (USHORT)m;
					ReadEntryById(&point, &common_all);

					if(!common_all.fixed.iost.almEnable)
						continue;

					if(!(common_all.fixed.ioattr.fault || common_all.fixed.ioattr.fail))
						continue;

					if(k==ANA_TYPE || k==OUTA_TYPE || k==PARAM_TYPE )
					{
						if(common_all.var.anlg.value.status.lmt_state)
							AddPointtoFaultTable(&common_all, ANA_ALM_TYPE_LMT);
						if(common_all.var.anlg.value.status.trend_state)
							AddPointtoFaultTable(&common_all, ANA_ALM_TYPE_TR);
						if(common_all.var.anlg.value.status.chan_fail)
							AddPointtoFaultTable(&common_all, ANA_ALM_TYPE_CHANFAIL);
					}
					else if(k==IMP_TYPE)
					{
						if(common_all.var.imp.entry.status.chan_fail)
							AddPointtoFaultTable(&common_all, IMP_ALM_TYPE_CHANFAIL);
					}
					else if(point.data_type==POL_TYPE || point.data_type==SOE_TYPE 
						|| point.data_type==SYSMSG_TYPE || point.data_type==OUTS_TYPE)
					{
						if(!(common_all.fixed.ioattr.onAlm || common_all.fixed.ioattr.offAlm))
							continue;
						if(common_all.var.ind.status.chan_fail)
							AddPointtoFaultTable(&common_all, IND_ALM_TYPE_CHANFAIL);
						if(common_all.var.ind.status.rt_state!=common_all.fixed.ioattr.normalState)
							AddPointtoFaultTable(&common_all, IND_ALM_TYPE_CHANGE);
					}
				}
			}	
		}  
	}  
}


time_t	GetTimeofRec(REC_ENTRY_UNION *rec, GULONG *ms)
{
	struct tm	time;

	*ms= 0;
	if(rec->ana_rec.type_id==DPS_SM_ALM)
		return rec->sms_rec.time;
	
	if(rec->ana_rec.type_id==DPS_ANA_ALM||rec->ana_rec.type_id==DPS_IMP_ALM)
	{
		time.tm_year= rec->ana_rec.year-1900;
		time.tm_mon= rec->ana_rec.month-1;
		time.tm_mday= rec->ana_rec.day;
		time.tm_hour= rec->ana_rec.hour;
		time.tm_min= rec->ana_rec.minute;
		time.tm_sec= rec->ana_rec.second;
		*ms= 0;
		return mktime(&time);
	}
	else if(rec->ana_rec.type_id==DPS_IND_ALM)
	{
		time.tm_year= rec->ind_rec.year-1900;
		time.tm_mon= rec->ind_rec.month-1;
		time.tm_mday= rec->ind_rec.day;
		time.tm_hour= rec->ind_rec.hour;
		time.tm_min= rec->ind_rec.minute;
		time.tm_sec= rec->ind_rec.second;
		*ms= rec->ind_rec.ms;
		return mktime(&time);
	}
	return 0;
}


void InsertFaultTable(REC_ENTRY_UNION *new_rec, BOOL time_flag)
{
	time_t	tt_new, tt_old;
	int		i, pos;
	GULONG		ms_new, ms_old;

	if(shm_fault_addr->head.header_ptr >= shm_fault_addr->head.buf_size)
	{
		Err_log("Too much fault record entry, delete old fault record entry");
		for(i=0; i<(shm_fault_addr->head.header_ptr-1); i++)
		{
			memcpy(&shm_fault_addr->rec[i], &shm_fault_addr->rec[i+1], sizeof(REC_ENTRY_UNION));
			shm_fault_addr->rec[i].ana_rec.rec_sn= i+1;
		}
		shm_fault_addr->head.header_ptr--;
		shm_fault_addr->head.count= shm_fault_addr->head.header_ptr;
	}

	if(time_flag)
	{
		/*按时间插入*/
		tt_new= GetTimeofRec(new_rec, &ms_new);
		
		for(pos=(shm_fault_addr->head.header_ptr-1); pos>=0; pos--)
		{
			tt_old= GetTimeofRec(&shm_fault_addr->rec[pos], &ms_old);
			if(tt_new>tt_old)
				break;
			if(tt_new==tt_old && ms_new>=ms_old)
				break;
		}
		
		for(i=shm_fault_addr->head.header_ptr; i>(pos+1); i--)
		{
			memcpy(&shm_fault_addr->rec[i], &shm_fault_addr->rec[i-1], sizeof(REC_ENTRY_UNION));
			shm_fault_addr->rec[i].ana_rec.rec_sn= i+1;
		}
		memcpy(&shm_fault_addr->rec[pos+1], new_rec, sizeof(REC_ENTRY_UNION));	
		shm_fault_addr->rec[pos+1].ana_rec.rec_sn= pos+2;
	}
	else
	{
		/*不按时间插入*/
		memcpy(&shm_fault_addr->rec[shm_fault_addr->head.header_ptr], new_rec, sizeof(REC_ENTRY_UNION));	
		shm_fault_addr->rec[shm_fault_addr->head.header_ptr].ana_rec.rec_sn= shm_fault_addr->head.header_ptr+1;
	}
	shm_fault_addr->head.header_ptr++;
	shm_fault_addr->head.count= shm_fault_addr->head.header_ptr;
}



void	AddPointtoFaultTable(DMS_COMMON	*common_all, int alm_type)
{
	REC_ENTRY_UNION	rec;

	if(common_all->point.data_type==ANA_TYPE 
		|| common_all->point.data_type==OUTA_TYPE 
		|| common_all->point.data_type==PARAM_TYPE )
	{
		rec.ana_rec.type_id= DPS_ANA_ALM;
		rec.ana_rec.stn_id= common_all->point.stn_id;
		rec.ana_rec.dev_id= common_all->point.dev_id;
		rec.ana_rec.data_type= common_all->point.data_type;
		rec.ana_rec.pt_id= common_all->point.pt_id;
		rec.ana_rec.status.ana_status= common_all->var.anlg.value.status;
		rec.ana_rec.status.ana_status.alm_type= alm_type;
		rec.ana_rec.data= common_all->var.anlg.value.data;
		rec.ana_rec.host_id= 0;
		rec.ana_rec.second= common_all->var.anlg.update_time.second;
		rec.ana_rec.minute= common_all->var.anlg.update_time.minute;
		rec.ana_rec.hour= common_all->var.anlg.update_time.hour;
		rec.ana_rec.day= common_all->var.anlg.update_time.day;
		rec.ana_rec.month= common_all->var.anlg.update_time.month;
		rec.ana_rec.year= common_all->var.anlg.update_time.year;
		rec.ana_rec.iost= common_all->fixed.iost;
	}
	else if(common_all->point.data_type==IMP_TYPE)
	{
		rec.ana_rec.type_id= DPS_IMP_ALM;
		rec.ana_rec.stn_id= common_all->point.stn_id;
		rec.ana_rec.dev_id= common_all->point.dev_id;
		rec.ana_rec.data_type= common_all->point.data_type;
		rec.ana_rec.pt_id= common_all->point.pt_id;
		rec.ana_rec.status.imp_status= common_all->var.imp.entry.status;
		rec.ana_rec.status.imp_status.alm_type= alm_type;
		rec.ana_rec.data= common_all->var.imp.entry.rt_value;
		rec.ana_rec.host_id= 0;
		rec.ana_rec.second= common_all->var.imp.entry.update_time.second;
		rec.ana_rec.minute= common_all->var.imp.entry.update_time.minute;
		rec.ana_rec.hour= common_all->var.imp.entry.update_time.hour;
		rec.ana_rec.day= common_all->var.imp.entry.update_time.day;
		rec.ana_rec.month= common_all->var.imp.entry.update_time.month;
		rec.ana_rec.year= common_all->var.imp.entry.update_time.year;
		rec.ana_rec.iost= common_all->fixed.iost;
	}
	else
	{
		rec.ind_rec.type_id= DPS_IND_ALM;
		rec.ind_rec.stn_id= common_all->point.stn_id;
		rec.ind_rec.dev_id= common_all->point.dev_id;
		rec.ind_rec.data_type= common_all->point.data_type;
		rec.ind_rec.pt_id= common_all->point.pt_id;
		rec.ind_rec.ind_status= common_all->var.ind.status;
		rec.ind_rec.ind_status.alm_type= alm_type;
		rec.ind_rec.ms= common_all->var.ind.change_time.ms;
		rec.ind_rec.iost= common_all->fixed.iost;
		rec.ind_rec.host_id= 0;
		rec.ind_rec.second= common_all->var.ind.change_time.second;
		rec.ind_rec.minute= common_all->var.ind.change_time.minute;
		rec.ind_rec.hour= common_all->var.ind.change_time.hour;
		rec.ind_rec.day= common_all->var.ind.change_time.day;
		rec.ind_rec.month= common_all->var.ind.change_time.month;
		rec.ind_rec.year= common_all->var.ind.change_time.year;
		rec.ind_rec.iost= common_all->fixed.iost;
	}

	InsertFaultTable(&rec, TRUE);
}


int GetPointIdFromRec(REC_ENTRY_UNION *rec, POINTER *pt, int *alm_type)
{   
    switch(rec->ana_rec.type_id)
	{
	case	DPS_ANA_ALM:
		pt->stn_id =  rec->ana_rec.stn_id ;
		pt->dev_id =  rec->ana_rec.dev_id ;
		pt->data_type =  rec->ana_rec.data_type ;
		pt->pt_id =  rec->ana_rec.pt_id ;
		*alm_type= rec->ana_rec.status.ana_status.alm_type;
		return 0;

	case	DPS_IMP_ALM:
		pt->stn_id =  rec->ana_rec.stn_id ;
		pt->dev_id =  rec->ana_rec.dev_id ;
		pt->data_type =  rec->ana_rec.data_type ;
		pt->pt_id =  rec->ana_rec.pt_id ;
		*alm_type= rec->ana_rec.status.imp_status.alm_type;
		return 0;

	case	DPS_IND_ALM:
		pt->stn_id =  rec->ind_rec.stn_id ;
		pt->dev_id =  rec->ind_rec.dev_id ;
		pt->data_type =  rec->ind_rec.data_type ;
		pt->pt_id =  rec->ind_rec.pt_id ;
		*alm_type= rec->ind_rec.ind_status.alm_type;
		return 0;

	default:
		return -1;
	}
}


void DeleteRecFromTable(POINTER *point, int alm_type)
{
	int			i, type;
	POINTER		pt;

	for(i=0; i<shm_fault_addr->head.header_ptr; i++)
	{
		if(-1==GetPointIdFromRec(&shm_fault_addr->rec[i], &pt, &type))
			continue;		
		if(pt.stn_id!=point->stn_id)
			continue;
		if(pt.dev_id!=point->dev_id)
			continue;
		if(pt.data_type!=point->data_type)
			continue;
		if(pt.pt_id!=point->pt_id)
			continue;
		if(alm_type!=type)
			continue;

		for(;i<(shm_fault_addr->head.header_ptr-1); i++)
		{
			memcpy(&shm_fault_addr->rec[i], &shm_fault_addr->rec[i+1], sizeof(REC_ENTRY_UNION));
			shm_fault_addr->rec[i].ana_rec.rec_sn= i+1;
		}			
		shm_fault_addr->head.header_ptr--;
		shm_fault_addr->head.count= shm_fault_addr->head.header_ptr;
	}
}


BOOL	FindSameRepeatRec(POINTER *point, int alm_type)
{
	int			i, type;
	POINTER		pt;

	for(i=0; i<shm_fault_addr->head.header_ptr; i++)
	{
		if(-1==GetPointIdFromRec(&shm_fault_addr->rec[i], &pt, &type))
			continue;		
		if(pt.stn_id!=point->stn_id)
			continue;
		if(pt.dev_id!=point->dev_id)
			continue;
		if(pt.data_type!=point->data_type)
			continue;
		if(pt.pt_id!=point->pt_id)
			continue;
		if(alm_type!=type)
			continue;
		switch(shm_fault_addr->rec[i].ana_rec.type_id)
		{
		case	DPS_ANA_ALM:
		case	DPS_IMP_ALM:
			if(shm_fault_addr->rec[i].ana_rec.iost.almRepeat)
				return TRUE;

		case	DPS_IND_ALM:
			if(shm_fault_addr->rec[i].ind_rec.iost.almRepeat)
				return TRUE;
		}
	}
	return FALSE;
}


void ScanFaultTable(void)
{
	POINTER			point;
	int				i, ptr;
	DMS_COMMON		common_all;
	REC_ENTRY_UNION *rec;
	int				alm_type;
	int				delete_flag;
	time_t			curr_time= time(NULL);

	for(ptr=0; ptr<shm_fault_addr->head.header_ptr; ptr++)
	{
		delete_flag= 0;
		rec= &shm_fault_addr->rec[ptr];

		if(rec->ana_rec.type_id==DPS_SM_ALM)
		{
			if((curr_time-rec->sms_rec.time)>=2*60)
				delete_flag= 1;
		}
		else
		{
			if(-1==GetPointIdFromRec(rec, &point, &alm_type))
				continue;
			
			ReadEntryById(&point, &common_all);
			if(common_all.fixed.iost.almNoAck)
				continue;
			
			if(!common_all.fixed.iost.almEnable)
				delete_flag= 1;
			else
			{
				if(point.data_type==ANA_TYPE || point.data_type==PARAM_TYPE || point.data_type==OUTA_TYPE)
				{
					if((alm_type==ANA_ALM_TYPE_LMT && !common_all.var.anlg.value.status.lmt_state)
						||(alm_type==ANA_ALM_TYPE_TR && !common_all.var.anlg.value.status.trend_state)
						||(alm_type==ANA_ALM_TYPE_CHANFAIL && !common_all.var.anlg.value.status.chan_fail))
						delete_flag= 1;
				}
				else if(point.data_type==IMP_TYPE)
				{
					if(alm_type==IMP_ALM_TYPE_CHANFAIL && !common_all.var.imp.entry.status.chan_fail)
						delete_flag= 1;
				}
				else if(point.data_type==POL_TYPE || point.data_type==SOE_TYPE 
					|| point.data_type==SYSMSG_TYPE || point.data_type==OUTS_TYPE)
				{
					if((alm_type==IND_ALM_TYPE_CHANFAIL && !common_all.var.ind.status.chan_fail)
						||(alm_type==IND_ALM_TYPE_CHANGE && (common_all.var.ind.status.rt_state==common_all.fixed.ioattr.normalState))
						||(!(common_all.fixed.ioattr.onAlm || common_all.fixed.ioattr.offAlm)))
						delete_flag= 1;
				}
			}
		}

		if(delete_flag)
		{
			for(i=ptr;i<(shm_fault_addr->head.header_ptr-1); i++)
			{
				memcpy(&shm_fault_addr->rec[i], &shm_fault_addr->rec[i+1], sizeof(REC_ENTRY_UNION));
				shm_fault_addr->rec[i].ana_rec.rec_sn= i+1;
			}
			shm_fault_addr->head.header_ptr--;
			shm_fault_addr->head.count= shm_fault_addr->head.header_ptr;
		}
	}/*for()*/
}

  
int	GetAlmCbaState_FAULT(POINTER *point,int type_id,int ind_rt_state,int qlt_alm_flag)//cxs add
{
	char	ptNameStr[POINT_NAME_SIZE+12];
	char	ptNameStr1[POINT_NAME_SIZE+16];
	BOOL    can_alarm;
	
	USHORT  iogrp=0;
	POINT_NAME	ptName;
	
	if(-1==GetPtNameById(point, ptNameStr, NULL))
		return -1;
	if(-1==StrtoPointName((char *)(&ptNameStr), &ptName))
		return -1;
	
	can_alarm=TRUE;
	
 
	
	sprintf(ptNameStr1,"%s.%s..%s",ptName.stnName,ptName.grpName,FALM_DCBA_SUFFIX_DEFINE);	
	
	can_alarm =InterLock_Judge( ptNameStr1 );
	if(can_alarm!=-1)
	{
		if(can_alarm==FALSE)
			return can_alarm;
	}
	
	GetPtIoGroupById(point, &iogrp);
	
	sprintf(ptNameStr1,"%s.%s_%d..%s",ptName.stnName,ptName.grpName,iogrp,FALM_GCBA_SUFFIX_DEFINE);
	
	can_alarm =InterLock_Judge( ptNameStr1 );
	if(can_alarm!=-1)
	{
		if(can_alarm==FALSE)
			return can_alarm;
	}
	
	if(type_id==DPS_IND_ALM)
	{
		sprintf(ptNameStr1,"%s%s_%d",ptNameStr,FALM_CBA_SUFFIX_DEFINE,ind_rt_state);
		//sprintf(ptNameStr1,"%s.._%d",ptNameStr,ind_rt_state);
		can_alarm =InterLock_Judge( ptNameStr1 );
		if(can_alarm!=-1)
		{
			if(can_alarm==FALSE)
				return can_alarm;
		}
		
	}

	sprintf(ptNameStr1,"%s%s",ptNameStr,FALM_CBA_SUFFIX_DEFINE);
	//sprintf(ptNameStr1,"%s%s",ptNameStr,FALM_CBA_SUFFIX_DEFINE);
	
	can_alarm =InterLock_Judge( ptNameStr1 );
	if(can_alarm!=-1)
	{
		if(can_alarm==FALSE)
			return can_alarm;
	}
	

	
	return can_alarm;
}
 //cxs add end 20110801 for faulttable

void UpdateFaultTable(REC_ENTRY_UNION *rec)
{
	POINTER		point;
	GULONG		normalState;
	USHORT		almNoAck;
	USHORT		state;
	int			alm_type;
	BOOL		faultPt, failPt;
	int ind_rt_state;//cxs

	DMS_COMMON	common_all;//cxs

	if(rec->ana_rec.type_id==DPS_SM_ALM)
	{
		InsertFaultTable(rec, FALSE);
		return;
	}

	if(-1==GetPointIdFromRec(rec, &point, &alm_type))
		return;

	if(-1==IsPtFaultById(&point, &faultPt))
		return;

	if(!faultPt)
	{
		IsPtFailById(&point, &failPt);
		if(!failPt)
			return;
	}
	
	
	//chenxs add  start 20110801 for interlock
	if(-1!=ReadEntryById(&point , &common_all))
	{
		
		if(common_all.fixed.ioattr.almCba)
		{
			if(rec->ana_rec.type_id==DPS_IND_ALM)
				ind_rt_state=rec->ind_rec.ind_status.rt_state;
			else
					ind_rt_state=-1;
			if( !GetAlmCbaState_FAULT(&point,rec->ana_rec.type_id,ind_rt_state,0) )
				return;
		}
		
	}
	//chenxs add  end 20110801 for interlock	
	
	if(rec->ana_rec.type_id==DPS_ANA_ALM)
	{
		if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_LMT)
		{
			state= rec->ana_rec.status.ana_status.lmt_state;
			if(state==ANA_ALM_HI_ALARM || state==ANA_ALM_HI_WARN
				|| state==ANA_ALM_LO_WARN || state==ANA_ALM_LO_ALARM)
			{
/*				if(rec->ana_rec.iost.almRepeat)
				{
					if(!FindSameRepeatRec(&point, alm_type))
						InsertFaultTable(rec, FALSE);
				}
				else
*/

				DeleteRecFromTable(&point, alm_type);
				InsertFaultTable(rec, FALSE);
			}
			else
			{
				IsPtAlmNotAckedById(&point, &almNoAck);
				if(!almNoAck)
					DeleteRecFromTable(&point, alm_type);
			}
		}
		else if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_TR)
		{
			if(rec->ana_rec.status.ana_status.trend_state==ANA_ALM_TR_WARN)
			{
/*				if(rec->ana_rec.iost.almRepeat)
				{
					if(!FindSameRepeatRec(&point, alm_type))
						InsertFaultTable(rec, FALSE);
				}
				else
*/

				DeleteRecFromTable(&point, alm_type);
				InsertFaultTable(rec, FALSE);
			}
			else
			{
				IsPtAlmNotAckedById(&point, &almNoAck);
				if(!almNoAck)
					DeleteRecFromTable(&point, alm_type);
			}
		}
		else if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_CHANFAIL)
		{
			if(rec->ana_rec.status.ana_status.chan_fail)
			{
/*				if(rec->ana_rec.iost.almRepeat)
				{
					if(!FindSameRepeatRec(&point, alm_type))
						InsertFaultTable(rec, FALSE);
				}
				else
*/

				DeleteRecFromTable(&point, alm_type);
				InsertFaultTable(rec, FALSE);
			}
			else
			{
				IsPtAlmNotAckedById(&point, &almNoAck);
				if(!almNoAck)
					DeleteRecFromTable(&point, alm_type);
			}
		}
	}
	else if(rec->ana_rec.type_id==DPS_IMP_ALM)
	{
		if(rec->ana_rec.status.imp_status.alm_type==IMP_ALM_TYPE_CHANFAIL)
		{
			if(rec->ana_rec.status.imp_status.chan_fail)
			{
/*				if(rec->ana_rec.iost.almRepeat)
				{
					if(!FindSameRepeatRec(&point, alm_type))
						InsertFaultTable(rec, FALSE);
				}
				else
*/

				DeleteRecFromTable(&point, alm_type);
				InsertFaultTable(rec, FALSE);
			}
			else
			{
				IsPtAlmNotAckedById(&point, &almNoAck);
				if(!almNoAck)
					DeleteRecFromTable(&point, alm_type);
			}
		}
	}
	else if(rec->ana_rec.type_id==DPS_IND_ALM)
	{
		if(rec->ind_rec.ind_status.alm_type==IND_ALM_TYPE_CHANGE)
		{
			GetPtNormalStateById(&point, &normalState);
			if(rec->ind_rec.ind_status.rt_state==normalState)
			{
				IsPtAlmNotAckedById(&point, &almNoAck);
				if(!almNoAck)
					DeleteRecFromTable(&point, alm_type);
			}
			else
			{
/*				if(rec->ind_rec.iost.almRepeat)
				{
					if(!FindSameRepeatRec(&point, alm_type))
						InsertFaultTable(rec, FALSE);
				}
				else
*/

				DeleteRecFromTable(&point, alm_type);
				InsertFaultTable(rec, FALSE);
			}
		}
		else if(rec->ind_rec.ind_status.alm_type==IND_ALM_TYPE_CHANFAIL)
		{
			if(rec->ind_rec.ind_status.chan_fail)
			{
/*				if(rec->ind_rec.iost.almRepeat)
				{
					if(!FindSameRepeatRec(&point, alm_type))
						InsertFaultTable(rec, FALSE);
				}
				else
*/

				DeleteRecFromTable(&point, alm_type);
				InsertFaultTable(rec, FALSE);
			}
			else
			{
				IsPtAlmNotAckedById(&point, &almNoAck);
				if(!almNoAck)
					DeleteRecFromTable(&point, alm_type);
			}
		}
	}
}





















