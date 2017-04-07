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



int GetEntryNameById( POINTER *point, char *entryName )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	strcpy( entryName, common_ptr.fixed_ptr->entryName );
	return 0;
}



int GetEntryLongnameById( POINTER *point, char *entryLongname )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	strcpy( entryLongname, common_ptr.fixed_ptr->entryLongname );
	return 0;
}

int GetEntryLongnameByNameStr( char *ptNameStr, char *entryLongname )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	strcpy( entryLongname, common_ptr.fixed_ptr->entryName );
	return 0;
}


int	GetPtNameById( POINTER *point, char *ptNameStr, POINT_NAME *pt_name )
{
	int	ret;
	POINT_NAME	point_name;
		
	CheckDbValid();
	ret= GetStnNameById( point->stn_id, point_name.stnName );
	if( ret==-1 )
		return -1;
		
	ret= GetGroupNameById( point->stn_id, point->dev_id, point_name.grpName );
	if( ret==-1 )
		return -1;
		
	ret= GetRecNameByType( point->data_type, point_name.recName );
	if( ret==-1 )
		return -1;
		
	ret= GetEntryNameById( point, point_name.entryName );
	if( ret==-1 )
		return -1;
	
	if( ptNameStr != NULL )
		PointNametoStr(&point_name, ptNameStr);
	if( pt_name != NULL )
		*pt_name= point_name;
	return 0;
}


int	GetPtLongnameById( POINTER *point, char *pointLongname )
{
	int	ret;
	char	str[POINT_LONGNAME_SIZE];
		
	CheckDbValid();
	ret= GetStnLongnameById( point->stn_id, str );
	if( ret==-1 )
		return -1;
	strcpy(pointLongname, str);
		
	ret= GetGroupLongnameById( point->stn_id, point->dev_id, str );
	if( ret==-1 )
		return -1;
	strcat(pointLongname, str);
		
	ret= GetEntryLongnameById( point, str );
	if( ret==-1 )
		return -1;
	strcat(pointLongname, str);
		
	return 0;
}



int GetPtLongnameByNameStr( char *ptNameStr, char *pointLongname )
{
	int	ret;
	char	str[POINT_LONGNAME_SIZE];
	POINTER	point;
		
	CheckDbValid();
	ret= GetPtIdByNameStr(ptNameStr, &point);
	if( ret==-1 )
		return -1;

	ret= GetStnLongnameById( point.stn_id, str );
	if( ret==-1 )
		return -1;
	strcpy(pointLongname, str);
		
	ret= GetGroupLongnameById( point.stn_id, point.dev_id, str );
	if( ret==-1 )
		return -1;
	strcat(pointLongname, str);
		
	ret= GetEntryLongnameById( &point, str );
	if( ret==-1 )
		return -1;
	strcat(pointLongname, str);
		
	return 0;
}


int GetPtNodenameById( POINTER *point, char *nodeName )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	strcpy( nodeName, common_ptr.fixed_ptr->nodeName );
	return 0;
}


int	GetPtNodenameByNameStr( char *ptNameStr, char *nodeName )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	strcpy( nodeName, common_ptr.fixed_ptr->nodeName );
	return 0;
}





int	GetPtCmdMsgSelById( POINTER *point, UCHAR *cmd_sel )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*cmd_sel= common_ptr.fixed_ptr->msg_sel.cmd;
	return 0;
}


int	GetPtCmdMsgSelByNameStr( char *ptNameStr, UCHAR *cmd_sel )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	*cmd_sel= common_ptr.fixed_ptr->msg_sel.cmd;
	return 0;
}


int	GetPtAlmMsgSelById( POINTER *point, UCHAR *alm_sel )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*alm_sel= common_ptr.fixed_ptr->msg_sel.alm;
	return 0;
}

int	GetPtAlmMsgSelByNameStr( char *ptNameStr, UCHAR *alm_sel )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	*alm_sel= common_ptr.fixed_ptr->msg_sel.alm;
	return 0;
}


int	GetPtHardAddrById( POINTER *point, HARD_ADDR *hard_addr )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*hard_addr= common_ptr.fixed_ptr->hard_addr;
	return 0;
}

int	GetPtHardAddrByNameStr( char *ptNameStr, HARD_ADDR *hard_addr )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	*hard_addr= common_ptr.fixed_ptr->hard_addr;
	return 0;
}



int	GetPtCtrlAddrById( POINTER *point, USHORT *ctrl_addr )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*ctrl_addr= common_ptr.fixed_ptr->ctrl_addr;
	Swap2Byte(ctrl_addr);
	return 0;
}


int	GetPtCtrlAddrByNameStr( char *ptNameStr, USHORT *ctrl_addr )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	*ctrl_addr= common_ptr.fixed_ptr->ctrl_addr;
	Swap2Byte(ctrl_addr);
	return 0;
}


int	GetPtUpdateSrcById( POINTER *point, CHAR *update_src )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*update_src= common_ptr.fixed_ptr->update_src;
	return 0;
}


int	GetPtUpdateSrcByNameStr( char *ptNameStr, CHAR *update_src )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	*update_src= common_ptr.fixed_ptr->update_src;
	return 0;
}

int	GetPtIoGroupById( POINTER *point, USHORT *io_group )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*io_group= common_ptr.fixed_ptr->io_group;
	Swap2Byte(io_group);
	return 0;
}


int	GetPtIoGroupByNameStr( char *ptNameStr, USHORT *io_group )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	*io_group= common_ptr.fixed_ptr->io_group;
	Swap2Byte(io_group);
	return 0;
}


int	GetPtDspFileNameById( POINTER *point, char *dsp_filename )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	strcpy(dsp_filename, common_ptr.fixed_ptr->dsp_filename);
	return 0;
}



int	GetPtDspFileNameByNameStr( char *ptNameStr, char *dsp_filename )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	strcpy(dsp_filename, common_ptr.fixed_ptr->dsp_filename);
	return 0;
}


int GetPairPtById( POINTER *point, POINTER *pair_pt )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*pair_pt= common_ptr.fixed_ptr->pair_pt;
	Swap2Byte(&pair_pt->pt_id);
	return 0;
}


int GetPairPtByNameStr( char *ptNameStr, POINTER *pair_pt )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	*pair_pt= common_ptr.fixed_ptr->pair_pt;
	Swap2Byte(&pair_pt->pt_id);
	return 0;
}




int	GetPtUpdateTimeById( POINTER *point, DATE_TIME *update_time )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	switch	( point->data_type )
	{
	case	SOE_TYPE:
	case	POL_TYPE:
	case	OUTS_TYPE:
	case	SYSMSG_TYPE:
		*update_time= common_ptr.var_ptr->ind.update_time;
		break;

	case	ANA_TYPE:
	case	OUTA_TYPE:
	case	PARAM_TYPE:
		*update_time= common_ptr.var_ptr->anlg.update_time;
		break;

	case	IMP_TYPE:
		*update_time= common_ptr.var_ptr->imp.entry.update_time;
		break;

	case	CALC_TYPE:
		*update_time= common_ptr.var_ptr->calc.update_time;
		break;

	case	CHARA_TYPE:
		*update_time= common_ptr.var_ptr->chara.update_time;
		break;

	case	CURVE_TYPE:
		*update_time= common_ptr.var_ptr->curve.update_time;
		break;

	case	DEVOBJ_TYPE:
		*update_time= common_ptr.var_ptr->devobj.update_time;
		break;

	default:
		return -1;
	}
	SwapDateTimeByte(update_time);
	return 0;
}



int	GetPtUpdateTimeByNameStr( char *ptNameStr, DATE_TIME *update_time )
{
	DMS_COMMON_PTR common_ptr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	switch	( common_ptr.point.data_type )
	{
	case	SOE_TYPE:
	case	POL_TYPE:
	case	OUTS_TYPE:
	case	SYSMSG_TYPE:
		*update_time= common_ptr.var_ptr->ind.update_time;
		break;

	case	ANA_TYPE:
	case	OUTA_TYPE:
	case	PARAM_TYPE:
		*update_time= common_ptr.var_ptr->anlg.update_time;
		break;

	case	IMP_TYPE:
		*update_time= common_ptr.var_ptr->imp.entry.update_time;
		break;

	case	CALC_TYPE:
		*update_time= common_ptr.var_ptr->calc.update_time;
		break;

	case	CHARA_TYPE:
		*update_time= common_ptr.var_ptr->chara.update_time;
		break;

	case	CURVE_TYPE:
		*update_time= common_ptr.var_ptr->curve.update_time;
		break;

	case	DEVOBJ_TYPE:
		*update_time= common_ptr.var_ptr->devobj.update_time;
		break;

	default:
		return -1;
	}
	SwapDateTimeByte(update_time);
	return 0;
}


int	IsPtValueManSetById( POINTER *point, BOOL *manSet )
{
	DMS_COMMON_PTR common_ptr;
	IND_DATA	ind_data;
	IMP_STATUS	imp_data;
	char		err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	switch	( point->data_type )
	{
	case	SOE_TYPE:
	case	POL_TYPE:
	case	OUTS_TYPE:
	case	SYSMSG_TYPE:
		ind_data= common_ptr.var_ptr->ind.status;
		Swap2Byte(&ind_data);
		*manSet= ind_data.man_set;
		return 0;

	case	ANA_TYPE:
	case	OUTA_TYPE:
	case	PARAM_TYPE:
		sprintf(err_str, "data_type error: %d", point->data_type);
		Err_log(err_str);
		return -1;

	case	IMP_TYPE:
		imp_data= common_ptr.var_ptr->imp.entry.status;
		Swap2Byte(&imp_data);
		*manSet= imp_data.man_set;
		return 0;

	default:
		return -1;
	}
}



int	IsPtValueManSetByNameStr( char *ptNameStr, BOOL *manSet )
{
	DMS_COMMON_PTR common_ptr;
	IND_DATA	ind_data;
	IMP_STATUS	imp_data;
	char		err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	switch	( common_ptr.point.data_type )
	{
	case	SOE_TYPE:
	case	POL_TYPE:
	case	OUTS_TYPE:
	case	SYSMSG_TYPE:
		ind_data= common_ptr.var_ptr->ind.status;
		Swap2Byte(&ind_data);
		*manSet= ind_data.man_set;
		return 0;

	case	ANA_TYPE:
	case	OUTA_TYPE:
	case	PARAM_TYPE:
		sprintf(err_str, "data_type error: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;

	case	IMP_TYPE:
		imp_data= common_ptr.var_ptr->imp.entry.status;
		Swap2Byte(&imp_data);
		*manSet= imp_data.man_set;
		return 0;

	default:
		return -1;
	}
}



int	IsPtValueChanFailById( POINTER *point, BOOL *chanFail)
{
	DMS_COMMON_PTR common_ptr;
	IND_DATA	ind_data;
	ANLG_STATUS	anlg_data;
	IMP_STATUS	imp_data;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	switch	( point->data_type )
	{
	case	SOE_TYPE:
	case	POL_TYPE:
	case	OUTS_TYPE:
	case	SYSMSG_TYPE:
		ind_data= common_ptr.var_ptr->ind.status;
		Swap2Byte(&ind_data);
		*chanFail= ind_data.chan_fail;
		return 0;

	case	ANA_TYPE:
	case	OUTA_TYPE:
	case	PARAM_TYPE:
		anlg_data= common_ptr.var_ptr->anlg.value.status;
		Swap2Byte(&anlg_data);
		*chanFail= anlg_data.chan_fail;
		return 0;

	case	IMP_TYPE:
		imp_data= common_ptr.var_ptr->imp.entry.status;
		Swap2Byte(&imp_data);
		*chanFail= imp_data.chan_fail;
		return 0;

	default:
		return -1;
	}
}



int	IsPtValueChanFailByNameStr( char *ptNameStr, BOOL *chanFail)
{
	DMS_COMMON_PTR common_ptr;
	IND_DATA	ind_data;
	ANLG_STATUS	anlg_data;
	IMP_STATUS	imp_data;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	switch	( common_ptr.point.data_type )
	{
	case	SOE_TYPE:
	case	POL_TYPE:
	case	OUTS_TYPE:
	case	SYSMSG_TYPE:
		ind_data= common_ptr.var_ptr->ind.status;
		Swap2Byte(&ind_data);
		*chanFail= ind_data.chan_fail;
		return 0;

	case	ANA_TYPE:
	case	OUTA_TYPE:
	case	PARAM_TYPE:
		anlg_data= common_ptr.var_ptr->anlg.value.status;
		Swap2Byte(&anlg_data);
		*chanFail= anlg_data.chan_fail;
		return 0;

	case	IMP_TYPE:
		imp_data= common_ptr.var_ptr->imp.entry.status;
		Swap2Byte(&imp_data);
		*chanFail= imp_data.chan_fail;
		return 0;

	default:
		return -1;
	}
}




/*Get point iost functions*/
int	GetPtViewPointById( POINTER *point, USHORT *viewPoint )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*viewPoint= iost.viewPoint;
	return 0;
}


int	GetPtViewPointByNameStr( char *ptNameStr, USHORT *viewPoint )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*viewPoint= iost.viewPoint;
	return 0;
}


int	IsPtScanEnableById( POINTER *point, USHORT *scanEnable )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*scanEnable= iost.scanEnable;
	return 0;
}


int	IsPtScanEnableByNameStr( char *ptNameStr, USHORT *scanEnable )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*scanEnable= iost.scanEnable;
	return 0;
}


int	IsPtAlmEnableById( POINTER *point, USHORT *almEnable )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*almEnable= iost.almEnable;
	return 0;
}


int	IsPtAlmEnableByNameStr( char *ptNameStr, USHORT *almEnable )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*almEnable= iost.almEnable;
	return 0;
}


int	IsPtAlmNotAckedById( POINTER *point, USHORT *almNoAck )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*almNoAck= iost.almNoAck;
	return 0;
}


int	IsPtAlmNotAckedByNameStr( char *ptNameStr, USHORT *almNoAck )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*almNoAck= iost.almNoAck;
	return 0;
}


int	IsPtCtrlLockedById( POINTER *point, USHORT *ctrlLock )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*ctrlLock= iost.ctrlLock;
	return 0;
}

int	IsPtCtrlLockedByNameStr( char *ptNameStr, USHORT *ctrlLock )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*ctrlLock= iost.ctrlLock;
	return 0;
}



int	IsPtNoUpdateById( POINTER *point, USHORT *noUpdate )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*noUpdate= iost.noUpdate;
	return 0;
}


int	IsPtNoUpdateByNameStr( char *ptNameStr, USHORT *noUpdate )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*noUpdate= iost.noUpdate;
	return 0;
}


int	IsPtAlmRepeatById( POINTER *point, USHORT *almRepeat )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*almRepeat= iost.almRepeat;
	return 0;
}


int	IsPtAlmRepeatByNameStr( char *ptNameStr, USHORT *almRepeat )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*almRepeat= iost.almRepeat;
	return 0;
}


int	GetPtTagStateById( POINTER *point, USHORT *tagState )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*tagState= iost.tagState;
	return 0;
}


int	GetPtTagStateByNameStr( char *ptNameStr, USHORT *tagState )
{
	DMS_COMMON_PTR common_ptr;
	IOST	iost;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	iost= common_ptr.fixed_ptr->iost;
	Swap2Byte(&iost);
	*tagState= iost.tagState;
	return 0;
}



/*Get point ioattr functions*/
int	GetPtProcTypeById( POINTER *point, USHORT *procType )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;
	char	err_str[128];

	CheckDbValid();
	if( (point->data_type != SOE_TYPE)
		&&(point->data_type != POL_TYPE)
		&&(point->data_type != OUTS_TYPE)
		&&(point->data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*procType= ioattr.procType;
	return 0;
}


int	GetPtProcTypeByNameStr( char *ptNameStr, USHORT *procType )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != SOE_TYPE)
		&&(common_ptr.point.data_type != POL_TYPE)
		&&(common_ptr.point.data_type != OUTS_TYPE)
		&&(common_ptr.point.data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}
	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*procType= ioattr.procType;
	return 0;
}



int	IsPtAlmCbaById( POINTER *point, BOOL *almCba )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*almCba= ioattr.almCba;
	return 0;	
}


int	IsPtAlmCbaByNameStr( char *ptNameStr, BOOL *almCba )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*almCba= ioattr.almCba;
	return 0;	
}


int	IsPtStatisticById( POINTER *point, BOOL *statistic )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*statistic= ioattr.statistic;
	return 0;
}

int	IsPtStatisticByNameStr( char *ptNameStr, BOOL *statistic )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*statistic= ioattr.statistic;
	return 0;
}



int	IsPtFaultById( POINTER *point, BOOL *fault )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*fault= ioattr.fault;
	return 0;
}


int	IsPtFaultByNameStr( char *ptNameStr, BOOL *fault )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*fault= ioattr.fault;
	return 0;
}


int	IsPtFailById( POINTER *point, BOOL *fail )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*fail= ioattr.fail;
	return 0;
}


int	IsPtFailByNameStrd( char *ptNameStr, BOOL *fail )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*fail= ioattr.fail;
	return 0;
}

int	IsPtCtrlById( POINTER *point, BOOL *ctrl )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*ctrl= ioattr.ctrl;
	return 0;
}

int	IsPtCtrlByNameStr( char *ptNameStr, BOOL *ctrl )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*ctrl= ioattr.ctrl;
	return 0;
}


int	IsPtSoeById( POINTER *point, BOOL *soe )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*soe= ioattr.soe;
	return 0;
}

int	IsPtSoeByNameStr( char *ptNameStr, BOOL *soe )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*soe= ioattr.soe;
	return 0;
}


int	IsPtTrendAlmById( POINTER *point, BOOL *trendAlm )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*trendAlm= ioattr.trendAlm;
	return 0;
}


int	IsPtTrendAlmByNameStr( char *ptNameStr, BOOL *trendAlm )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*trendAlm= ioattr.trendAlm;
	return 0;
}


int	IsPtKeySignalById( POINTER *point, BOOL *keySignal )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*keySignal= ioattr.keySignal;
	return 0;
}


int	IsPtKeySignalByNameStr( char *ptNameStr, BOOL *keySignal )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*keySignal= ioattr.keySignal;
	return 0;
}


int	IsPtFaultRecById( POINTER *point, BOOL *faultRec )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*faultRec= ioattr.faultRec;
	return 0;
}


int	IsPtFaultRecByNameStr( char *ptNameStr, BOOL *faultRec )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*faultRec= ioattr.faultRec;
	return 0;
}


int	IsPtAudAlmById( POINTER *point, BOOL *audAlm )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*audAlm= ioattr.audAlm;
	return 0;
}


int	IsPtAudAlmByNameStr( char *ptNameStr, BOOL *audAlm )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*audAlm= ioattr.audAlm;
	return 0;
}


int	IsPtNormalCloseById( POINTER *point, BOOL *normalClose )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*normalClose= ioattr.normalClose;
	return 0;
}


int	IsPtNormalCloseByNameStr( char *ptNameStr, BOOL *normalClose )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*normalClose= ioattr.normalClose;
	return 0;
}


int	IsPtDoublePointById( POINTER *point, BOOL *doublePt )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*doublePt= ioattr.doublePt;
	return 0;
}


int	IsPtDoublePointByNameStr( char *ptNameStr, BOOL *doublePt )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*doublePt= ioattr.doublePt;
	return 0;
}


int	IsPtSysVirtualById( POINTER *point, BOOL *sysVirtual )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*sysVirtual= ioattr.sysVirtual;
	return 0;
}


int	IsPtSysVirtualByNameStr( char *ptNameStr, BOOL *sysVirtual )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*sysVirtual= ioattr.sysVirtual;
	return 0;
}


int	IsPtOnAlmById( POINTER *point, BOOL *onAlm )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*onAlm= ioattr.onAlm;
	return 0;
}


int	IsPtOnAlmByNameStr( char *ptNameStr, BOOL *onAlm )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*onAlm= ioattr.onAlm;
	return 0;
}


int	IsPtOffAlmById( POINTER *point, BOOL *offAlm )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*offAlm= ioattr.offAlm;
	return 0;
}


int	IsPtOffAlmByNameStr( char *ptNameStr, BOOL *offAlm )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*offAlm= ioattr.offAlm;
	return 0;
}


int	GetPtNormalStateById( POINTER *point, GULONG *normalState )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*normalState= ioattr.normalState;
	return 0;
}


int	GetPtNormalStateByNameStr( char *ptNameStr, GULONG *normalState )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*normalState= ioattr.normalState;
	return 0;
}


int	IsPtLcuVirtualById( POINTER *point, BOOL *lcuVirtual )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*lcuVirtual= ioattr.lcuVirtual;
	return 0;
}


int	IsPtLcuVirtualByNameStr( char *ptNameStr, BOOL *lcuVirtual )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*lcuVirtual= ioattr.lcuVirtual;
	return 0;
}


int	IsPtCascadeById( POINTER *point, BOOL *cascade )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*cascade= ioattr.cascade;
	return 0;
}


int	IsPtCascadeByNameStr( char *ptNameStr, BOOL *cascade )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*cascade= ioattr.cascade;
	return 0;
}



int	IsPtHdbRecordById( POINTER *point, BOOL *hdbRec )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*hdbRec= ioattr.hdbRec;
	return 0;
}


int	IsPtHdbRecordByNameStr( char *ptNameStr, BOOL *hdbRec )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*hdbRec= ioattr.hdbRec;
	return 0;
}



int	IsPtHdbCurveById( POINTER *point, BOOL *hdbCurve )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*hdbCurve= ioattr.hdbCurve;
	return 0;
}


int	IsPtHdbCurveByNameStr( char *ptNameStr, BOOL *hdbCurve )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*hdbCurve= ioattr.hdbCurve;
	return 0;
}

int	GetPtHdbPeriodById( POINTER *point, GULONG *hdbPeriod )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*hdbPeriod= ioattr.hdbPeriod;
	return 0;
}


int	GetPtHdbPeriodByNameStr( char *ptNameStr, GULONG *hdbPeriod )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	*hdbPeriod= ioattr.hdbPeriod;
	return 0;
}




/*Get analog or calculate point functions*/
int	GetAnaPtValueById( POINTER *point, ANLG_DATA *value )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( (point->data_type != ANA_TYPE)
		&&(point->data_type != OUTA_TYPE)
		&&(point->data_type != CALC_TYPE)
		&&(point->data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*value= common_ptr.var_ptr->anlg.value;
	Swap4Byte(&value->data);
	Swap2Byte(&value->status);
	return 0;
}


int	GetAnaPtValueByNameStr( char *ptNameStr, ANLG_DATA *value )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != ANA_TYPE)
		&&(common_ptr.point.data_type != OUTA_TYPE)
		&&(common_ptr.point.data_type != CALC_TYPE)
		&&(common_ptr.point.data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*value= common_ptr.var_ptr->anlg.value;
	Swap4Byte(&value->data);
	Swap2Byte(&value->status);
	return 0;
}



int	GetAnaPtfValueById( POINTER *point, FLOAT *fValue )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( (point->data_type != ANA_TYPE)
		&&(point->data_type != OUTA_TYPE)
		&&(point->data_type != CALC_TYPE)
		&&(point->data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*fValue= common_ptr.var_ptr->anlg.fValue;
	Swap4Byte(fValue);
	return 0;
}


int	GetAnaPtfValueByNameStr( char *ptNameStr, FLOAT *fValue )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != ANA_TYPE)
		&&(common_ptr.point.data_type != OUTA_TYPE)
		&&(common_ptr.point.data_type != CALC_TYPE)
		&&(common_ptr.point.data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*fValue= common_ptr.var_ptr->anlg.fValue;
	Swap4Byte(fValue);
	return 0;
}



int	GetAnaPtLmtStateById( POINTER *point, USHORT *lmtState )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	ANLG_STATUS	status;

	CheckDbValid();
	if( (point->data_type != ANA_TYPE)
		&&(point->data_type != OUTA_TYPE)
		&&(point->data_type != CALC_TYPE)
		&&(point->data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	status= common_ptr.var_ptr->anlg.value.status;
	Swap2Byte(&status);
	*lmtState= status.lmt_state;
	return 0;
}


int	GetAnaPtLmtStateByNameStr( char *ptNameStr, USHORT *lmtState )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	ANLG_STATUS	status;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != ANA_TYPE)
		&&(common_ptr.point.data_type != OUTA_TYPE)
		&&(common_ptr.point.data_type != CALC_TYPE)
		&&(common_ptr.point.data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	status= common_ptr.var_ptr->anlg.value.status;
	Swap2Byte(&status);
	*lmtState= status.lmt_state;
	return 0;
}



int	GetAnaPtTrendStateById( POINTER *point, USHORT *trendState )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	ANLG_STATUS	status;

	CheckDbValid();
	if( (point->data_type != ANA_TYPE)
		&&(point->data_type != OUTA_TYPE)
		&&(point->data_type != CALC_TYPE)
		&&(point->data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	status= common_ptr.var_ptr->anlg.value.status;
	Swap2Byte(&status);
	*trendState= status.trend_state;
	return 0;
}


int	GetAnaPtTrendStateByNameStr( char *ptNameStr, USHORT *trendState )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	ANLG_STATUS	status;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != ANA_TYPE)
		&&(common_ptr.point.data_type != OUTA_TYPE)
		&&(common_ptr.point.data_type != CALC_TYPE)
		&&(common_ptr.point.data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	status= common_ptr.var_ptr->anlg.value.status;
	Swap2Byte(&status);
	*trendState= status.trend_state;
	return 0;
}


int	GetAnaPtCtrlblockStateById( POINTER *point, USHORT *ctrlBlock )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	ANLG_STATUS	status;

	CheckDbValid();
	if( (point->data_type != ANA_TYPE)
		&&(point->data_type != OUTA_TYPE)
		&&(point->data_type != CALC_TYPE)
		&&(point->data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	status= common_ptr.var_ptr->anlg.value.status;
	Swap2Byte(&status);
	*ctrlBlock= status.ctrl_block;
	return 0;
}


int	GetAnaPtCtrlBlockStateByNameStr( char *ptNameStr, USHORT *ctrlBlock )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	ANLG_STATUS	status;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != ANA_TYPE)
		&&(common_ptr.point.data_type != OUTA_TYPE)
		&&(common_ptr.point.data_type != CALC_TYPE)
		&&(common_ptr.point.data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	status= common_ptr.var_ptr->anlg.value.status;
	Swap2Byte(&status);
	*ctrlBlock= status.ctrl_block;
	return 0;
}


int	GetAnaPtOverflowStateById( POINTER *point, USHORT *overflow )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	ANLG_STATUS	status;

	CheckDbValid();
	if( (point->data_type != ANA_TYPE)
		&&(point->data_type != OUTA_TYPE)
		&&(point->data_type != CALC_TYPE)
		&&(point->data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	status= common_ptr.var_ptr->anlg.value.status;
	Swap2Byte(&status);
	*overflow= status.overflow;
	return 0;
}


int	GetAnaPtOverflowStateByNameStr( char *ptNameStr, USHORT *overflow )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	ANLG_STATUS	status;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != ANA_TYPE)
		&&(common_ptr.point.data_type != OUTA_TYPE)
		&&(common_ptr.point.data_type != CALC_TYPE)
		&&(common_ptr.point.data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	status= common_ptr.var_ptr->anlg.value.status;
	Swap2Byte(&status);
	*overflow= status.overflow;
	return 0;
}


int	GetAnaPtUnitNameById( POINTER *point, char *unitName )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( (point->data_type != ANA_TYPE)
		&&(point->data_type != OUTA_TYPE)
		&&(point->data_type != CALC_TYPE)
		&&(point->data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	strcpy(unitName, common_ptr.var_ptr->anlg.unitName);
	return 0;
}


int	GetAnaPtUnitNameByNameStr( char *ptNameStr, char *unitName )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != ANA_TYPE)
		&&(common_ptr.point.data_type != OUTA_TYPE)
		&&(common_ptr.point.data_type != CALC_TYPE)
		&&(common_ptr.point.data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	strcpy(unitName, common_ptr.var_ptr->anlg.unitName);
	return 0;
}



int	GetAnaPtEngLmtParaById( POINTER *point, ANA_ENG_PARA *eng_para, LMT_PARA *lmt_para )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( (point->data_type != ANA_TYPE)
		&&(point->data_type != OUTA_TYPE)
		&&(point->data_type != CALC_TYPE)
		&&(point->data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	if( eng_para != NULL )
	{
		*eng_para= common_ptr.var_ptr->anlg.eng_para;
		SwapAnaEngParaByte(eng_para);
	}
	if( lmt_para != NULL )
	{
		*lmt_para= common_ptr.var_ptr->anlg.lmt_para;
		SwapLmtParaByte(lmt_para);
	}
	return 0;
}


int	GetAnaPtEngLmtParaByNameStr( char *ptNameStr, ANA_ENG_PARA *eng_para, LMT_PARA *lmt_para )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != ANA_TYPE)
		&&(common_ptr.point.data_type != OUTA_TYPE)
		&&(common_ptr.point.data_type != CALC_TYPE)
		&&(common_ptr.point.data_type != PARAM_TYPE))
	{
		sprintf(err_str, "Not ana/outa/calc/param type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}
	
	if( eng_para != NULL )
	{
		*eng_para= common_ptr.var_ptr->anlg.eng_para;
		SwapAnaEngParaByte(eng_para);
	}
	if( lmt_para != NULL )
	{
		*lmt_para= common_ptr.var_ptr->anlg.lmt_para;
		SwapLmtParaByte(lmt_para);
	}
	return 0;
}




/*Get impulse point functions*/
int	GetImpPtRtValueById( POINTER *point, GULONG *rt_value )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*rt_value= common_ptr.var_ptr->imp.entry.rt_value;
	Swap4Byte(rt_value);
	return 0;
}


int	GetImpPtRtValueByNameStr( char *ptNameStr, GULONG *rt_value )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*rt_value= common_ptr.var_ptr->imp.entry.rt_value;
	Swap4Byte(rt_value);
	return 0;
}


int	GetImpPtBaseValueById( POINTER *point, GLONG *base_value )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*base_value= common_ptr.var_ptr->imp.entry.base_value;
	Swap4Byte(base_value);
	return 0;
}


int	GetImpPtBaseValueByNameStr( char *ptNameStr, GLONG *base_value )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*base_value= common_ptr.var_ptr->imp.entry.base_value;
	Swap4Byte(base_value);
	return 0;
}


int	GetImpPtDaccumById( POINTER *point, FLOAT *d_accum )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*d_accum= common_ptr.var_ptr->imp.entry.d_accum;
	Swap4Byte(d_accum);
	return 0;
}


int	GetImpPtDaccumByNameStr( char *ptNameStr, FLOAT *d_accum )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*d_accum= common_ptr.var_ptr->imp.entry.d_accum;
	Swap4Byte(d_accum);
	return 0;
}



int	GetImpPtEngParaById( POINTER *point, IMP_ENG_PARA *eng_para )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*eng_para= common_ptr.var_ptr->imp.entry.eng_para;
	SwapImpEngParaByte(eng_para);
	return 0;
}


int	GetImpPtEngParaByNameStr( char *ptNameStr, IMP_ENG_PARA *eng_para )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*eng_para= common_ptr.var_ptr->imp.entry.eng_para;
	SwapImpEngParaByte(eng_para);
	return 0;
}



int	GetImpPtStatisticUnitNameById( POINTER *point, enum IMP_STATISTIC_TYPE type, char *unitName )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;
	GULONG	rate;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	if(type==RD_IMP_DAY)
		rate= common_ptr.var_ptr->imp.entry.eng_para.DAY_RATE;
	else if(type==RD_IMP_MONTH || type==RD_IMP_YEAR)
		rate= common_ptr.var_ptr->imp.entry.eng_para.YEAR_RATE;
	else
	{
		sprintf(err_str, "imp statistic type error: %d", type);
		Err_log(err_str);
		return -1;
	}
	Swap4Byte(&rate);

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);

	if(ioattr.procType==0) /*ACTIVE*/
	{
		if(rate==1)
		{
			strcpy(unitName, "Wh");
			return 0;
		}
		else if(rate==1000)
		{
			strcpy(unitName, "kWh");
			return 0;
		}
		else if(rate==1000000)
		{
			strcpy(unitName, "MWh");
			return 0;
		}
		else if(rate==1000000000)
		{
			strcpy(unitName, "GWh");
			return 0;
		}
		else
		{
			sprintf(err_str, "imp rate error: %d", rate);
			Err_log(err_str);
			return -1;
		}
	}	
	else /*REACTIVE*/
	{
		if(rate==1)
		{
			strcpy(unitName, "Varh");
			return 0;
		}
		else if(rate==1000)
		{
			strcpy(unitName, "kVarh");
			return 0;
		}
		else if(rate==1000000)
		{
			strcpy(unitName, "MVarh");
			return 0;
		}
		else if(rate==1000000000)
		{
			strcpy(unitName, "GVarh");
			return 0;
		}
		else
		{
			sprintf(err_str, "imp rate error: %d", rate);
			Err_log(err_str);
			return -1;
		}
	}	
}


int	GetImpPtStatisticUnitNameByNameStr( char *ptNameStr, enum IMP_STATISTIC_TYPE type, char *unitName )
{
	DMS_COMMON_PTR common_ptr;
	IOATTR	ioattr;
	GULONG	rate;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}	

	if(type==RD_IMP_DAY)
		rate= common_ptr.var_ptr->imp.entry.eng_para.DAY_RATE;
	else if(type==RD_IMP_MONTH || type==RD_IMP_YEAR)
		rate= common_ptr.var_ptr->imp.entry.eng_para.YEAR_RATE;
	else
	{
		sprintf(err_str, "imp statistic type error: %d", type);
		Err_log(err_str);
		return -1;
	}
	Swap4Byte(&rate);

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);

	if(ioattr.procType==0) /*ACTIVE*/
	{
		if(rate==1)
		{
			strcpy(unitName, "Wh");
			return 0;
		}
		else if(rate==1000)
		{
			strcpy(unitName, "kWh");
			return 0;
		}
		else if(rate==1000000)
		{
			strcpy(unitName, "MWh");
			return 0;
		}
		else if(rate==1000000000)
		{
			strcpy(unitName, "GWh");
			return 0;
		}
		else
		{
			sprintf(err_str, "imp rate error: %d", rate);
			Err_log(err_str);
			return -1;
		}
	}	
	else /*REACTIVE*/
	{
		if(rate==1)
		{
			strcpy(unitName, "Varh");
			return 0;
		}
		else if(rate==1000)
		{
			strcpy(unitName, "kVarh");
			return 0;
		}
		else if(rate==1000000)
		{
			strcpy(unitName, "MVarh");
			return 0;
		}
		else if(rate==1000000000)
		{
			strcpy(unitName, "GVarh");
			return 0;
		}
		else
		{
			sprintf(err_str, "imp rate error: %d", rate);
			Err_log(err_str);
			return -1;
		}
	}	
}


int	GetImpPtDataById( POINTER *point, enum RD_IMP_DATA_OPT opt, IMP_DATA *imp_data )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	switch(opt)
	{
	case	RD_IMP_DATA_PPEAK:
		*imp_data= common_ptr.var_ptr->imp.entry.ppeak;
		break;

	case	RD_IMP_DATA_PEAK:
		*imp_data= common_ptr.var_ptr->imp.entry.peak;
		break;

	case	RD_IMP_DATA_VALLEY:
		*imp_data= common_ptr.var_ptr->imp.entry.valley;
		break;

	case	RD_IMP_DATA_NORMAL:
		*imp_data= common_ptr.var_ptr->imp.entry.normal;
		break;

	case	RD_IMP_DATA_SUM:
		*imp_data= common_ptr.var_ptr->imp.entry.sum;
		break;

	case	RD_IMP_DATA_INIT:
		*imp_data= common_ptr.var_ptr->imp.entry.init;
		break;

	case	RD_IMP_DATA_LAST:
		*imp_data= common_ptr.var_ptr->imp.entry.last;
		break;

	default:
		return -1;
	}
	SwapImpDataByte(imp_data);
	return 0;
}


int	GetImpPtDataByNameStr( char *ptNameStr, enum RD_IMP_DATA_OPT opt, IMP_DATA *imp_data )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}
	
	switch(opt)
	{
	case	RD_IMP_DATA_PPEAK:
		*imp_data= common_ptr.var_ptr->imp.entry.ppeak;
		break;

	case	RD_IMP_DATA_PEAK:
		*imp_data= common_ptr.var_ptr->imp.entry.peak;
		break;

	case	RD_IMP_DATA_VALLEY:
		*imp_data= common_ptr.var_ptr->imp.entry.valley;
		break;

	case	RD_IMP_DATA_NORMAL:
		*imp_data= common_ptr.var_ptr->imp.entry.normal;
		break;

	case	RD_IMP_DATA_SUM:
		*imp_data= common_ptr.var_ptr->imp.entry.sum;
		break;

	case	RD_IMP_DATA_INIT:
		*imp_data= common_ptr.var_ptr->imp.entry.init;
		break;

	case	RD_IMP_DATA_LAST:
		*imp_data= common_ptr.var_ptr->imp.entry.last;
		break;

	default:
		return -1;
	}
	SwapImpDataByte(imp_data);
	return 0;
}



int	GetImpPtFloatDataById( POINTER *point, enum RD_IMP_DATA_OPT opt, IMP_FLOAT_DATA *imp_fdata )
{
	DMS_COMMON_PTR common_ptr;
	char		err_str[128];
	IMP_DATA	imp_data;
	IMP_ENG_PARA	eng_para;
	float		imp_cvt;

	CheckDbValid();
	if( point->data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	switch(opt)
	{
	case	RD_IMP_DATA_PPEAK:
		imp_data= common_ptr.var_ptr->imp.entry.ppeak;
		break;

	case	RD_IMP_DATA_PEAK:
		imp_data= common_ptr.var_ptr->imp.entry.peak;
		break;

	case	RD_IMP_DATA_VALLEY:
		imp_data= common_ptr.var_ptr->imp.entry.valley;
		break;

	case	RD_IMP_DATA_NORMAL:
		imp_data= common_ptr.var_ptr->imp.entry.normal;
		break;

	case	RD_IMP_DATA_SUM:
		imp_data= common_ptr.var_ptr->imp.entry.sum;
		break;

	default:
		return -1;
	}
	SwapImpDataByte(&imp_data);

	eng_para= common_ptr.var_ptr->imp.entry.eng_para;
	SwapImpEngParaByte(&eng_para);
	imp_cvt= eng_para.CT*eng_para.PT*eng_para.ENG_CVT;

	imp_fdata->day= imp_data.day*imp_cvt/eng_para.DAY_RATE;
	imp_fdata->month= imp_data.month*imp_cvt/eng_para.YEAR_RATE;
	imp_fdata->year= imp_data.year*imp_cvt/eng_para.YEAR_RATE;
	return 0;
}


int	GetImpPtFloatDataByNameStr( char *ptNameStr, enum RD_IMP_DATA_OPT opt, IMP_FLOAT_DATA *imp_fdata )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	IMP_DATA	imp_data;
	IMP_ENG_PARA	eng_para;
	float		imp_cvt;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != IMP_TYPE )
	{
		sprintf(err_str, "Not imp type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}
	
	switch(opt)
	{
	case	RD_IMP_DATA_PPEAK:
		imp_data= common_ptr.var_ptr->imp.entry.ppeak;
		break;

	case	RD_IMP_DATA_PEAK:
		imp_data= common_ptr.var_ptr->imp.entry.peak;
		break;

	case	RD_IMP_DATA_VALLEY:
		imp_data= common_ptr.var_ptr->imp.entry.valley;
		break;

	case	RD_IMP_DATA_NORMAL:
		imp_data= common_ptr.var_ptr->imp.entry.normal;
		break;

	case	RD_IMP_DATA_SUM:
		imp_data= common_ptr.var_ptr->imp.entry.sum;
		break;

	default:
		return -1;
	}
	SwapImpDataByte(&imp_data);

	eng_para= common_ptr.var_ptr->imp.entry.eng_para;
	SwapImpEngParaByte(&eng_para);
	imp_cvt= eng_para.CT*eng_para.PT*eng_para.ENG_CVT;

	imp_fdata->day= imp_data.day*imp_cvt/eng_para.DAY_RATE;
	imp_fdata->month= imp_data.month*imp_cvt/eng_para.YEAR_RATE;
	imp_fdata->year= imp_data.year*imp_cvt/eng_para.YEAR_RATE;
	return 0;
}



/*Get indication point functions*/
int GetIndPtStateById( POINTER *point, USHORT *state )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	IND_DATA	ind_data;

	CheckDbValid();
	if( (point->data_type != SOE_TYPE)
		&&(point->data_type != POL_TYPE)
		&&(point->data_type != OUTS_TYPE)
		&&(point->data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ind_data= common_ptr.var_ptr->ind.status;
	Swap2Byte(&ind_data);
	*state= ind_data.state;
	return 0;
}

int GetIndPtStateByNameStr( char *ptNameStr, USHORT *state )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	IND_DATA	ind_data;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != SOE_TYPE)
		&&(common_ptr.point.data_type != POL_TYPE)
		&&(common_ptr.point.data_type != OUTS_TYPE)
		&&(common_ptr.point.data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	ind_data= common_ptr.var_ptr->ind.status;
	Swap2Byte(&ind_data);
	*state= ind_data.state;
	return 0;
}

int GetIndPtRtStateById( POINTER *point, USHORT *rt_state )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	IND_DATA	ind_data;

	CheckDbValid();
	if( (point->data_type != SOE_TYPE)
		&&(point->data_type != POL_TYPE)
		&&(point->data_type != OUTS_TYPE)
		&&(point->data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ind_data= common_ptr.var_ptr->ind.status;
	Swap2Byte(&ind_data);
	*rt_state= ind_data.rt_state;
	return 0;
}

int GetIndPtRtStateByNameStr( char *ptNameStr, USHORT *rt_state )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	IND_DATA	ind_data;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != SOE_TYPE)
		&&(common_ptr.point.data_type != POL_TYPE)
		&&(common_ptr.point.data_type != OUTS_TYPE)
		&&(common_ptr.point.data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	ind_data= common_ptr.var_ptr->ind.status;
	Swap2Byte(&ind_data);
	*rt_state= ind_data.rt_state;
	return 0;
}

int GetIndPtChangeTimeById( POINTER *point, DATE_TIME *change_time )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( (point->data_type != SOE_TYPE)
		&&(point->data_type != POL_TYPE)
		&&(point->data_type != OUTS_TYPE)
		&&(point->data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*change_time= common_ptr.var_ptr->ind.change_time;
	SwapDateTimeByte(change_time);
	return 0;
}

int GetIndPtChangeTimeByNameStr( char *ptNameStr, DATE_TIME *change_time )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != SOE_TYPE)
		&&(common_ptr.point.data_type != POL_TYPE)
		&&(common_ptr.point.data_type != OUTS_TYPE)
		&&(common_ptr.point.data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*change_time= common_ptr.var_ptr->ind.change_time;
	SwapDateTimeByte(change_time);
	return 0;
}


int IsIndPtFaultTagById( POINTER *point, BOOL *fault_tag )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	IND_DATA	ind_data;

	CheckDbValid();
	if( (point->data_type != SOE_TYPE)
		&&(point->data_type != POL_TYPE)
		&&(point->data_type != OUTS_TYPE)
		&&(point->data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ind_data= common_ptr.var_ptr->ind.status;
	Swap2Byte(&ind_data);
	*fault_tag= ind_data.fault_tag;
	return 0;
}


int IsIndPtFaultTagByNameStr( char *ptNameStr, BOOL *fault_tag )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];
	IND_DATA	ind_data;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != SOE_TYPE)
		&&(common_ptr.point.data_type != POL_TYPE)
		&&(common_ptr.point.data_type != OUTS_TYPE)
		&&(common_ptr.point.data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	ind_data= common_ptr.var_ptr->ind.status;
	Swap2Byte(&ind_data);
	*fault_tag= ind_data.fault_tag;
	return 0;
}



int GetIndPtSTCById( POINTER *point, STC_DATA *stc_data )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( (point->data_type != SOE_TYPE)
		&&(point->data_type != POL_TYPE)
		&&(point->data_type != OUTS_TYPE)
		&&(point->data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*stc_data= common_ptr.var_ptr->ind.stc;
	SwapStcDataByte(stc_data);
	return 0;
}


int GetIndPtSTCByNameStr( char *ptNameStr, STC_DATA *stc_data )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != SOE_TYPE)
		&&(common_ptr.point.data_type != POL_TYPE)
		&&(common_ptr.point.data_type != OUTS_TYPE)
		&&(common_ptr.point.data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*stc_data= common_ptr.var_ptr->ind.stc;
	SwapStcDataByte(stc_data);
	return 0;
}





/*Get temp character point functions*/
int GetCharaPtDataById( POINTER *point, CHARA_DATA *chara_data )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != CHARA_TYPE )
	{
		sprintf(err_str, "Not chara type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*chara_data= common_ptr.var_ptr->chara.chara_data;
	SwapCharaDataByte(chara_data);
	return 0;
}

int GetCharaPtDataByNameStr( char *ptNameStr, CHARA_DATA *chara_data )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != CHARA_TYPE )
	{
		sprintf(err_str, "Not chara type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*chara_data= common_ptr.var_ptr->chara.chara_data;
	SwapCharaDataByte(chara_data);
	return 0;
}


int GetCharaStartPtNameById( POINTER *point, char *pt_name )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != CHARA_TYPE )
	{
		sprintf(err_str, "Not chara type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	strcpy(pt_name, common_ptr.var_ptr->chara.start_pt_name);
	return 0;
}


int GetCharaStartPtNameByNameStr( char *ptNameStr, char *pt_name )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != CHARA_TYPE )
	{
		sprintf(err_str, "Not chara type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	strcpy(pt_name, common_ptr.var_ptr->chara.start_pt_name);
	return 0;
}



int GetCharaPtNumById( POINTER *point, UCHAR *pt_num )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != CHARA_TYPE )
	{
		sprintf(err_str, "Not chara type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	*pt_num= common_ptr.var_ptr->chara.pt_num;
	return 0;
}


int GetCharaPtNumByNameStr( char *ptNameStr, UCHAR *pt_num )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != CHARA_TYPE )
	{
		sprintf(err_str, "Not chara type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*pt_num= common_ptr.var_ptr->chara.pt_num;
	return 0;
}



int GetIndPtStateStrById( POINTER *point, CHAR *str )
{
	DMS_COMMON_PTR common_ptr;
	char		err_str[128];
	IOATTR		ioattr;
	GULONG		procType;
	UCHAR		alm_sel;
	IND_DATA	ind_data;
	USHORT		state;

	CheckDbValid();
	if( (point->data_type != SOE_TYPE)
		&&(point->data_type != POL_TYPE)
		&&(point->data_type != OUTS_TYPE)
		&&(point->data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	procType= ioattr.procType;
	alm_sel= common_ptr.fixed_ptr->msg_sel.alm;
	ind_data= common_ptr.var_ptr->ind.status;
	Swap2Byte(&ind_data);
	state= ind_data.state;
	
	strcpy(str, db_ptr->msg_def[procType].group[alm_sel].def[state]);
	return 0;
}


int GetIndPtStateStrByNameStr( char *ptNameStr, CHAR *str )
{
	DMS_COMMON_PTR common_ptr;
	char		err_str[128];
	IOATTR		ioattr;
	GULONG		procType;
	UCHAR		alm_sel;
	IND_DATA	ind_data;
	USHORT		state;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != SOE_TYPE)
		&&(common_ptr.point.data_type != POL_TYPE)
		&&(common_ptr.point.data_type != OUTS_TYPE)
		&&(common_ptr.point.data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	procType= ioattr.procType;
	alm_sel= common_ptr.fixed_ptr->msg_sel.alm;
	ind_data= common_ptr.var_ptr->ind.status;
	Swap2Byte(&ind_data);
	state= ind_data.state;
	
	strcpy(str, db_ptr->msg_def[procType].group[alm_sel].def[state]);
	return 0;
}


int GetIndPtRtStateStrById( POINTER *point, CHAR *str )
{
	DMS_COMMON_PTR common_ptr;
	char		err_str[128];
	IOATTR		ioattr;
	GULONG		procType;
	UCHAR		alm_sel;
	IND_DATA	ind_data;
	USHORT		state;

	CheckDbValid();
	if( (point->data_type != SOE_TYPE)
		&&(point->data_type != POL_TYPE)
		&&(point->data_type != OUTS_TYPE)
		&&(point->data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	procType= ioattr.procType;
	alm_sel= common_ptr.fixed_ptr->msg_sel.alm;
	ind_data= common_ptr.var_ptr->ind.status;
	Swap2Byte(&ind_data);
	state= ind_data.rt_state;
	
	strcpy(str, db_ptr->msg_def[procType].group[alm_sel].def[state]);
	return 0;
}


int GetIndPtRtStateStrByNameStr( char *ptNameStr, CHAR *str )
{
	DMS_COMMON_PTR common_ptr;
	char		err_str[128];
	IOATTR		ioattr;
	GULONG		procType;
	UCHAR		alm_sel;
	IND_DATA	ind_data;
	USHORT		state;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != SOE_TYPE)
		&&(common_ptr.point.data_type != POL_TYPE)
		&&(common_ptr.point.data_type != OUTS_TYPE)
		&&(common_ptr.point.data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	procType= ioattr.procType;
	alm_sel= common_ptr.fixed_ptr->msg_sel.alm;
	ind_data= common_ptr.var_ptr->ind.status;
	Swap2Byte(&ind_data);
	state= ind_data.rt_state;
	
	strcpy(str, db_ptr->msg_def[procType].group[alm_sel].def[state]);
	return 0;
}


int GetIndPtAlmMsgStrById( POINTER *point, USHORT state, CHAR *str )
{
	DMS_COMMON_PTR common_ptr;
	char		err_str[128];
	IOATTR		ioattr;
	GULONG		procType;
	UCHAR		alm_sel;

	CheckDbValid();
	if( state>15 )
	{
		sprintf(err_str, "state overrange: %d", state);
		Err_log(err_str);
		return -1;
	}
	
	if( (point->data_type != SOE_TYPE)
		&&(point->data_type != POL_TYPE)
		&&(point->data_type != OUTS_TYPE)
		&&(point->data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	procType= ioattr.procType;
	alm_sel= common_ptr.fixed_ptr->msg_sel.alm;
	
	strcpy(str, db_ptr->msg_def[procType].group[alm_sel].def[state]);
	return 0;
}


int GetIndPtAlmMsgByNameStr( char *ptNameStr, USHORT state, CHAR *str )
{
	DMS_COMMON_PTR common_ptr;
	char		err_str[128];
	IOATTR		ioattr;
	GULONG		procType;
	UCHAR		alm_sel;

	CheckDbValid();
	if( state>15 )
	{
		sprintf(err_str, "state overrange: %d", state);
		Err_log(err_str);
		return -1;
	}
	
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( (common_ptr.point.data_type != SOE_TYPE)
		&&(common_ptr.point.data_type != POL_TYPE)
		&&(common_ptr.point.data_type != OUTS_TYPE)
		&&(common_ptr.point.data_type != SYSMSG_TYPE))
	{
		sprintf(err_str, "Not soe/pol/outs/sysmsg type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	ioattr= common_ptr.fixed_ptr->ioattr;
	Swap4Byte(&ioattr);
	procType= ioattr.procType;
	alm_sel= common_ptr.fixed_ptr->msg_sel.alm;
	
	strcpy(str, db_ptr->msg_def[procType].group[alm_sel].def[state]);
	return 0;
}


int GetPtCmdMsgStrById( POINTER *point, UCHAR cmdType, USHORT cmd_opt, CHAR *str )
{
	DMS_COMMON_PTR common_ptr;
	char		err_str[128];
	UCHAR		cmd_sel;
	int			i;

	CheckDbValid();
	if(cmdType==C_DATA_POLLING||cmdType==C_LCU_PARAM)
		cmd_sel= 0;
	else
	{
		if( -1==GetCommonPtrById(point, &common_ptr) )
			return -1;
		if(cmd_opt>=64)
		{
			sprintf(err_str, "Cmd opt is overrange(0~63): %d", cmd_opt);
			Err_log(err_str);
			return -1;
		}
		cmd_sel= common_ptr.fixed_ptr->msg_sel.cmd;
	}

	for(i=0; i<MAX_CMDTYPE_NUM; i++)
	{
		if(db_ptr->cmd_msg[i].cmdType==cmdType)
		{
			strcpy(str, db_ptr->cmd_msg[i].group[cmd_sel].def[cmd_opt]);
			return 0;
		}
	}
	sprintf(err_str, "Failed to find the cmdType msg: %d", cmdType);
	Err_log(err_str);
	return -1;
}


int GetPtCmdMsgStrByNameStr( char *ptNameStr, UCHAR cmdType, USHORT cmd_opt, CHAR *str )
{
	DMS_COMMON_PTR common_ptr;
	char		err_str[128];
	UCHAR		cmd_sel;
	int			i;

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if(cmd_opt>=64)
	{
		sprintf(err_str, "Cmd opt is overrange(0~63): %d", cmd_opt);
		Err_log(err_str);
		return -1;
	}

	cmd_sel= common_ptr.fixed_ptr->msg_sel.cmd;

	for(i=0; i<MAX_CMDTYPE_NUM; i++)
	{
		if(db_ptr->cmd_msg[i].cmdType==cmdType)
		{
			strcpy(str, db_ptr->cmd_msg[i].group[cmd_sel].def[cmd_opt]);
			return 0;
		}
	}
	sprintf(err_str, "Failed to find the cmdType msg: %d", cmdType);
	Err_log(err_str);
	return -1;
}


int	GetCurvePtDataById( POINTER *point, CURVE_DATA *curve )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != CURVE_TYPE )
	{
		sprintf(err_str, "Not curve type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	*curve= common_ptr.var_ptr->curve.data;
	SwapCurveDataByte(curve);
	return 0;
}


int	GetCurvePtDataByNameStr( char *ptNameStr, CURVE_DATA *curve )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != CURVE_TYPE )
	{
		sprintf(err_str, "Not curve type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*curve= common_ptr.var_ptr->curve.data;
	SwapCurveDataByte(curve);
	return 0;
}


int	GetCurvePtEngParaById( POINTER *point, ANA_ENG_PARA *eng_para )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != CURVE_TYPE )
	{
		sprintf(err_str, "Not curve type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	*eng_para= common_ptr.var_ptr->curve.eng_para;
	SwapAnaEngParaByte(eng_para);
	return 0;
}


int	GetCurvePtEngParaByNameStr( char *ptNameStr, ANA_ENG_PARA *eng_para )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != CURVE_TYPE )
	{
		sprintf(err_str, "Not curve type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*eng_para= common_ptr.var_ptr->curve.eng_para;
	SwapAnaEngParaByte(eng_para);
	return 0;
}



int	GetCurvePtSegNumById( POINTER *point, USHORT *segNum )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != CURVE_TYPE )
	{
		sprintf(err_str, "Not curve type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	*segNum= common_ptr.var_ptr->curve.seg_num;
	Swap2Byte(segNum);
	return 0;
}


int	GetCurvePtSegNumByNameStr( char *ptNameStr, USHORT *segNum )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != CURVE_TYPE )
	{
		sprintf(err_str, "Not curve type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*segNum= common_ptr.var_ptr->curve.seg_num;
	Swap2Byte(segNum);
	return 0;
}



int	GetObjPtStateById( POINTER *point, USHORT *state )
{
	DMS_COMMON_PTR common_ptr;
	FB_STATUS1	fb_status1;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	memcpy(&fb_status1, &common_ptr.var_ptr->devobj.fb_status1, sizeof(fb_status1));
	Swap2Byte(&fb_status1);
	if(fb_status1.p0==0 && fb_status1.p1==0)
		*state= 2;
	*state= fb_status1.p0+(fb_status1.p1<<1);

	if(*state==0)
		*state= 2;
	else if(*state==1)
		*state= 0;
	else if(*state==2)
		*state= 1;
	else if(*state==3)
		*state= 3;
	return 0;
}


int	GetObjPtStateByNameStr( char *ptNameStr, USHORT *state )
{
	DMS_COMMON_PTR common_ptr;
	FB_STATUS1	fb_status1;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}
	memcpy(&fb_status1, &common_ptr.var_ptr->devobj.fb_status1, sizeof(fb_status1));
	Swap2Byte(&fb_status1);
	if(fb_status1.p0==0 && fb_status1.p1==0)
		*state= 2;
	*state= fb_status1.p0+(fb_status1.p1<<1);

	if(*state==0)
		*state= 2;
	else if(*state==1)
		*state= 0;
	else if(*state==2)
		*state= 1;
	else if(*state==3)
		*state= 3;
	return 0;
}


int	GetObjPtCmdTimeById( POINTER *point, DATE_TIME *cmd_time )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	*cmd_time= common_ptr.var_ptr->devobj.cmd_time;
	SwapDateTimeByte(cmd_time);
	return 0;
}


int	GetObjPtCmdTimeByNameStr( char *ptNameStr, DATE_TIME *cmd_time )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*cmd_time= common_ptr.var_ptr->devobj.cmd_time;
	SwapDateTimeByte(cmd_time);
	return 0;
}



int	GetObjPtUsrNameById( POINTER *point, char *usr_name )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	strcpy(usr_name, common_ptr.var_ptr->devobj.usr_name);
	return 0;
}


int	GetObjPtUsrNameByNameStr( char *ptNameStr, char *usr_name )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	strcpy(usr_name, common_ptr.var_ptr->devobj.usr_name);
	return 0;
}


int	GetObjPtHostIdById( POINTER *point, UCHAR *host_id )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	*host_id= common_ptr.var_ptr->devobj.host_id;
	return 0;
}


int	GetObjPtHostIdByNameStr( char *ptNameStr, UCHAR *host_id )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*host_id= common_ptr.var_ptr->devobj.host_id;
	return 0;
}


int	GetObjPtCmdStatusById( POINTER *point, COMMAND_STATUS *cmd_status )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	memcpy(cmd_status, &common_ptr.var_ptr->devobj.status, sizeof(cmd_status));
	Swap2Byte(cmd_status);
	return 0;
}


int	GetObjPtCmdStatusByNameStr( char *ptNameStr, COMMAND_STATUS *cmd_status )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	memcpy(cmd_status, &common_ptr.var_ptr->devobj.status, sizeof(cmd_status));
	Swap2Byte(cmd_status);
	return 0;
}


int	GetObjPtCmdDataById( POINTER *point, COMMAND_STATUS *cmd_data )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	memcpy(cmd_data, &common_ptr.var_ptr->devobj.data, sizeof(cmd_data));
	Swap4Byte(cmd_data);
	return 0;
}


int	GetObjPtCmdDataByNameStr( char *ptNameStr, COMMAND_STATUS *cmd_data )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	memcpy(cmd_data, &common_ptr.var_ptr->devobj.data, sizeof(cmd_data));
	Swap4Byte(cmd_data);
	return 0;
}


int	GetObjPtFbStatus1ById( POINTER *point, FB_STATUS1 *fb_status1 )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	memcpy(fb_status1, &common_ptr.var_ptr->devobj.fb_status1, sizeof(fb_status1));
	Swap2Byte(fb_status1);
	return 0;
}


int	GetObjPtFbStatus1ByNameStr( char *ptNameStr, FB_STATUS1 *fb_status1 )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	memcpy(fb_status1, &common_ptr.var_ptr->devobj.fb_status1, sizeof(fb_status1));
	Swap2Byte(fb_status1);
	return 0;
}


int	GetObjPtFbStatus2ById( POINTER *point, FB_STATUS2 *fb_status2 )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	memcpy(fb_status2, &common_ptr.var_ptr->devobj.fb_status2, sizeof(fb_status2));
	Swap2Byte(fb_status2);
	return 0;
}


int	GetObjPtFbStatus2ByNameStr( char *ptNameStr, FB_STATUS2 *fb_status2 )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	memcpy(fb_status2, &common_ptr.var_ptr->devobj.fb_status2, sizeof(fb_status2));
	Swap2Byte(fb_status2);
	return 0;
}


int	GetObjPtCmdParamById( POINTER *point, USHORT *cmd_param )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( point->data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}

	if( -1==GetCommonPtrById(point, &common_ptr) )
		return -1;
	
	*cmd_param= common_ptr.var_ptr->devobj.param;
	Swap2Byte(cmd_param);
	return 0;
}


int	GetObjPtCmdParamByNameStr( char *ptNameStr, USHORT *cmd_param )
{
	DMS_COMMON_PTR common_ptr;
	char	err_str[128];

	CheckDbValid();
	if( -1==GetCommonPtrByNameStr(ptNameStr, &common_ptr) )
		return -1;

	if( common_ptr.point.data_type != DEVOBJ_TYPE )
	{
		sprintf(err_str, "Not devobj type: %d", common_ptr.point.data_type);
		Err_log(err_str);
		return -1;
	}

	*cmd_param= common_ptr.var_ptr->devobj.param;
	Swap2Byte(cmd_param);
	return 0;
}








