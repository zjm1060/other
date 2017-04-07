#include <string.h>
 
 
#include    "../inc/os_type_def.h"
 
#include    "../inc/dms_std.h"
#include    "../inc/dms_data_def.h"
#include    "../inc/dms_util_def.h"
#include    "../inc/dms_com.h"
#include    "../inc/dms_functions.h"
#include    "../../../dps/v4.0a/inc/dps_type_def.h"



extern DBS_GBL_COMMON  *db_ptr;
int	GetCommonPtrById( POINTER *point, DMS_COMMON_PTR *common_ptr);
int	GetCommonPtrByNameStr( char *ptNameStr, DMS_COMMON_PTR *common_ptr);
void	CheckDbValid(void);



int	SetPtValueIostById( POINTER *point, WR_VAR_UNION new_value, 
						 IOST iost, DATE_TIME time, enum RW_FLAG rw_flag )
{
	DMS_COMMON_PTR	common_ptr;
	WR_VAR_UNION	value;
	DATE_TIME		change_time;
	FLOAT			fValue;
	USHORT			stc;
	IOST			iost_new;
	enum RW_IMP_OPT	imp_opt;
	char			err_str[128];
	POINTER			pair_pt;
	DMS_COMMON_PTR	pair_common_ptr;
	IOATTR			ioattr;
	IND_DATA		ind_status, pair_status;
	USHORT			state;


	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	if( rw_flag & RW_SCAN_ENABLE )
	{
		iost_new= common_ptr.fixed_ptr->iost;
		Swap2Byte(&iost_new);
		iost_new.scanEnable= iost.scanEnable;
		Swap2Byte(&iost_new);
		common_ptr.fixed_ptr->iost= iost_new;	
	}
	if( rw_flag & RW_ALM_ENABLE )
	{
		iost_new= common_ptr.fixed_ptr->iost;
		Swap2Byte(&iost_new);
		iost_new.almEnable= iost.almEnable;	
		Swap2Byte(&iost_new);
		common_ptr.fixed_ptr->iost= iost_new;	
	}
	if( rw_flag & RW_ALM_NOACK )
	{
		iost_new= common_ptr.fixed_ptr->iost;
		Swap2Byte(&iost_new);
		iost_new.almNoAck= iost.almNoAck;	
		Swap2Byte(&iost_new);
		common_ptr.fixed_ptr->iost= iost_new;	
	}
	if( rw_flag & RW_CTRL_LOCK )
	{
		iost_new= common_ptr.fixed_ptr->iost;
		Swap2Byte(&iost_new);
		iost_new.ctrlLock= iost.ctrlLock;	
		Swap2Byte(&iost_new);
		common_ptr.fixed_ptr->iost= iost_new;	
	}
	if( rw_flag & RW_UPDATED )
	{
		iost_new= common_ptr.fixed_ptr->iost;
		Swap2Byte(&iost_new);
		iost_new.noUpdate= iost.noUpdate;	
		Swap2Byte(&iost_new);
		common_ptr.fixed_ptr->iost= iost_new;	
	}
	if( rw_flag & RW_ALM_REPEAT )
	{
		iost_new= common_ptr.fixed_ptr->iost;
		Swap2Byte(&iost_new);
		iost_new.almRepeat= iost.almRepeat;	
		Swap2Byte(&iost_new);
		common_ptr.fixed_ptr->iost= iost_new;	
	}
	if( rw_flag & RW_TAG_STATE )
	{
		iost_new= common_ptr.fixed_ptr->iost;
		Swap2Byte(&iost_new);
		iost_new.tagState= iost.tagState;
		Swap2Byte(&iost_new);
		common_ptr.fixed_ptr->iost= iost_new;	
	}
	if( rw_flag & RW_VIEWPOINT )
	{
		iost_new= common_ptr.fixed_ptr->iost;
		Swap2Byte(&iost_new);
		iost_new.viewPoint= iost.viewPoint;
		Swap2Byte(&iost_new);
		common_ptr.fixed_ptr->iost= iost_new;	
	}
	
	if( rw_flag & RW_ADD_NORMAL_OPEN )
	{
		if( (point->data_type==SOE_TYPE)
			||(point->data_type==POL_TYPE)
			||(point->data_type==OUTS_TYPE)
			||(point->data_type==SYSMSG_TYPE))
		{
			stc= common_ptr.var_ptr->ind.stc.norm_open;
			Swap2Byte(&stc);
			stc++;
			Swap2Byte(&stc);
			common_ptr.var_ptr->ind.stc.norm_open= stc;
		}
		else
		{
			sprintf(err_str, "data_type error: %d", point->data_type);
			Err_log(err_str);
			return -1;
		}
	}

	if( rw_flag & RW_ADD_NORMAL_CLOSE )
	{
		if( (point->data_type==SOE_TYPE)
			||(point->data_type==POL_TYPE)
			||(point->data_type==OUTS_TYPE)
			||(point->data_type==SYSMSG_TYPE))
		{
			stc= common_ptr.var_ptr->ind.stc.norm_close;
			Swap2Byte(&stc);
			stc++;
			Swap2Byte(&stc);
			common_ptr.var_ptr->ind.stc.norm_close= stc;
		}
		else
		{
			sprintf(err_str, "data_type error: %d", point->data_type);
			Err_log(err_str);
			return -1;
		}
	}
	
	if( rw_flag & RW_ADD_FAULT_OPEN )
	{
		if( (point->data_type==SOE_TYPE)
			||(point->data_type==POL_TYPE)
			||(point->data_type==OUTS_TYPE)
			||(point->data_type==SYSMSG_TYPE))
		{
			stc= common_ptr.var_ptr->ind.stc.fault_open;
			Swap2Byte(&stc);
			stc++;
			Swap2Byte(&stc);
			common_ptr.var_ptr->ind.stc.fault_open= stc;
		}
		else
		{
			sprintf(err_str, "data_type error: %d", point->data_type);
			Err_log(err_str);
			return -1;
		}
	}
	
	if( rw_flag & RW_ADD_FAULT_CLOSE )
	{
		if( (point->data_type==SOE_TYPE)
			||(point->data_type==POL_TYPE)
			||(point->data_type==OUTS_TYPE)
			||(point->data_type==SYSMSG_TYPE))
		{
			stc= common_ptr.var_ptr->ind.stc.fault_close;
			Swap2Byte(&stc);
			stc++;
			Swap2Byte(&stc);
			common_ptr.var_ptr->ind.stc.fault_close= stc;
		}
		else
		{
			sprintf(err_str, "data_type error: %d", point->data_type);
			Err_log(err_str);
			return -1;
		}
	}
	
/*	if( rw_flag & RW_CHANGE_TIME )
	{
		if( (point->data_type==SOE_TYPE)
			||(point->data_type==POL_TYPE)
			||(point->data_type==OUTS_TYPE)
			||(point->data_type==SYSMSG_TYPE))
		{
			change_time= time;
			SwapDateTimeByte(&change_time);
			common_ptr.var_ptr->ind.change_time= change_time;
		}
		else
		{
			sprintf(err_str, "data_type error: %d", point->data_type);
			Err_log(err_str);
			return -1;
		}
	}
*/
	if( rw_flag & RW_UPDATE_TIME )
	{
		change_time= time;
		SwapDateTimeByte(&change_time);
		switch	( point->data_type )
		{
		case	SOE_TYPE:
		case	POL_TYPE:
		case	OUTS_TYPE:
		case	SYSMSG_TYPE:
			common_ptr.var_ptr->ind.update_time= change_time;
			if( rw_flag & RW_RT_VALUE )
			{
				ind_status= common_ptr.var_ptr->ind.status;
				Swap2Byte(&ind_status);
				if(ind_status.rt_state!=new_value.ind.rt_state)
					common_ptr.var_ptr->ind.change_time= change_time;
			}
			break;

		case	ANA_TYPE:
		case	OUTA_TYPE:
		case	PARAM_TYPE:
			common_ptr.var_ptr->anlg.update_time= change_time;
			break;

		case	IMP_TYPE:
			common_ptr.var_ptr->imp.entry.update_time= change_time;
			break;

		case	CALC_TYPE:
			common_ptr.var_ptr->calc.update_time= change_time;
			break;

		case	CHARA_TYPE:
			common_ptr.var_ptr->chara.update_time= change_time;
			break;

		case	CURVE_TYPE:
			common_ptr.var_ptr->curve.update_time= change_time;
			break;

		case	DEVOBJ_TYPE:
			common_ptr.var_ptr->devobj.update_time= change_time;
			break;

		default:
			break;
		}
	}

	if( rw_flag & RW_RT_VALUE )
	{
		common_ptr.fixed_ptr->update_src= point->update_src;
		switch	( point->data_type )
		{
		case	SOE_TYPE:
		case	POL_TYPE:
		case	OUTS_TYPE:
		case	SYSMSG_TYPE:
			value.ind= common_ptr.var_ptr->ind.status;
			Swap2Byte(&value.ind);
			value.ind.rt_state= new_value.ind.rt_state;
			value.ind.state= new_value.ind.rt_state;
			ioattr= common_ptr.fixed_ptr->ioattr;
			Swap4Byte(&ioattr);
			if(ioattr.doublePt)
			{
				pair_pt= common_ptr.fixed_ptr->pair_pt;
				Swap2Byte(&pair_pt.pt_id);
				if( -1!=GetCommonPtrById(&pair_pt, &pair_common_ptr) )
				{
					pair_status= pair_common_ptr.var_ptr->ind.status;
					Swap2Byte(&pair_status);
					if(ioattr.normalClose)
						state= value.ind.rt_state+(pair_status.rt_state<<1);
					else
						state= (value.ind.rt_state<<1)+pair_status.rt_state;

					if(state==0)
						state= 2;
					else if(state==1)
						state= 0;
					else if(state==2)
						state= 1;
					else if(state==3)
						state= 3;
					
					if(value.ind.rt_state>1 || pair_status.rt_state>1)
						state= 4;

					pair_status.state= state;
					value.ind.state= state;
					Swap2Byte(&pair_status);
					pair_common_ptr.var_ptr->ind.status= pair_status;					
				}
				else
				{
					sprintf(err_str, "Fail to find pair pt %d.%d.%d.%d \
						of double pt %d.%d.%d.%d in database", 
						pair_pt.stn_id, pair_pt.dev_id, pair_pt.data_type, pair_pt.pt_id,
						point->stn_id, point->dev_id, point->data_type, point->pt_id);
					Err_log(err_str);
				}
			}

			Swap2Byte(&value.ind);
			common_ptr.var_ptr->ind.status= value.ind;
			break;

		case	ANA_TYPE:
		case	OUTA_TYPE:
		case	PARAM_TYPE:
			value.anlg.data= new_value.anlg.data;
			fValue= AnlgDatatoFloat(&value.anlg);
			Swap4Byte(&value.anlg.data);
			Swap4Byte(&fValue);
			common_ptr.var_ptr->anlg.value.data= value.anlg.data;
			common_ptr.var_ptr->anlg.fValue= fValue;
			break;

		case	CALC_TYPE:
			value.calc.data= new_value.calc.data;
			fValue= AnlgDatatoFloat(&value.calc);
			Swap4Byte(&value.calc.data);
			Swap4Byte(&fValue);
			common_ptr.var_ptr->calc.value.data= value.calc.data;
			common_ptr.var_ptr->calc.fValue= fValue;
			break;

		case	IMP_TYPE:
			value.imp= new_value.imp;
			imp_opt= value.imp.status.option;
			value.imp.status.option= 0;
			Swap4Byte(&value.imp.iValue);
			Swap4Byte(&value.imp.fValue);
			switch( imp_opt )
			{
			case	RW_IMP_ACCUM:
				common_ptr.var_ptr->imp.entry.rt_value= value.imp.iValue;
				break;

			case	RW_D_ACCUM:
				common_ptr.var_ptr->imp.entry.d_accum= value.imp.fValue;
				break;

			case	RW_D_DAY_PEAK:
				common_ptr.var_ptr->imp.entry.peak.day= value.imp.iValue;
				break;

			case	RW_D_MONTH_PEAK:
				common_ptr.var_ptr->imp.entry.peak.month= value.imp.iValue;
				break;

			case	RW_D_YEAR_PEAK:
				common_ptr.var_ptr->imp.entry.peak.year= value.imp.iValue;
				break;

			case	RW_D_DAY_VALLY:
				common_ptr.var_ptr->imp.entry.valley.day= value.imp.iValue;
				break;

			case	RW_D_MONTH_VALLY:
				common_ptr.var_ptr->imp.entry.valley.month= value.imp.iValue;
				break;

			case	RW_D_YEAR_VALLY:
				common_ptr.var_ptr->imp.entry.valley.year= value.imp.iValue;
				break;

			case	RW_D_DAY_NORMAL:
				common_ptr.var_ptr->imp.entry.normal.day= value.imp.iValue;
				break;

			case	RW_D_MONTH_NORMAL:
				common_ptr.var_ptr->imp.entry.normal.month= value.imp.iValue;
				break;

			case	RW_D_YEAR_NORMAL:
				common_ptr.var_ptr->imp.entry.normal.year= value.imp.iValue;
				break;

			case	RW_D_DAY_SUM:
				common_ptr.var_ptr->imp.entry.sum.day= value.imp.iValue;
				break;

			case	RW_D_MONTH_SUM:
				common_ptr.var_ptr->imp.entry.sum.month= value.imp.iValue;
				break;

			case	RW_D_YEAR_SUM:
				common_ptr.var_ptr->imp.entry.sum.year= value.imp.iValue;
				break;

			case	RW_D_DAY_PPEAK:
				common_ptr.var_ptr->imp.entry.ppeak.day= value.imp.iValue;
				break;

			case	RW_D_MONTH_PPEAK:
				common_ptr.var_ptr->imp.entry.ppeak.month= value.imp.iValue;
				break;

			case	RW_D_YEAR_PPEAK:
				common_ptr.var_ptr->imp.entry.ppeak.year= value.imp.iValue;
				break;

			default:
				sprintf(err_str, "imp_opt error: %d", imp_opt);
				Err_log(err_str);
				return -1;
			}
			break;

		default:
			sprintf(err_str, "data_type error: %d", point->data_type);
			Err_log(err_str);
			return -1;
		}

	}

	if( rw_flag & RW_FAULT_TAG )
	{
		if( (point->data_type==SOE_TYPE)
			||(point->data_type==POL_TYPE)
			||(point->data_type==OUTS_TYPE)
			||(point->data_type==SYSMSG_TYPE))
		{
			value.ind= common_ptr.var_ptr->ind.status;
			Swap2Byte(&value.ind);
			value.ind.fault_tag= new_value.ind.fault_tag;
			Swap2Byte(&value.ind);
			common_ptr.var_ptr->ind.status= value.ind;
		}
		else
		{
			sprintf(err_str, "data_type error: %d", point->data_type);
			Err_log(err_str);
			return -1;
		}
	}

	if( rw_flag & RW_MAN_SET ) /*also RW_CTRL_BLOCK*/
	{
		switch	( point->data_type )
		{
		case	SOE_TYPE:
		case	POL_TYPE:
		case	OUTS_TYPE:
		case	SYSMSG_TYPE:
			value.ind= common_ptr.var_ptr->ind.status;
			Swap2Byte(&value.ind);
			value.ind.man_set= new_value.ind.man_set;
			Swap2Byte(&value.ind);
			common_ptr.var_ptr->ind.status= value.ind;
			break;

		case	ANA_TYPE:
		case	OUTA_TYPE:
		case	PARAM_TYPE:
			value.anlg.status= common_ptr.var_ptr->anlg.value.status;
			Swap2Byte(&value.anlg.status);
			value.anlg.status.ctrl_block= new_value.anlg.status.ctrl_block;
			Swap2Byte(&value.anlg.status);
			common_ptr.var_ptr->anlg.value.status= value.anlg.status;
			break;

		case	CALC_TYPE:
			value.calc.status= common_ptr.var_ptr->calc.value.status;
			Swap2Byte(&value.calc.status);
			value.calc.status.ctrl_block= new_value.calc.status.ctrl_block;
			Swap2Byte(&value.calc.status);
			common_ptr.var_ptr->calc.value.status= value.calc.status;
			break;

		case	IMP_TYPE:
			value.imp.status= common_ptr.var_ptr->imp.entry.status;
			Swap2Byte(&value.imp.status);
			value.imp.status.man_set= new_value.imp.status.man_set;
			Swap2Byte(&value.imp.status);
			common_ptr.var_ptr->imp.entry.status= value.imp.status;
			break;

		default:
			sprintf(err_str, "data_type error: %d", point->data_type);
			Err_log(err_str);
			return -1;
		}

	}

	if( rw_flag & RW_OVERFLOW )
	{
		switch	( point->data_type )
		{
		case	ANA_TYPE:
		case	OUTA_TYPE:
		case	PARAM_TYPE:
			value.anlg.status= common_ptr.var_ptr->anlg.value.status;
			Swap2Byte(&value.anlg.status);
			value.anlg.status.overflow= new_value.anlg.status.overflow;
			Swap2Byte(&value.anlg.status);
			common_ptr.var_ptr->anlg.value.status= value.anlg.status;
			break;

		case	CALC_TYPE:
			value.calc.status= common_ptr.var_ptr->calc.value.status;
			Swap2Byte(&value.calc.status);
			value.calc.status.overflow= new_value.calc.status.overflow;
			Swap2Byte(&value.calc.status);
			common_ptr.var_ptr->calc.value.status= value.calc.status;
			break;

		default:
			sprintf(err_str, "data_type error: %d", point->data_type);
			Err_log(err_str);
			return -1;
		}

	}

	if( rw_flag & RW_CHAN_FAIL )
	{
		switch	( point->data_type )
		{
		case	SOE_TYPE:
		case	POL_TYPE:
		case	OUTS_TYPE:
		case	SYSMSG_TYPE:
			value.ind= common_ptr.var_ptr->ind.status;
			Swap2Byte(&value.ind);
			value.ind.chan_fail= new_value.ind.chan_fail;
			Swap2Byte(&value.ind);
			common_ptr.var_ptr->ind.status= value.ind;
			break;

		case	ANA_TYPE:
		case	OUTA_TYPE:
		case	PARAM_TYPE:
			value.anlg.status= common_ptr.var_ptr->anlg.value.status;
			Swap2Byte(&value.anlg.status);
			value.anlg.status.chan_fail= new_value.anlg.status.chan_fail;
			Swap2Byte(&value.anlg.status);
			common_ptr.var_ptr->anlg.value.status= value.anlg.status;
			break;

		case	CALC_TYPE:
			value.calc.status= common_ptr.var_ptr->calc.value.status;
			Swap2Byte(&value.calc.status);
			value.calc.status.chan_fail= new_value.calc.status.chan_fail;
			Swap2Byte(&value.calc.status);
			common_ptr.var_ptr->calc.value.status= value.calc.status;
			break;

		case	IMP_TYPE:
			value.imp.status= common_ptr.var_ptr->imp.entry.status;
			Swap2Byte(&value.imp.status);
			value.imp.status.chan_fail= new_value.imp.status.chan_fail;
			Swap2Byte(&value.imp.status);
			common_ptr.var_ptr->imp.entry.status= value.imp.status;
			break;

		default:
			sprintf(err_str, "data_type error: %d", point->data_type);
			Err_log(err_str);
			return -1;
		}

	}

	if( rw_flag & RW_LMT_STATE )
	{
		if( (common_ptr.point.data_type == ANA_TYPE)
			||(common_ptr.point.data_type == OUTA_TYPE)
			||(common_ptr.point.data_type == CALC_TYPE)
			||(common_ptr.point.data_type == PARAM_TYPE))
		{
			value.anlg.status= common_ptr.var_ptr->anlg.value.status;
			Swap2Byte(&value.anlg.status);
			value.anlg.status.lmt_state= new_value.anlg.status.lmt_state;
			if(value.anlg.status.lmt_state==ANA_ALM_HI_RECOVER 
				|| value.anlg.status.lmt_state==ANA_ALM_LO_RECOVER
				||value.anlg.status.lmt_state==ANA_ALM_HH_RECOVER 
				|| value.anlg.status.lmt_state==ANA_ALM_LL_RECOVER )
				value.anlg.status.lmt_state= ANA_ALM_NORMAL;
			Swap2Byte(&value.anlg.status);
			common_ptr.var_ptr->anlg.value.status= value.anlg.status;
		}
		else
		{
			sprintf(err_str, "data_type error: %d", point->data_type);
			Err_log(err_str);
			return -1;
		}
	}

	if( rw_flag & RW_TREND_STATE )
	{
		if( (common_ptr.point.data_type == ANA_TYPE)
			||(common_ptr.point.data_type == OUTA_TYPE)
			||(common_ptr.point.data_type == CALC_TYPE)
			||(common_ptr.point.data_type == PARAM_TYPE))
		{
			value.anlg.status= common_ptr.var_ptr->anlg.value.status;
			Swap2Byte(&value.anlg.status);
			value.anlg.status.trend_state= new_value.anlg.status.trend_state;
			if(value.anlg.status.trend_state==ANA_ALM_TR_RECOVER)
				value.anlg.status.trend_state= ANA_ALM_NORMAL;
			Swap2Byte(&value.anlg.status);
			common_ptr.var_ptr->anlg.value.status= value.anlg.status;
		}
		else
		{
			sprintf(err_str, "data_type error: %d", point->data_type);
			Err_log(err_str);
			return -1;
		}
	}

	return 0;
}

	
int	SetPtValueIostByNameStr( char *ptNameStr, WR_VAR_UNION value, 
				IOST iost, DATE_TIME update_time, enum RW_FLAG rw_flag )
{
	POINTER	point;

	CheckDbValid();
	if( -1==GetPtIdByNameStr(ptNameStr, &point) )
		return -1;

	point.update_src= 0;
	return SetPtValueIostById( &point, value, iost, update_time, rw_flag );
}
				
			
int	SetPtIoattrById( POINTER *point, IOATTR ioattr, enum RW_IOATTR_FLAG rw_flag )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr_new;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr_new= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr_new);
	if( rw_flag & RW_PROCTYPE )
		ioattr_new.procType= ioattr.procType;	
	if( rw_flag & RW_ALM_CBA )
		ioattr_new.almCba= ioattr.almCba;	
	if( rw_flag & RW_STATISTIC )
		ioattr_new.statistic= ioattr.statistic;	
	if( rw_flag & RW_FAULT )
		ioattr_new.fault= ioattr.fault;	
	if( rw_flag & RW_FAIL )
		ioattr_new.fail= ioattr.fail;	
	if( rw_flag & RW_CTRL )
		ioattr_new.ctrl= ioattr.ctrl;	
	if( rw_flag & RW_SOE )
		ioattr_new.soe= ioattr.soe;	
	if( rw_flag & RW_TREND_ALM )
		ioattr_new.trendAlm= ioattr.trendAlm;	
	if( rw_flag & RW_KEY_SIGNAL )
		ioattr_new.keySignal= ioattr.keySignal;	
	if( rw_flag & RW_FAULT_REC )
		ioattr_new.faultRec= ioattr.faultRec;	
	if( rw_flag & RW_AUD_ALM )
		ioattr_new.audAlm= ioattr.audAlm;	
	if( rw_flag & RW_NORM_CLOSE )
		ioattr_new.normalClose= ioattr.normalClose;	
	if( rw_flag & RW_DOUBLE_PT )
		ioattr_new.doublePt= ioattr.doublePt;	
	if( rw_flag & RW_SYS_VIRTUAL )
		ioattr_new.sysVirtual= ioattr.sysVirtual;	
	if( rw_flag & RW_LCU_VIRTUAL )
		ioattr_new.lcuVirtual= ioattr.lcuVirtual;	
	if( rw_flag & RW_CASCADE )
		ioattr_new.cascade= ioattr.cascade;	
	if( rw_flag & RW_HDB_REC )
		ioattr_new.hdbRec= ioattr.hdbRec;	
	if( rw_flag & RW_HDB_CURVE )
		ioattr_new.hdbCurve= ioattr.hdbCurve;	
	if( rw_flag & RW_HDB_PERIOD )
		ioattr_new.hdbPeriod= ioattr.hdbPeriod;	
	if( rw_flag & RW_ON_ALM )
		ioattr_new.onAlm= ioattr.onAlm;	
	if( rw_flag & RW_OFF_ALM )
		ioattr_new.offAlm= ioattr.offAlm;	
	if( rw_flag & RW_NORM_STATE )
		ioattr_new.normalState= ioattr.normalState;	
					
	Swap4Byte(&ioattr_new);
	common_ptr.fixed_ptr->ioattr= ioattr_new;
	return 0;
}


int	SetPtIoattrByNameStr( char *ptNameStr, IOATTR ioattr, enum RW_IOATTR_FLAG rw_flag )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr_new;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr_new= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr_new);
	if( rw_flag & RW_PROCTYPE )
		ioattr_new.procType= ioattr.procType;	
	if( rw_flag & RW_ALM_CBA )
		ioattr_new.almCba= ioattr.almCba;	
	if( rw_flag & RW_STATISTIC )
		ioattr_new.statistic= ioattr.statistic;	
	if( rw_flag & RW_FAULT )
		ioattr_new.fault= ioattr.fault;	
	if( rw_flag & RW_FAIL )
		ioattr_new.fail= ioattr.fail;	
	if( rw_flag & RW_CTRL )
		ioattr_new.ctrl= ioattr.ctrl;	
	if( rw_flag & RW_SOE )
		ioattr_new.soe= ioattr.soe;	
	if( rw_flag & RW_TREND_ALM )
		ioattr_new.trendAlm= ioattr.trendAlm;	
	if( rw_flag & RW_KEY_SIGNAL )
		ioattr_new.keySignal= ioattr.keySignal;	
	if( rw_flag & RW_FAULT_REC )
		ioattr_new.faultRec= ioattr.faultRec;	
	if( rw_flag & RW_AUD_ALM )
		ioattr_new.audAlm= ioattr.audAlm;	
	if( rw_flag & RW_NORM_CLOSE )
		ioattr_new.normalClose= ioattr.normalClose;	
	if( rw_flag & RW_DOUBLE_PT )
		ioattr_new.doublePt= ioattr.doublePt;	
	if( rw_flag & RW_SYS_VIRTUAL )
		ioattr_new.sysVirtual= ioattr.sysVirtual;	
	if( rw_flag & RW_LCU_VIRTUAL )
		ioattr_new.lcuVirtual= ioattr.lcuVirtual;	
	if( rw_flag & RW_CASCADE )
		ioattr_new.cascade= ioattr.cascade;	
	if( rw_flag & RW_HDB_REC )
		ioattr_new.hdbRec= ioattr.hdbRec;	
	if( rw_flag & RW_HDB_CURVE )
		ioattr_new.hdbCurve= ioattr.hdbCurve;	
	if( rw_flag & RW_ON_ALM )
		ioattr_new.onAlm= ioattr.onAlm;	
	if( rw_flag & RW_OFF_ALM )
		ioattr_new.offAlm= ioattr.offAlm;	
	if( rw_flag & RW_NORM_STATE )
		ioattr_new.normalState= ioattr.normalState;	
					
	Swap4Byte(&ioattr_new);
	common_ptr.fixed_ptr->ioattr= ioattr_new;
	return 0;
}



int	SetImpPtVarById( POINTER *point, ENTRY_IMP_VAL imp_var)
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( point->data_type != IMP_TYPE )
		return -1;

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	SwapImpEngParaByte(&imp_var.eng_para);
	SwapDateTimeByte(&imp_var.update_time);
	Swap4Byte(&imp_var.rt_value);
	Swap2Byte(&imp_var.status);
	Swap4Byte(&imp_var.base_value);
	Swap4Byte(&imp_var.d_accum);
	SwapImpDataByte(&imp_var.init);
	SwapImpDataByte(&imp_var.last);
	SwapImpDataByte(&imp_var.ppeak);
	SwapImpDataByte(&imp_var.peak);
	SwapImpDataByte(&imp_var.valley);
	SwapImpDataByte(&imp_var.normal);
	SwapImpDataByte(&imp_var.sum);

	common_ptr.var_ptr->imp.entry= imp_var;
	common_ptr.fixed_ptr->update_src= point->update_src;
	return 0;
}


int	SetImpPtVarByNameStr( char *ptNameStr, ENTRY_IMP_VAL imp_var)
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != IMP_TYPE )
		return -1;

	SwapImpEngParaByte(&imp_var.eng_para);
	SwapDateTimeByte(&imp_var.update_time);
	Swap4Byte(&imp_var.rt_value);
	Swap2Byte(&imp_var.status);
	Swap4Byte(&imp_var.base_value);
	Swap4Byte(&imp_var.d_accum);
	SwapImpDataByte(&imp_var.init);
	SwapImpDataByte(&imp_var.last);
	SwapImpDataByte(&imp_var.ppeak);
	SwapImpDataByte(&imp_var.peak);
	SwapImpDataByte(&imp_var.valley);
	SwapImpDataByte(&imp_var.normal);
	SwapImpDataByte(&imp_var.sum);

	common_ptr.var_ptr->imp.entry= imp_var;
	return 0;
}


int	SetCharaPtDataById( POINTER *point, CHARA_DATA chara_data, DATE_TIME update_time)
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( point->data_type != CHARA_TYPE )
		return -1;

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	SwapDateTimeByte(&update_time);
	SwapCharaDataByte(&chara_data);
	common_ptr.var_ptr->chara.update_time= update_time;
	common_ptr.var_ptr->chara.chara_data= chara_data;
	common_ptr.fixed_ptr->update_src= point->update_src;
	return 0;
}


int	SetCharaPtDataByNameStr( char *ptNameStr, CHARA_DATA chara_data, DATE_TIME update_time)
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != CHARA_TYPE )
		return -1;

	SwapDateTimeByte(&update_time);
	SwapCharaDataByte(&chara_data);
	common_ptr.var_ptr->chara.update_time= update_time;
	common_ptr.var_ptr->chara.chara_data= chara_data;
	return 0;
}


int	SetAnaPtfValueById( POINTER *point, FLOAT value, DATE_TIME time )
{
	DMS_COMMON_PTR common_ptr;
	ANLG_DATA	anlg_data;
	IOST		iost_new;

	CheckDbValid();
	if( (point->data_type != ANA_TYPE)
		&&(point->data_type != OUTA_TYPE)
		&&(point->data_type != CALC_TYPE)
		&&(point->data_type != PARAM_TYPE))
		return -1;

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	FloattoAnlgData( value, &anlg_data );
	Swap2Byte(&anlg_data.status);
	Swap4Byte(&anlg_data.data);
	Swap4Byte(&value);
	common_ptr.var_ptr->anlg.value.data= anlg_data.data;
	common_ptr.var_ptr->anlg.fValue= value;

	iost_new= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost_new);
	iost_new.noUpdate= 0;	
	Swap2Byte(&iost_new);
	common_ptr.fixed_ptr->iost= iost_new;	
	SwapDateTimeByte(&time);
	common_ptr.var_ptr->anlg.update_time= time;
	common_ptr.fixed_ptr->update_src= point->update_src;
	return 0;
}


int	SetAnaPtfValueByNameStr( char *ptNameStr, FLOAT value, DATE_TIME time )
{
	DMS_COMMON_PTR common_ptr;
	ANLG_DATA	anlg_data;
	IOST		iost_new;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != ANA_TYPE)
		&&(common_ptr.point.data_type != OUTA_TYPE)
		&&(common_ptr.point.data_type != CALC_TYPE)
		&&(common_ptr.point.data_type != PARAM_TYPE))
		return -1;

	FloattoAnlgData( value, &anlg_data );
	Swap2Byte(&anlg_data.status);
	Swap4Byte(&anlg_data.data);
	Swap4Byte(&value);
	common_ptr.var_ptr->anlg.value.data= anlg_data.data;
	common_ptr.var_ptr->anlg.fValue= value;

	iost_new= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost_new);
	iost_new.noUpdate= 0;	
	Swap2Byte(&iost_new);
	common_ptr.fixed_ptr->iost= iost_new;	
	SwapDateTimeByte(&time);
	common_ptr.var_ptr->anlg.update_time= time;
	return 0;
}


int	SetAnaPtEngLmtParaById( POINTER *point, FLOAT data, enum RW_ENG_LMT_OPT opt )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( (point->data_type != ANA_TYPE)
		&&(point->data_type != OUTA_TYPE)
		&&(point->data_type != PARAM_TYPE))
		return -1;

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	Swap4Byte(&data);
	switch( opt )
	{
	case	RW_D_BAND:
		common_ptr.var_ptr->anlg.eng_para.D_BAND= data;
		break;

	case	RW_ENG_CVT:
		common_ptr.var_ptr->anlg.eng_para.ENG_CVT= data;
		break;

	case	RW_HI_RANGE:
		common_ptr.var_ptr->anlg.eng_para.HI_RANGE= data;
		break;

	case	RW_LO_RANGE:
		common_ptr.var_ptr->anlg.eng_para.LO_RANGE= data;
		break;

	case	RW_ZERO:
		common_ptr.var_ptr->anlg.eng_para.ZERO= data;
		break;

	case	RW_COMPSATE:
		common_ptr.var_ptr->anlg.eng_para.COMPSATE= data;
		break;

	case	RW_HI_CODE:
		common_ptr.var_ptr->anlg.eng_para.HI_CODE= data;
		break;

	case	RW_LO_CODE:
		common_ptr.var_ptr->anlg.eng_para.LO_CODE= data;
		break;

	case	RW_HI_ALARM:
		common_ptr.var_ptr->anlg.lmt_para.HI_ALARM= data;
		break;

	case	RW_HI_WARN:
		common_ptr.var_ptr->anlg.lmt_para.HI_WARN= data;
		break;

	case	RW_HI_RECOVER:
		common_ptr.var_ptr->anlg.lmt_para.HI_RECOVER= data;
		break;

	case	RW_LO_ALARM:
		common_ptr.var_ptr->anlg.lmt_para.LO_ALARM= data;
		break;

	case	RW_LO_WARN:
		common_ptr.var_ptr->anlg.lmt_para.LO_WARN= data;
		break;

	case	RW_LO_RECOVER:
		common_ptr.var_ptr->anlg.lmt_para.LO_RECOVER= data;
		break;

	case	RW_TR_WARN:
		common_ptr.var_ptr->anlg.lmt_para.TR_WARN= data;
		break;

	case	RW_TR_RECOVER:
		common_ptr.var_ptr->anlg.lmt_para.TR_RECOVER= data;
		break;

	default:
		return -1;
	}
	return 0;
}


int	SetAnaPtEngLmtParaByNameStr( char *ptNameStr, FLOAT data, enum RW_ENG_LMT_OPT opt )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != ANA_TYPE)
		&&(common_ptr.point.data_type != OUTA_TYPE)
		&&(common_ptr.point.data_type != PARAM_TYPE))
		return -1;

	Swap4Byte(&data);
	switch( opt )
	{
	case	RW_D_BAND:
		common_ptr.var_ptr->anlg.eng_para.D_BAND= data;
		break;

	case	RW_ENG_CVT:
		common_ptr.var_ptr->anlg.eng_para.ENG_CVT= data;
		break;

	case	RW_HI_RANGE:
		common_ptr.var_ptr->anlg.eng_para.HI_RANGE= data;
		break;

	case	RW_LO_RANGE:
		common_ptr.var_ptr->anlg.eng_para.LO_RANGE= data;
		break;

	case	RW_ZERO:
		common_ptr.var_ptr->anlg.eng_para.ZERO= data;
		break;

	case	RW_COMPSATE:
		common_ptr.var_ptr->anlg.eng_para.COMPSATE= data;
		break;

	case	RW_HI_CODE:
		common_ptr.var_ptr->anlg.eng_para.HI_CODE= data;
		break;

	case	RW_LO_CODE:
		common_ptr.var_ptr->anlg.eng_para.LO_CODE= data;
		break;

	case	RW_HI_ALARM:
		common_ptr.var_ptr->anlg.lmt_para.HI_ALARM= data;
		break;

	case	RW_HI_WARN:
		common_ptr.var_ptr->anlg.lmt_para.HI_WARN= data;
		break;

	case	RW_HI_RECOVER:
		common_ptr.var_ptr->anlg.lmt_para.HI_RECOVER= data;
		break;

	case	RW_LO_ALARM:
		common_ptr.var_ptr->anlg.lmt_para.LO_ALARM= data;
		break;

	case	RW_LO_WARN:
		common_ptr.var_ptr->anlg.lmt_para.LO_WARN= data;
		break;

	case	RW_LO_RECOVER:
		common_ptr.var_ptr->anlg.lmt_para.LO_RECOVER= data;
		break;

	case	RW_TR_WARN:
		common_ptr.var_ptr->anlg.lmt_para.TR_WARN= data;
		break;

	case	RW_TR_RECOVER:
		common_ptr.var_ptr->anlg.lmt_para.TR_RECOVER= data;
		break;

	default:
		return -1;
	}
	return 0;
}


int	SetIndPtStcById( POINTER *point, USHORT data, enum RW_STC_OPT opt )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( (point->data_type!=SOE_TYPE)
		&&(point->data_type!=POL_TYPE)
		&&(point->data_type!=OUTS_TYPE)
		&&(point->data_type!=SYSMSG_TYPE))
		return -1;

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	Swap2Byte(&data);
	switch( opt )
	{
	case	RW_STC_NORMAL_OPEN:
		common_ptr.var_ptr->ind.stc.norm_open= data;
		break;

	case	RW_STC_NORMAL_CLOSE:
		common_ptr.var_ptr->ind.stc.norm_close= data;
		break;

	case	RW_STC_FAULT_OPEN:
		common_ptr.var_ptr->ind.stc.fault_open= data;
		break;

	case	RW_STC_FAULT_CLOSE:
		common_ptr.var_ptr->ind.stc.fault_close= data;
		break;

	default:
		return -1;
	}
	return 0;
}


int	SetIndPtStcByNameStr( char *ptNameStr, USHORT data, enum RW_STC_OPT opt )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type!=SOE_TYPE)
		&&(common_ptr.point.data_type!=POL_TYPE)
		&&(common_ptr.point.data_type!=OUTS_TYPE)
		&&(common_ptr.point.data_type!=SYSMSG_TYPE))
		return -1;

	Swap2Byte(&data);
	switch( opt )
	{
	case	RW_STC_NORMAL_OPEN:
		common_ptr.var_ptr->ind.stc.norm_open= data;
		break;

	case	RW_STC_NORMAL_CLOSE:
		common_ptr.var_ptr->ind.stc.norm_close= data;
		break;

	case	RW_STC_FAULT_OPEN:
		common_ptr.var_ptr->ind.stc.fault_open= data;
		break;

	case	RW_STC_FAULT_CLOSE:
		common_ptr.var_ptr->ind.stc.fault_close= data;
		break;

	default:
		return -1;
	}
	return 0;
}


int	SetIndPtAlmMsgSelById( POINTER *point, UCHAR sel )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( (point->data_type!=SOE_TYPE)
		&&(point->data_type!=POL_TYPE)
		&&(point->data_type!=OUTS_TYPE)
		&&(point->data_type!=SYSMSG_TYPE))
		return -1;

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	common_ptr.fixed_ptr->msg_sel.alm= sel;
	return 0;
}


int	SetIndPtAlmMsgSelByNameStr( char *ptNameStr, UCHAR sel )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type!=SOE_TYPE)
		&&(common_ptr.point.data_type!=POL_TYPE)
		&&(common_ptr.point.data_type!=OUTS_TYPE)
		&&(common_ptr.point.data_type!=SYSMSG_TYPE))
		return -1;

	common_ptr.fixed_ptr->msg_sel.alm= sel;
	return 0;
}



int	SetPtCmdMsgSelById( POINTER *point, UCHAR sel )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	common_ptr.fixed_ptr->msg_sel.cmd= sel;
	return 0;
}


int	SetPtCmdMsgSelByNameStr( char *ptNameStr, UCHAR sel )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	common_ptr.fixed_ptr->msg_sel.cmd= sel;
	return 0;
}


int	SetPtCtrlAddrById( POINTER *point, USHORT addr )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	Swap2Byte(&addr);
	common_ptr.fixed_ptr->ctrl_addr= addr;
	return 0;
}


int	SetPtCtrlAddrByNameStr( char *ptNameStr, USHORT addr )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	Swap2Byte(&addr);
	common_ptr.fixed_ptr->ctrl_addr= addr;
	return 0;
}



int	SetPtIoGroupById( POINTER *point, USHORT io_group )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	Swap2Byte(&io_group);
	common_ptr.fixed_ptr->io_group= io_group;
	return 0;
}


int	SetPtIoGroupByNameStr( char *ptNameStr, USHORT io_group )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	Swap2Byte(&io_group);
	common_ptr.fixed_ptr->io_group= io_group;
	return 0;
}



int SetCurvePtDataById( POINTER *point, FLOAT data[], USHORT seg_offset, 
					   USHORT seg_num, DATE_TIME time )
{
	DMS_COMMON_PTR common_ptr;
	char		err_str[128];
	USHORT		i;
	FLOAT		fValue;
	USHORT		num;

	CheckDbValid();
	if( point->data_type!=CURVE_TYPE )
	{
		sprintf(err_str, "Not curve type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	num= common_ptr.var_ptr->curve.seg_num;
	Swap2Byte(&num);
	if( (seg_offset+seg_num)>num )
	{
		sprintf(err_str, "Curve point seg pos overrange: offset=%d, num=%d", 
			seg_offset, seg_num);
		Err_log(err_str);
		return -1;
	}

	for(i=0; i<seg_num; i++)
	{
		fValue= data[i];
		Swap4Byte(&fValue);
		common_ptr.var_ptr->curve.data.value[i+seg_offset]= fValue;
	}

	SwapDateTimeByte(&time);
	common_ptr.var_ptr->curve.update_time= time;
	common_ptr.fixed_ptr->update_src= point->update_src;

	return 0;
}




int SetCurvePtDataByNameStr( char *ptNameStr, FLOAT data[], USHORT seg_offset, 
					   USHORT seg_num, DATE_TIME time )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	USHORT		i;
	FLOAT		fValue;
	USHORT		num;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != CURVE_TYPE )
	{
		sprintf(err_str, "Not curve type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	num= common_ptr.var_ptr->curve.seg_num;
	Swap2Byte(&num);
	if( (seg_offset+seg_num)>num )
	{
		sprintf(err_str, "Curve point seg pos overrange: offset=%d, num=%d", 
			seg_offset, seg_num);
		Err_log(err_str);
		return -1;
	}

	for(i=0; i<seg_num; i++)
	{
		fValue= data[i];
		Swap4Byte(&fValue);
		common_ptr.var_ptr->curve.data.value[i+seg_offset]= fValue;
	}

	SwapDateTimeByte(&time);
	common_ptr.var_ptr->curve.update_time= time;

	return 0;
}


int	SetCurvePtEngParaById( POINTER *point, FLOAT data, enum RW_ENG_LMT_OPT opt )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( point->data_type != CURVE_TYPE )
		return -1;

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	Swap4Byte(&data);
	switch( opt )
	{
	case	RW_ENG_CVT:
		common_ptr.var_ptr->curve.eng_para.ENG_CVT= data;
		break;

	case	RW_HI_RANGE:
		common_ptr.var_ptr->curve.eng_para.HI_RANGE= data;
		break;

	case	RW_LO_RANGE:
		common_ptr.var_ptr->curve.eng_para.LO_RANGE= data;
		break;

	default:
		return -1;
	}
	return 0;
}


int	SetCurvePtEngParaByNameStr( char *ptNameStr, FLOAT data, enum RW_ENG_LMT_OPT opt )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != CURVE_TYPE )
		return -1;

	Swap4Byte(&data);
	switch( opt )
	{
	case	RW_ENG_CVT:
		common_ptr.var_ptr->curve.eng_para.ENG_CVT= data;
		break;

	case	RW_HI_RANGE:
		common_ptr.var_ptr->curve.eng_para.HI_RANGE= data;
		break;

	case	RW_LO_RANGE:
		common_ptr.var_ptr->curve.eng_para.LO_RANGE= data;
		break;

	default:
		return -1;
	}
	return 0;
}


int SetPairPtById( POINTER *point, POINTER pair_pt )
{
	DMS_COMMON_PTR common_ptr, common_ptr1;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	if( -1==GetCommonPtrById(&pair_pt, &common_ptr1) )
		return -1;

	Swap2Byte(&pair_pt.pt_id);
	common_ptr.fixed_ptr->pair_pt= pair_pt;
	if(common_ptr1.fixed_ptr->ctrl_addr)
		common_ptr.fixed_ptr->ctrl_addr= common_ptr1.fixed_ptr->ctrl_addr;
	if(common_ptr1.fixed_ptr->ioattr.ctrl)
		common_ptr.fixed_ptr->ioattr.ctrl= common_ptr1.fixed_ptr->ioattr.ctrl;
	if(point->data_type==PARAM_TYPE)
	{
		common_ptr.var_ptr->anlg.eng_para.HI_RANGE= common_ptr1.var_ptr->anlg.eng_para.HI_RANGE;
		common_ptr.var_ptr->anlg.eng_para.LO_RANGE= common_ptr1.var_ptr->anlg.eng_para.LO_RANGE;
	}
	return 0;
}


int SetPairPtByNameStr( char *ptNameStr, POINTER pair_pt )
{
	DMS_COMMON_PTR common_ptr, common_ptr1;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;
	if( -1==GetCommonPtrById(&pair_pt, &common_ptr1) )
		return -1;

	Swap2Byte(&pair_pt.pt_id);
	common_ptr.fixed_ptr->pair_pt= pair_pt;
	if(common_ptr1.fixed_ptr->ctrl_addr)
		common_ptr.fixed_ptr->ctrl_addr= common_ptr1.fixed_ptr->ctrl_addr;
	if(common_ptr1.fixed_ptr->ioattr.ctrl)
		common_ptr.fixed_ptr->ioattr.ctrl= common_ptr1.fixed_ptr->ioattr.ctrl;
	if(common_ptr.point.data_type==PARAM_TYPE)
	{
		common_ptr.var_ptr->anlg.eng_para.HI_RANGE= common_ptr1.var_ptr->anlg.eng_para.HI_RANGE;
		common_ptr.var_ptr->anlg.eng_para.LO_RANGE= common_ptr1.var_ptr->anlg.eng_para.LO_RANGE;
	}
	return 0;
}


int SetDevObjPtVarById( POINTER *point, ENTRY_DEVOBJ_VAL devobj_var )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	SwapDateTimeByte(&devobj_var.update_time);
	SwapDateTimeByte(&devobj_var.cmd_time);
	Swap2Byte(&devobj_var.status);
	Swap4Byte(&devobj_var.data);
	Swap2Byte(&devobj_var.fb_status1);
	Swap2Byte(&devobj_var.fb_status2);
	Swap2Byte(&devobj_var.param);
	common_ptr.var_ptr->devobj= devobj_var;
	common_ptr.fixed_ptr->update_src= point->update_src;
	return 0;
}


int	SetDevObjPtVarByNameStr( char *ptNameStr, ENTRY_DEVOBJ_VAL devobj_var )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	SwapDateTimeByte(&devobj_var.update_time);
	SwapDateTimeByte(&devobj_var.cmd_time);
	Swap2Byte(&devobj_var.status);
	Swap4Byte(&devobj_var.data);
	Swap2Byte(&devobj_var.fb_status1);
	Swap2Byte(&devobj_var.fb_status2);
	Swap2Byte(&devobj_var.param);
	common_ptr.var_ptr->devobj= devobj_var;
	return 0;
}


