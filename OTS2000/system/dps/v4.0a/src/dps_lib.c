
#include	<ctype.h>
#include	"../../../dps/v4.0a/inc/dps_functions_def.h" 
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"


static COLOR_PAIR	COLOR_GREEN	=	{GREEN,GREEN3};
static COLOR_PAIR	COLOR_RED	=	{RED1,RED3};	
static COLOR_PAIR	COLOR_YLW	=	{YLW1,YLW3};	
static COLOR_PAIR	COLOR_WHITE=	{WHITE,GRAY1};	


static COLOR_PAIR	COLOR_NORMAL;
static COLOR_PAIR	COLOR_ALM_LMT;	
static COLOR_PAIR	COLOR_WRN_LMT;	
static COLOR_PAIR	COLOR_TR_LMT;	
static COLOR_PAIR	COLOR_RECOVER_LMT;	
static COLOR_PAIR	COLOR_CHANNEL_FAIL;
static COLOR_PAIR	COLOR_MAN_FORCE;
static COLOR_PAIR	COLOR_FAIL_TAG;//guzhang
static COLOR_PAIR	COLOR_FAULT_TAG;//shigu
static COLOR_PAIR	COLOR_OVERFLOW;
static COLOR_PAIR	COLOR_SMS_ALARM;



#define	DEBUG

int GetColorPairFromNameStr(char *colorName, COLOR_PAIR *color_pair)
{
	char	err_str[256];
	
	if(!strcmp(colorName, "GREEN"))
	{
		*color_pair= COLOR_GREEN;
		return 0;
	}
	if(!strcmp(colorName, "RED"))
	{
		*color_pair= COLOR_RED;
		return 0;
	}
	if(!strcmp(colorName, "YELLOW"))
	{
		*color_pair= COLOR_YLW;
		return 0;
	}
	if(!strcmp(colorName, "WHITE"))
	{
		*color_pair= COLOR_WHITE;
		return 0;
	}
	
	sprintf(err_str, "unsupported color define: %s", colorName);
	Err_log(err_str);
	return -1;
}

void InitColorDefine(void)
{
	static int initColorFlag;
	char	colorName[256];

	if(initColorFlag)
		return;

	initColorFlag= 1;
	
	if(-1==GetEmsString("EVENT_COLOR_NORMAL", colorName) 
		|| -1==GetColorPairFromNameStr(colorName, &COLOR_NORMAL))
		COLOR_NORMAL= COLOR_GREEN;

	if(-1==GetEmsString("EVENT_COLOR_ALM_LMT", colorName) 
		|| -1==GetColorPairFromNameStr(colorName, &COLOR_ALM_LMT))
		COLOR_ALM_LMT= COLOR_RED;

	if(-1==GetEmsString("EVENT_COLOR_WARN_LMT", colorName) 
		|| -1==GetColorPairFromNameStr(colorName, &COLOR_WRN_LMT))
		COLOR_WRN_LMT= COLOR_YLW;
	
	if(-1==GetEmsString("EVENT_COLOR_TREND_LMT", colorName) 
		|| -1==GetColorPairFromNameStr(colorName, &COLOR_TR_LMT))
		COLOR_TR_LMT= COLOR_RED;
	
	if(-1==GetEmsString("EVENT_COLOR_RECOVER_LMT", colorName) 
		|| -1==GetColorPairFromNameStr(colorName, &COLOR_RECOVER_LMT))
		COLOR_RECOVER_LMT= COLOR_WHITE;
	
	if(-1==GetEmsString("EVENT_COLOR_CHANNEL_FAIL", colorName) 
		|| -1==GetColorPairFromNameStr(colorName, &COLOR_CHANNEL_FAIL))
		COLOR_CHANNEL_FAIL= COLOR_RED;
	
	if(-1==GetEmsString("EVENT_COLOR_MAN_FORCE", colorName) 
		|| -1==GetColorPairFromNameStr(colorName, &COLOR_MAN_FORCE))
		COLOR_MAN_FORCE= COLOR_RED;
	
	if(-1==GetEmsString("EVENT_COLOR_FAIL_TAG", colorName) 
		|| -1==GetColorPairFromNameStr(colorName, &COLOR_FAIL_TAG))
		COLOR_FAIL_TAG= COLOR_YLW;
	
	if(-1==GetEmsString("EVENT_COLOR_FAULT_TAG", colorName) 
		|| -1==GetColorPairFromNameStr(colorName, &COLOR_FAULT_TAG))
		COLOR_FAULT_TAG= COLOR_RED;
	
	if(-1==GetEmsString("EVENT_COLOR_SMS_ALARM", colorName) 
		|| -1==GetColorPairFromNameStr(colorName, &COLOR_SMS_ALARM))
		COLOR_SMS_ALARM= COLOR_RED;
	
	if(-1==GetEmsString("EVENT_COLOR_OVERFLOW", colorName) 
		|| -1==GetColorPairFromNameStr(colorName, &COLOR_OVERFLOW))
		COLOR_OVERFLOW= COLOR_RED;
}

int	GetPointFromRec(REC_ENTRY_UNION *rec, POINTER *pt)
{   
    switch(rec->ana_rec.type_id)
	{
	case	DPS_ANA_ALM:
	case	DPS_IMP_ALM:
		pt->stn_id =  rec->ana_rec.stn_id ;
		pt->dev_id =  rec->ana_rec.dev_id ;
		pt->data_type =  rec->ana_rec.data_type ;
		pt->pt_id =  rec->ana_rec.pt_id ;
		return 0; /* sucessfully!! */
		break;

	case	DPS_IND_ALM:
		pt->stn_id =  rec->ind_rec.stn_id ;
		pt->dev_id =  rec->ind_rec.dev_id ;
		pt->data_type =  rec->ind_rec.data_type ;
		pt->pt_id =  rec->ind_rec.pt_id ;
		return 0; /* sucessfully!! */
		break;

	case	DPS_ANA_PARAM_SET:
	case	DPS_IMP_PARAM_SET:
	case	DPS_C_ONOFF_SET:
		pt->stn_id =  rec->param_set_rec.stn_id ;
		pt->dev_id =  rec->param_set_rec.dev_id ;
		pt->data_type =  rec->param_set_rec.data_type ;
		pt->pt_id =  rec->param_set_rec.pt_id ;
		return 0; /* sucessfully!! */
		break;

	case	DPS_LMS:
	case	DPS_SMS:
	case	DPS_SM_ALM:
		return -1; /* Failed */
		break;

	default:
		pt->stn_id =  rec->cmd_rec.stn_id ;
		pt->dev_id =  rec->cmd_rec.dev_id ;
		pt->data_type =  rec->cmd_rec.data_type ;
		pt->pt_id =  rec->cmd_rec.pt_id ;
		return 0; /* sucessfully!! */
		break;
	}
}


void *_EX_CS_(char *name, USHORT state)
{
	char	str[256];

	sprintf(str, "%s%d", name, state);
	return _CS_(str);
}

int	GetCmdSrcStr(USHORT	src_id, char *cmd_src)
{
	if(src_id>7)
		return -1;

	strcpy(cmd_src, _EX_CS_("ALM_STR_CMD_SOURCE", src_id));
	return 0;
}


long GetTimefromRec(REC_ENTRY_UNION *rec)
{
	struct tm	time;

	time.tm_isdst= 0;
	switch(rec->ana_rec.type_id)
	{
	case	DPS_ANA_ALM:
	case	DPS_IMP_ALM:
		time.tm_sec= rec->ana_rec.second;
		time.tm_min= rec->ana_rec.minute;
		time.tm_hour= rec->ana_rec.hour;
		time.tm_mday= rec->ana_rec.day;
		time.tm_mon= rec->ana_rec.month-1;
		time.tm_year= rec->ana_rec.year-1900;
		break;

	case	DPS_IND_ALM:
		time.tm_sec= rec->ind_rec.second;
		time.tm_min= rec->ind_rec.minute;
		time.tm_hour= rec->ind_rec.hour;
		time.tm_mday= rec->ind_rec.day;
		time.tm_mon= rec->ind_rec.month-1;
		time.tm_year= rec->ind_rec.year-1900;
		break;

	case	DPS_ANA_PARAM_SET:
	case	DPS_IMP_PARAM_SET:
	case	DPS_C_ONOFF_SET:
		time.tm_sec= rec->param_set_rec.second;
		time.tm_min= rec->param_set_rec.minute;
		time.tm_hour= rec->param_set_rec.hour;
		time.tm_mday= rec->param_set_rec.day;
		time.tm_mon= rec->param_set_rec.month-1;
		time.tm_year= rec->param_set_rec.year-1900;
		break;

	case	DPS_LMS:
		time.tm_sec= rec->lms_rec.second;
		time.tm_min= rec->lms_rec.minute;
		time.tm_hour= rec->lms_rec.hour;
		time.tm_mday= rec->lms_rec.day;
		time.tm_mon= rec->lms_rec.month-1;
		time.tm_year= rec->lms_rec.year-1900;
		break;

	case	DPS_SMS:
		return rec->sms_rec.time;

	default:
		time.tm_sec= rec->cmd_rec.second;
		time.tm_min= rec->cmd_rec.minute;
		time.tm_hour= rec->cmd_rec.hour;
		time.tm_mday= rec->cmd_rec.day;
		time.tm_mon= rec->cmd_rec.month-1;
		time.tm_year= rec->cmd_rec.year-1900;
		break;
	}
	return mktime(&time);
}


void SwapRecEntryByte(REC_ENTRY_UNION *rec_entry)
{

	switch(rec_entry->ana_rec.type_id)
	{
	case	DPS_ANA_ALM:
	case	DPS_IMP_ALM:
		Swap4Byte(&rec_entry->ana_rec.rec_sn);
		Swap2Byte(&rec_entry->ana_rec.pt_id);
		Swap2Byte(&rec_entry->ana_rec.year);
		Swap2Byte(&rec_entry->ana_rec.status);
		Swap4Byte(&rec_entry->ana_rec.data);
		Swap2Byte(&rec_entry->ana_rec.iost);
		break;

	case	DPS_IND_ALM:
		Swap4Byte(&rec_entry->ind_rec.rec_sn);
		Swap2Byte(&rec_entry->ind_rec.pt_id);
		Swap2Byte(&rec_entry->ind_rec.year);
		Swap2Byte(&rec_entry->ind_rec.ind_status);
		Swap2Byte(&rec_entry->ind_rec.ms);
		Swap2Byte(&rec_entry->ind_rec.iost);
		break;

	case	DPS_ANA_PARAM_SET:
	case	DPS_IMP_PARAM_SET:
	case	DPS_C_ONOFF_SET:
		Swap4Byte(&rec_entry->param_set_rec.rec_sn);
		Swap2Byte(&rec_entry->param_set_rec.pt_id);
		Swap2Byte(&rec_entry->param_set_rec.year);
		Swap2Byte(&rec_entry->param_set_rec.cmd_status);
		Swap4Byte(&rec_entry->param_set_rec.data_new);
		Swap4Byte(&rec_entry->param_set_rec.data_old);
		break;

	case	DPS_LMS:
		Swap4Byte(&rec_entry->lms_rec.rec_sn);
		Swap2Byte(&rec_entry->lms_rec.msg_ptr);
		Swap2Byte(&rec_entry->lms_rec.year);
		break;

	case	DPS_SMS:
	case	DPS_SM_ALM:
		Swap4Byte(&rec_entry->sms_rec.rec_sn);
		Swap4Byte(&rec_entry->sms_rec.time);
		break;

	case	DPS_DEV_FB:
		Swap4Byte(&rec_entry->obj_rec.rec_sn);
		Swap2Byte(&rec_entry->obj_rec.pt_id);
		Swap2Byte(&rec_entry->obj_rec.fb_status1);
		Swap2Byte(&rec_entry->obj_rec.fb_status2);
		Swap2Byte(&rec_entry->obj_rec.param);
		Swap2Byte(&rec_entry->obj_rec.ms);
		Swap2Byte(&rec_entry->obj_rec.year);
		Swap2Byte(&rec_entry->obj_rec.iost);
		break;

	default:
		Swap4Byte(&rec_entry->cmd_rec.rec_sn);
		Swap2Byte(&rec_entry->cmd_rec.pt_id);
		Swap2Byte(&rec_entry->cmd_rec.year);
		Swap2Byte(&rec_entry->cmd_rec.cmd_status);
		Swap4Byte(&rec_entry->cmd_rec.data);
		Swap2Byte(&rec_entry->cmd_rec.ctrl_addr);
		break;
	}
}





int	GetPtLogicnameById(POINTER *point, char *logicname)
{
	POINT_NAME	pt_name;

	if(-1==GetPtNameById(point, NULL, &pt_name))
	{
		sprintf(logicname, "No such point");
		return -1;
	}
	else
	{
		sprintf(logicname, "%s.%s", pt_name.grpName, pt_name.entryName);
		return 0;
	}
}


int	GetAlmStrings(REC_ENTRY_UNION *rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color)
{
	int	ret;

	InitColorDefine();
	
	alm_str->hostname[0]= 0;
	alm_str->logicname[0]= 0;
	alm_str->longname[0]= 0;
	alm_str->message[0]	= 0;
	alm_str->sn_str[0]	= 0;
	alm_str->state_str[0]=	0;
	alm_str->time_str[0]= 0;
	alm_str->usrname[0]	= 0;
	alm_str->repeatAlm[0]= 0;
	alm_str->cmd_src[0]= 0;
    alm_str->cmd_result[0]= 0;
	if(color)
		*color= COLOR_NORMAL;
	if(blink)
		*blink= FALSE;
	
	switch(rec->ana_rec.type_id)
	{
	case	DPS_ANA_ALM:
		ret= GetAnaAlmStrings(&rec->ana_rec, alm_str, blink, color);
		break;

	case	DPS_IMP_ALM:
		ret= GetImpAlmStrings(&rec->ana_rec, alm_str, blink, color);
		break;

	case	DPS_IND_ALM:
		ret= GetIndAlmStrings(&rec->ind_rec, alm_str, blink, color);
		break;

	case	DPS_ANA_PARAM_SET:
	case	DPS_IMP_PARAM_SET:
	case	DPS_C_ONOFF_SET:
		ret= GetParamSetAlmStrings(&rec->param_set_rec, alm_str, blink, color);
		break;

	case	DPS_LMS:
		ret= GetLmsAlmStrings(&rec->lms_rec, alm_str, blink, color);
		break;

	case	DPS_SMS:
	case	DPS_SM_ALM:
		ret= GetSmsAlmStrings(&rec->sms_rec, alm_str, blink, color);
		break;

	default:
		ret= GetCmdRecStrings(&rec->cmd_rec, alm_str, blink, color);
		break;
	}

	return ret;
}



int	GetAnaAlmStrings(ANLG_REC *ana_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color)
{
	POINTER		point;
	char		err_str[128];
	USHORT		almRepeat= 0;
	USHORT		almNoAck= 0;

	sprintf(alm_str->sn_str, "%04d", ana_rec->rec_sn);

	sprintf(alm_str->time_str, "%04d-%02d-%02d %02d:%02d:%02d",
		ana_rec->year, ana_rec->month, ana_rec->day,
		ana_rec->hour, ana_rec->minute, ana_rec->second);

	if(-1==GetHostNameById(ana_rec->host_id, alm_str->hostname))
	{
		strcpy(alm_str->hostname, "No define");
		sprintf(err_str, "No host define: %d", ana_rec->host_id);
		Err_log( err_str);
	}

	GetPointFromRec((REC_ENTRY_UNION*)ana_rec, &point);
	if(-1==GetPtLongnameById(&point, alm_str->longname))
	{
		strcpy(alm_str->longname, "No such point");
		sprintf(err_str, "No point define: %d.%d.%d.%d", ana_rec->stn_id, 
			ana_rec->dev_id, ana_rec->data_type, ana_rec->pt_id);
		Err_log(err_str);
		return -1;
	}
	GetPtLogicnameById(&point, alm_str->logicname);

	if(ana_rec->iost.almRepeat)
		strcpy(alm_str->repeatAlm, "R");
	if(blink)
	{
		IsPtAlmNotAckedById(&point, &almNoAck);
		*blink= almNoAck;
	}

	switch(ana_rec->status.ana_status.alm_type)
	{
	case	ANA_ALM_TYPE_LMT:
		sprintf(alm_str->state_str, "%.3f", (ana_rec->data)/1000.);
		switch(ana_rec->status.ana_status.lmt_state)
		{
		case	1:
			strcat(alm_str->state_str, _CS_("ALM_STR_H_ALM_LMT"));
			if(color)
				*color= COLOR_ALM_LMT;
			break;
		case	2:
			strcat(alm_str->state_str, _CS_("ALM_STR_H_WRN_LMT"));
			if(color)
				*color= COLOR_WRN_LMT;
			break;
		case	3:
			strcat(alm_str->state_str, _CS_("ALM_STR_L_WRN_LMT"));
			if(color)
				*color= COLOR_WRN_LMT;
			break;
		case	4:
			strcat(alm_str->state_str, _CS_("ALM_STR_L_ALM_LMT"));
			if(color)
				*color= COLOR_ALM_LMT;
			break;
		case	5:
			strcat(alm_str->state_str, _CS_("ALM_STR_H_RECOVER"));
			if(color)
				*color= COLOR_RECOVER_LMT;
			break;
		case	6:
			strcat(alm_str->state_str, _CS_("ALM_STR_L_RECOVER"));
			if(color)
				*color= COLOR_RECOVER_LMT;
			break;
		case	7:
			strcat(alm_str->state_str, _CS_("ALM_STR_HH_RECOVER"));
			if(color)
				*color= COLOR_RECOVER_LMT;
			break;
		case	8:
			strcat(alm_str->state_str, _CS_("ALM_STR_LL_RECOVER"));
			if(color)
				*color= COLOR_RECOVER_LMT;
			break;
		}
		break;

	case	ANA_ALM_TYPE_TR:
		sprintf(alm_str->state_str, "%.3f", (ana_rec->data)/1000.);
		if(ana_rec->status.ana_status.trend_state==1)
		{
			strcat(alm_str->state_str, _CS_("ALM_STR_TR_WRN_LMT"));
			if(color)
				*color= COLOR_WRN_LMT;
		}
		else if(ana_rec->status.ana_status.trend_state==2)
		{
			strcat(alm_str->state_str, _CS_("ALM_STR_TR_RECOVER_LMT"));
			if(color)
				*color= COLOR_RECOVER_LMT;
		}
		break;

	case	ANA_ALM_TYPE_CHANFAIL:
		if(ana_rec->status.ana_status.chan_fail)
		{
			strcpy(alm_str->state_str, _CS_("ALM_STR_CHANNEL_FAIL"));
			if(color)
				*color= COLOR_CHANNEL_FAIL;
		}
		else
			strcpy(alm_str->state_str, _CS_("ALM_STR_CHANNEL_NORM"));
		break;

	case	ANA_ALM_TYPE_CTRLBLOCK:
		if(ana_rec->status.ana_status.ctrl_block)
		{
			strcpy(alm_str->state_str, _CS_("ALM_STR_CTRL_BLOCK"));
			if(color)
				*color= COLOR_MAN_FORCE;
		}
		else
			strcpy(alm_str->state_str, _CS_("ALM_STR_CTRL_BLOCK_RESET"));
		break;
	case	ANA_ALM_TYPE_OVERFLOW:
		if(ana_rec->status.ana_status.overflow)
		{
			strcpy(alm_str->state_str, _CS_("ALM_STR_OVERFLOW"));
			if(color)
				*color= COLOR_OVERFLOW;
		}
		else
			strcpy(alm_str->state_str, _CS_("ALM_STR_OVERFLOW_RESET"));
		break;
	}
	
	return 0;
}


int	GetImpAlmStrings(ANLG_REC *ana_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color)
{
	POINTER		point;
	char		err_str[128];
	USHORT		almRepeat= 0;
	USHORT		almNoAck= 0;

	sprintf(alm_str->sn_str, "%04d", ana_rec->rec_sn);

	sprintf(alm_str->time_str, "%04d-%02d-%02d %02d:%02d:%02d",
		ana_rec->year, ana_rec->month, ana_rec->day,
		ana_rec->hour, ana_rec->minute, ana_rec->second);

	if(-1==GetHostNameById(ana_rec->host_id, alm_str->hostname))
	{
		strcpy(alm_str->hostname, "No define");
		sprintf(err_str, "No host define: %d", ana_rec->host_id);
		Err_log( err_str);
	}

	GetPointFromRec((REC_ENTRY_UNION*)ana_rec, &point);
	if(-1==GetPtLongnameById(&point, alm_str->longname))
	{
		strcpy(alm_str->longname, "No such point");
		sprintf(err_str, "No point define: %d.%d.%d.%d", ana_rec->stn_id, 
			ana_rec->dev_id, ana_rec->data_type, ana_rec->pt_id);
		Err_log(err_str);
		return -1;
	}
	GetPtLogicnameById(&point, alm_str->logicname);

	if(ana_rec->iost.almRepeat)
		strcpy(alm_str->repeatAlm, "R");
	if(blink)
	{
		IsPtAlmNotAckedById(&point, &almNoAck);
		*blink= almNoAck;
	}

	switch(ana_rec->status.imp_status.alm_type)
	{
	case	IMP_ALM_TYPE_CHANFAIL:
		if(ana_rec->status.imp_status.chan_fail)
		{
			strcpy(alm_str->state_str, _CS_("ALM_STR_CHANNEL_FAIL"));
			if(color)
				*color= COLOR_CHANNEL_FAIL;
		}
		else
			strcpy(alm_str->state_str, _CS_("ALM_STR_CHANNEL_NORM"));
		break;

	case	IMP_ALM_TYPE_MANFORCE:
		if(ana_rec->status.imp_status.man_set)
		{
			strcpy(alm_str->state_str, _CS_("ALM_STR_MAN_FORCE"));
			if(color)
				*color= COLOR_MAN_FORCE;
		}
		else
			strcpy(alm_str->state_str, _CS_("ALM_STR_MAN_FORCE_RESET"));
		break;
	}
	return 0;
}


int	GetIndAlmStrings(IND_REC *ind_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color)
{
	POINTER		point;
	char		err_str[128];
	DMS_COMMON	common_all;
	USHORT		almRepeat= 0;
	USHORT		almNoAck= 0;

	sprintf(alm_str->sn_str, "%04d", ind_rec->rec_sn);
	sprintf(alm_str->time_str, "%04d-%02d-%02d %02d:%02d:%02d:%03d",
		ind_rec->year, ind_rec->month, ind_rec->day,
		ind_rec->hour, ind_rec->minute, ind_rec->second, ind_rec->ms);

	if(-1==GetHostNameById(ind_rec->host_id, alm_str->hostname))
	{
		strcpy(alm_str->hostname, "No define");
		sprintf(err_str, "No host define: %d", ind_rec->host_id);
		Err_log( err_str);
	}

	GetPointFromRec((REC_ENTRY_UNION*)ind_rec, &point);
	if(-1==ReadEntryById(&point, &common_all))
	{
		strcpy(alm_str->longname, "No such point");
		sprintf(err_str, "No point define: %d.%d.%d.%d", ind_rec->stn_id, 
			ind_rec->dev_id, ind_rec->data_type, ind_rec->pt_id);
		Err_log( err_str);
		return -1;
	}
	GetPtLongnameById(&point, alm_str->longname);
	GetPtLogicnameById(&point, alm_str->logicname);

	if(ind_rec->iost.almRepeat)
		strcpy(alm_str->repeatAlm, "R");
	if(blink)
		*blink= common_all.fixed.iost.almNoAck;

	switch(ind_rec->ind_status.alm_type)
	{
	case	IND_ALM_TYPE_CHANGE:
		GetIndPtAlmMsgStrById(&point, ind_rec->ind_status.rt_state, alm_str->state_str);
		if( ind_rec->ind_status.fault_tag )
		{
			if(color)
				*color= COLOR_FAULT_TAG;
		}
		else if(ind_rec->ind_status.rt_state!=common_all.fixed.ioattr.normalState )
		{
			if(color)
			{
				if(common_all.fixed.ioattr.fault)
					*color= COLOR_FAULT_TAG;
				else if(common_all.fixed.ioattr.fail)
					*color= COLOR_FAIL_TAG;
			}
		}
		else
		{
			if(color)
				*color= COLOR_RECOVER_LMT;
		}
		break;

	case	IND_ALM_TYPE_CHANFAIL:
		if(ind_rec->ind_status.chan_fail)
		{
			strcpy(alm_str->state_str, _CS_("ALM_STR_CHANNEL_FAIL"));
			if(color)
				*color= COLOR_CHANNEL_FAIL;
		}
		else
			strcpy(alm_str->state_str, _CS_("ALM_STR_CHANNEL_NORM"));
		break;

	case	IND_ALM_TYPE_MANFORCE:
		if(ind_rec->ind_status.man_set)
		{
			strcpy(alm_str->state_str, _CS_("ALM_STR_MAN_FORCE"));
			if(color)
				*color= COLOR_MAN_FORCE;
		}
		else
			strcpy(alm_str->state_str, _CS_("ALM_STR_MAN_FORCE_RESET"));
		break;
	}
	
	return 0;
}


int	GetParamSetAlmStrings(PARAM_SET_REC *param_set_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color)
{
	POINTER		point;
	char		err_str[128];
	USHORT		almRepeat= 0;
	USHORT		almNoAck= 0;

	sprintf(alm_str->sn_str, "%04d", param_set_rec->rec_sn);

	sprintf(alm_str->time_str, "%04d-%02d-%02d %02d:%02d:%02d",
		param_set_rec->year, param_set_rec->month, param_set_rec->day,
		param_set_rec->hour, param_set_rec->minute, param_set_rec->second);

	if(-1==GetHostNameById(param_set_rec->host_id, alm_str->hostname))
	{
		strcpy(alm_str->hostname, "No define");
		sprintf(err_str, "No host define: %d", param_set_rec->host_id);
		Err_log( err_str);
	}
	
	strncpy(alm_str->usrname, param_set_rec->usr_name, USR_NAME_SIZE);

	GetPointFromRec((REC_ENTRY_UNION*)param_set_rec, &point);
	if(-1==GetPtLongnameById(&point, alm_str->longname))
	{
		strcpy(alm_str->longname, "No such point");
		sprintf(err_str, "No point define: %d.%d.%d.%d", param_set_rec->stn_id, 
			param_set_rec->dev_id, param_set_rec->data_type, param_set_rec->pt_id);
		Err_log( err_str);
		return -1;
	}
//	strcat(alm_str->longname, " ");
//	strcat(alm_str->longname, _CS_("ALM_STR_PARAM_SET"));
//	GetPtLogicnameById(&point, alm_str->logicname);
	if(-1==GetPtCmdMsgStrById(&point, param_set_rec->type_id, param_set_rec->cmd_status.opr, alm_str->state_str))
	{
		strcpy(alm_str->state_str, "No define");
		sprintf(err_str, "No such cmd msg define: type %d, opr %d",  
			param_set_rec->type_id, param_set_rec->cmd_status.opr);
		Err_log( err_str);
		return -1;
	}

	switch(param_set_rec->type_id)
	{
	case	DPS_ANA_PARAM_SET:
		if(param_set_rec->cmd_status.opr>=16)
		{
			sprintf(err_str, "ANA_PARAM_SET opr overrange: %d", param_set_rec->cmd_status.opr);
			Err_log( err_str);
			break;
		}
		sprintf(alm_str->state_str, "%s %s%.3f %s%.3f", alm_str->state_str, 
			_CS_("ALM_STR_OLD_VALUE"), param_set_rec->data_old.fValue, 
			_CS_("ALM_STR_NEW_VALUE"), param_set_rec->data_new.fValue);
		break;

	case	DPS_IMP_PARAM_SET:
		if(param_set_rec->cmd_status.opr>=25)
		{
			sprintf(err_str, "IMP_PARAM_SET opr overrange: %d", param_set_rec->cmd_status.opr);
			Err_log( err_str);
			break;
		}
		if(param_set_rec->cmd_status.opr==IMP_PARAM_SET_IMP_ACCUM 
			|| param_set_rec->cmd_status.opr==IMP_PARAM_SET_BASE_ACCUM
			|| param_set_rec->cmd_status.opr==IMP_PARAM_SET_CONSTANT)
			sprintf(alm_str->state_str, "%s %s%d %s%d", alm_str->state_str, 
				_CS_("ALM_STR_OLD_VALUE"), param_set_rec->data_old.iValue, 
				_CS_("ALM_STR_NEW_VALUE"), param_set_rec->data_new.iValue);
		else
			sprintf(alm_str->state_str, "%s %s%.3f %s%.3f", alm_str->state_str, 
				_CS_("ALM_STR_OLD_VALUE"), param_set_rec->data_old.fValue, 
				_CS_("ALM_STR_NEW_VALUE"), param_set_rec->data_new.fValue);
		break;

	case	DPS_C_ONOFF_SET:
		if(param_set_rec->cmd_status.opr>=5)
		{
			sprintf(err_str, "IND_PARAM_SET opr overrange: %d", param_set_rec->cmd_status.opr);
			Err_log( err_str);
			break;
		}
		sprintf(alm_str->state_str, "%s %s%d %s%d", alm_str->state_str, 
			_CS_("ALM_STR_OLD_VALUE"), param_set_rec->data_old.iValue, 
			_CS_("ALM_STR_NEW_VALUE"), param_set_rec->data_new.iValue);
		break;
	}
	
	return 0;
}



int	GetSmsAlmStrings(SMS_REC *sms_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color)
{
	char		err_str[128];
	struct	tm	time;

	if(sms_rec->type_id== DPS_SM_ALM)
	{
		if(color)
			*color= COLOR_SMS_ALARM;
	}
	sprintf(alm_str->sn_str, "%04d", sms_rec->rec_sn);

	if(-1==GetHostNameById(sms_rec->host_id, alm_str->hostname))
	{
		strcpy(alm_str->hostname, "No define");
		sprintf(err_str, "No host define: %d", sms_rec->host_id);
		Err_log( err_str);
	}

	time= *localtime(&sms_rec->time);
	sprintf(alm_str->time_str, "%02d-%02d %02d:%02d:%02d",
		time.tm_mon+1, time.tm_mday,
		time.tm_hour, time.tm_min, time.tm_sec);

	strncpy(alm_str->message, sms_rec->message, SMS_MESSAGE_SIZE);
	alm_str->message[SMS_MESSAGE_SIZE]= 0;
	return 0;
}


int	GetLmsAlmStrings(LMS_REC *lms_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color)
{
	char		err_str[128];
	char		filename[256], buffer[256];
	FILE		*fp;
	int			id;

	if(color)
		*color= COLOR_SMS_ALARM;

	sprintf(alm_str->sn_str, "%04d", lms_rec->rec_sn);

	sprintf(alm_str->time_str, "%04d-%02d-%02d %02d:%02d:%02d",
		lms_rec->year, lms_rec->month, lms_rec->day,
		lms_rec->hour, lms_rec->minute, lms_rec->second);

	if(-1==GetHostNameById(lms_rec->host_id, alm_str->hostname))
	{
		strcpy(alm_str->hostname, "No define");
		sprintf(err_str, "No host define: %d", lms_rec->host_id);
		Err_log( err_str);
	}
	
	sprintf(filename, "%s%s", SYS_DEFINE_PATH, LMS_DEFINE_FILE);
	if((fp=fopen(filename, "r"))==NULL)
	{
		sprintf(err_str, "Fail to open %s", filename);
		Err_log(err_str);
		strcpy(alm_str->message, "No defined");
		return -1;
	}
	while(fgets(buffer, sizeof(buffer)-1, fp))
	{
		if(sscanf(buffer, "%d %s", &id, alm_str->message)!=2)
			continue;
		if(id==lms_rec->msg_ptr)
		{
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	strcpy(alm_str->message, "No defined");
	return -1;
}



int	GetCmdRecStrings(CMD_REC *cmd_rec, ALM_STR *alm_str, BOOL *blink, COLOR_PAIR *color)
{
	POINTER		point;
	GULONG		iValue;
	char		err_str[128];
	char		dev_longname[GROUP_LONGNAME_SIZE];
	char		rec_longname[RECORD_LONGNAME_SIZE];

	sprintf(alm_str->sn_str, "%04d", cmd_rec->rec_sn);

	sprintf(alm_str->time_str, "%04d-%02d-%02d %02d:%02d:%02d",
		cmd_rec->year, cmd_rec->month, cmd_rec->day,
		cmd_rec->hour, cmd_rec->minute, cmd_rec->second);

	if(-1==GetHostNameById(cmd_rec->host_id, alm_str->hostname))
	{
		strcpy(alm_str->hostname, "No define");
		sprintf(err_str, "No host define: %d", cmd_rec->host_id);
		Err_log(err_str);
	}

	strncpy(alm_str->usrname, cmd_rec->usr_name, USR_NAME_SIZE);

	GetPointFromRec((REC_ENTRY_UNION*)cmd_rec, &point);
	if(-1==GetPtLogicnameById(&point, alm_str->logicname))
	{
		strcpy(alm_str->longname, "No such point");
		sprintf(err_str, "No point define: %d.%d.%d.%d", cmd_rec->stn_id, 
			cmd_rec->dev_id, cmd_rec->data_type, cmd_rec->pt_id);
		Err_log( err_str);
	}
	else
	{
		switch(cmd_rec->type_id)
		{
		case	TIME_SYNC:
		case	C_LCU_ONOFF:
		case	C_UNIT:
		case	C_APOWER:
		case	C_RPOWER:
		case	C_VOLT_STEP:
		case	C_SPEED_STEP:
		case	C_NET_SWITCH:
		case	C_DATA_POLLING:
		case	C_LCU_PARAM:
		case	C_PLC_SWITCH:
		case	C_SIGNAL_RESET:
			GetStnLongnameById(point.stn_id, alm_str->longname);
			GetGroupLongnameById(point.stn_id, point.dev_id, dev_longname);
			strcat(alm_str->longname, dev_longname);
			break;

		default:
			GetPtLongnameById(&point, alm_str->longname);
			break;
		}
	}

	if(-1==GetPtCmdMsgStrById(&point, cmd_rec->type_id, cmd_rec->cmd_status.opr, alm_str->state_str))
	{
		strcpy(alm_str->state_str, "No define");
		sprintf(err_str, "No such cmd msg define: type %d, opr %d",  
			cmd_rec->type_id, cmd_rec->cmd_status.opr);
		Err_log( err_str);
		return -1;
	}

	if(-1==GetCmdSrcStr(cmd_rec->cmd_status.src, alm_str->cmd_src))
	{
		strcpy(alm_str->cmd_src, "No define");
		sprintf(err_str, "No such cmd src define: %d", cmd_rec->cmd_status.src);
		Err_log( err_str);
	}

	strcpy(alm_str->cmd_result, _EX_CS_("ALM_STR_CMD_RESULT", cmd_rec->cmd_status.result));

	switch(cmd_rec->type_id)
	{
	case	C_UNIT:
		if(cmd_rec->cmd_status.opr==1 && cmd_rec->data.iValue!=0)
		{
			sprintf(alm_str->state_str, "%s %s %.3f", alm_str->state_str,
				_CS_("ALM_STR_C_UNIT_YG"), cmd_rec->data.iValue/1000.0);
		}
		if(cmd_rec->cmd_status.opr==5)
		{
			sprintf(alm_str->state_str, "%s%d%s", alm_str->state_str,
				cmd_rec->data.iValue/1000, _CS_("ALM_STR_C_UNIT_JUMP"));
		}
		break;

	case	C_APOWER:
		if(cmd_rec->cmd_status.opr==0)
		{
			sprintf(alm_str->state_str, "%s %s %.2f", alm_str->state_str,
				_CS_("ALM_STR_C_APOWER"), cmd_rec->data.iValue/1000.0);
		}
		if(cmd_rec->cmd_status.opr==1)
		{
			sprintf(alm_str->state_str, "%s %.2f%%", alm_str->state_str,
				cmd_rec->data.iValue/1000.0);
		}
		else if(cmd_rec->cmd_status.opr==2)
		{
			sprintf(alm_str->state_str, "%s %s %.2f", alm_str->state_str,
				_CS_("ALM_STR_C_APOWER_FLUX"), cmd_rec->data.iValue/1000.0);
		}
		break;

	case	C_RPOWER:
		if(cmd_rec->cmd_status.opr==0)
		{
			sprintf(alm_str->state_str, "%s %s %.2f", alm_str->state_str,
				_CS_("ALM_STR_C_RPOWER"), cmd_rec->data.iValue/1000.0);
		}
		else if(cmd_rec->cmd_status.opr==1)
		{
			sprintf(alm_str->state_str, "%s %s %.2f", alm_str->state_str,
				_CS_("ALM_STR_C_RPOWER_VOLT"), cmd_rec->data.iValue/1000.0);
		}
		else if(cmd_rec->cmd_status.opr==2)
		{
			sprintf(alm_str->state_str, "%s %.2f", alm_str->state_str,
				cmd_rec->data.iValue/1000.0);
		}
		break;

	case	C_GATE:
		if(cmd_rec->cmd_status.opr==4||cmd_rec->cmd_status.opr==5)
		{
			char	unitName[128];
			GetAnaPtUnitNameById(&point, unitName);
			sprintf(alm_str->state_str, "%s (%s): %.3f", alm_str->state_str,
				unitName, cmd_rec->data.iValue/1000.0);
		}
		else if(cmd_rec->cmd_status.opr==13)
		{
			sprintf(alm_str->state_str, "%s %s %.3f", alm_str->state_str,
				_CS_("ALM_STR_C_GATE_FLUX"), cmd_rec->data.iValue/1000.0);
		}
		break;

	case	C_TRANS:
		if(cmd_rec->cmd_status.opr==4||cmd_rec->cmd_status.opr==5)
		{
			sprintf(alm_str->state_str, "%s %s %d", alm_str->state_str,
				_CS_("ALM_STR_C_TRANS_POS"), cmd_rec->data.iValue/1000);
		}
		break;

	case	C_ONOFF_FORCE:
		if(cmd_rec->cmd_status.opr==1||cmd_rec->cmd_status.opr==2)
		{
			sprintf(alm_str->state_str, "%s %s %d", alm_str->state_str,
				_CS_("ALM_STR_C_ONOFF_FORCE_TIME"), cmd_rec->data.iValue);
		}
		break;

	case	C_ANA_FORCE:
		if(cmd_rec->cmd_status.opr==1)
		{
			sprintf(alm_str->state_str, "%s: %.3f", alm_str->state_str,
				cmd_rec->data.iValue/1000.0);
		}
		break;

	case	C_OP_VALUE_SET:
		if(point.data_type!=CURVE_TYPE)
			sprintf(alm_str->state_str, "%s: %.3f", _CS_("ALM_STR_OP_VALUE_SET_CONST"),
				cmd_rec->data.iValue/1000.0);
		else
			sprintf(alm_str->state_str, "%s %.3f %s %d", _CS_("ALM_STR_OP_VALUE_SET_CONST"),
				cmd_rec->data.iValue/1000.0,
				_CS_("ALM_STR_CURVE_OFFSET"), cmd_rec->ctrl_addr);
		break;

	case	C_LCU_PARAM:
		GetRecLongnameByType(point.data_type, rec_longname);
		strcat(alm_str->longname, rec_longname);
		sprintf(alm_str->state_str, "%s %d:%d",
			alm_str->state_str, cmd_rec->pt_id, cmd_rec->data.iValue);
		break;

	case	DPS_IOST_SET:
		if(cmd_rec->cmd_status.opr>=1&&cmd_rec->cmd_status.opr<7)
		{
			if(cmd_rec->cmd_status.state>1)
			{
				sprintf(err_str, "IOST_SET state overrange: %d-%d", 
					cmd_rec->cmd_status.opr, cmd_rec->cmd_status.state);
				Err_log( err_str);
				break;
			}
			sprintf(alm_str->state_str, "%s %s %s", alm_str->state_str, _CS_("ALM_STR_SET_CONST"),
				cmd_rec->cmd_status.state? _CS_("ALM_STR_SET_STATE_YES"):_CS_("ALM_STR_SET_STATE_NO"));
		}
		else if(cmd_rec->cmd_status.opr==7)/*TAG*/
		{
			if(cmd_rec->cmd_status.state>3)
			{
				sprintf(err_str, "IOST_SET state overrange: %d-%d", 
					cmd_rec->cmd_status.opr, cmd_rec->cmd_status.state);
				Err_log( err_str);
				break;
			}
			sprintf(alm_str->state_str, "%s %s %s", alm_str->state_str, _CS_("ALM_STR_SET_CONST"),
				_EX_CS_("ALM_STR_IOST_TAG_SET", cmd_rec->cmd_status.state));
		}
		else
		{
			sprintf(err_str, "IOST_SET opr overrange: %d", cmd_rec->cmd_status.opr);
			Err_log(err_str);
			break;
		}
		break;

	case	DPS_IOATTR_SET:
		iValue= cmd_rec->data.iValue & 0xFFFF;
		if(cmd_rec->cmd_status.opr==IOATTR_SET_HDB_PERIOD) /*历史库转存类型*/
		{
			if(iValue>7)
			{
				sprintf(err_str, "IOATTR_SET state overrange: %d-%d", 
					cmd_rec->cmd_status.opr, iValue);
				Err_log( err_str);
				break;
			}
			sprintf(alm_str->state_str, "%s %s %s", alm_str->state_str, _CS_("ALM_STR_SET_CONST"),
				_EX_CS_("ALM_STR_HDB_PERIOD_TYPE", (USHORT)iValue));
		}
		else if(cmd_rec->cmd_status.opr==IOATTR_SET_PROCTYPE) /*接点类型*/
		{
			if(iValue>31)
			{
				sprintf(err_str, "IOATTR_SET state overrange: %d-%d", 
					cmd_rec->cmd_status.opr, iValue);
				Err_log( err_str);
				break;
			}
			sprintf(alm_str->state_str, "%s %s %d", 
				alm_str->state_str, _CS_("ALM_STR_SET_CONST"), iValue);
		}
		else if(cmd_rec->cmd_status.opr==IOATTR_SET_NORM_STATE)
		{
			if(cmd_rec->cmd_status.state>1)
			{
				sprintf(err_str, "IOATTR_SET state overrange: %d-%d", 
					cmd_rec->cmd_status.opr, cmd_rec->cmd_status.state);
				Err_log( err_str);
				break;
			}
			sprintf(alm_str->state_str, "%s %s %d", 
				alm_str->state_str, _CS_("ALM_STR_SET_CONST"), cmd_rec->cmd_status.state);
		}
		else if(cmd_rec->cmd_status.opr<IOATTR_SET_NORM_STATE)
		{
			if(cmd_rec->cmd_status.state>1)
			{
				sprintf(err_str, "IOATTR_SET state overrange: %d-%d", 
					cmd_rec->cmd_status.opr, cmd_rec->cmd_status.state);
				Err_log( err_str);
				break;
			}
			sprintf(alm_str->state_str, "%s %s %s", alm_str->state_str, _CS_("ALM_STR_SET_CONST"),
				cmd_rec->cmd_status.state? _CS_("ALM_STR_SET_STATE_YES"):_CS_("ALM_STR_SET_STATE_NO"));
		}
		else
		{
			sprintf(err_str, "IOATTR_SET opr overrange: %d", cmd_rec->cmd_status.opr);
			Err_log( err_str);
			break;
		}
		break;

	case	DPS_IO_GROUP_SET:
		iValue= cmd_rec->data.iValue & 0xFFFF;
		if(cmd_rec->cmd_status.opr==0)
			sprintf(alm_str->state_str, "%s %s %d", 
				alm_str->state_str, _CS_("ALM_STR_SET_CONST"), iValue);
		else
		{
			sprintf(err_str, "IO_GROUP_SET opr overrange: %d", cmd_rec->cmd_status.opr);
			Err_log( err_str);
			break;
		}
		break;

	case	DPS_MAN_VALUE_SET:
		if(point.data_type==CURVE_TYPE)
		{
			sprintf(alm_str->state_str, "%s %s%.3f %s %d", alm_str->state_str,
				_CS_("ALM_STR_SET_CONST"), cmd_rec->data.iValue/1000.0,
				_CS_("ALM_STR_CURVE_OFFSET"), cmd_rec->ctrl_addr);
		}
		else
		{
			sprintf(alm_str->state_str, "%s %s%.3f", alm_str->state_str,
				_CS_("ALM_STR_SET_CONST"), cmd_rec->data.iValue/1000.0);
		}
		break;
		
	case	DPS_MAN_CURVE_SET:
		sprintf(alm_str->state_str, "%s %s%.3f %s %d", alm_str->state_str,
			_CS_("ALM_STR_SET_CONST"), cmd_rec->data.iValue/1000.0,
			_CS_("ALM_STR_CURVE_OFFSET"), cmd_rec->ctrl_addr);
		break;

	case	DPS_MAN_TIME_SET:
		{
			struct tm timeValue;
			char	timeStr[256];
			timeValue= *localtime((time_t*)&(cmd_rec->data.iValue));
			sprintf(timeStr, "%04d-%02d-%02d %02d:%02d:%02d:%03d",
				timeValue.tm_year+1900, timeValue.tm_mon+1,
				timeValue.tm_mday, timeValue.tm_hour, timeValue.tm_min,
				timeValue.tm_sec, cmd_rec->ctrl_addr);

			sprintf(alm_str->state_str, "%s %s%s", alm_str->state_str,
				_CS_("ALM_STR_SET_CONST"), timeStr);
		}
		break;
	}
	
	return 0;
}


int BroadCastHostSwitchCmd(char *hostname, BOOL main_state, char *usr_name)
{      
	COMMAND_MSG     msg;
	LAN_HEAD        head;
	DATE_TIME	date_time;
	POINTER		point;
	
	
	if(-1==GetHostStatePtId(hostname, &point))		
		return -1;
	
	GetSysTime(&date_time);
	
	msg.type_id= C_HOST_SYS;
	msg.stn_id= point.stn_id;
	msg.dev_id= point.dev_id;
	msg.data_type= point.data_type;
	msg.point_id= point.pt_id;
	if(main_state)
		msg.status.opr= 1;//main
	else
		msg.status.opr= 2;//vice
	msg.status.state= 0;
	msg.status.src= 0;
	msg.status.result= 0;
	msg.data.iValue= 0;
	msg.ctrl_addr= 0;
	GetSrcId((UCHAR	*)&msg.host_id);
	msg.usr_id= -1;
	msg.second= date_time.second;
	msg.minute= date_time.minute;
	msg.hour= date_time.hour;
	msg.day= date_time.day;
	msg.month= date_time.month;
	msg.year= date_time.year;
	strncpy(msg.usr_login_name, usr_name, sizeof(msg.usr_login_name));
	
	head.length= sizeof(COMMAND_MSG);
	head.dp_type= COMM_RECORD_TIME;
	head.dest_id= 0;
	GetLocalHostId(&head.src_id);
	head.dest_stn= GetModuleId();
	
	Swap2Byte(&head.length);
	Swap2Byte(&msg.point_id);
	Swap2Byte(&msg.status);
	Swap4Byte(&msg.data);
	Swap2Byte(&msg.ctrl_addr);
	Swap2Byte(&msg.year);

	lan_out ((char *)&msg,head,0);  
	return(0);
}       


int FillTimeMsg(UCHAR *time_buf)
{
	DATE_TIME	time;

	GetSysTime(&time);
	time_buf[0]= time.second;
	time_buf[1]= time.minute;
	time_buf[2]= time.hour;
	time_buf[3]= time.day;
	time_buf[4]= time.month;
	memcpy(&time_buf[5], &time.year, 2);
	Swap2Byte(&time_buf[5]);
	return time.ms;
}

void FillIndAlmInfo(POINTER point, IND_DATA status, DATE_TIME *date_time, SP_IND_ALM_PKG *info)
{
	UCHAR			hostId;
	
	info->type_id= DPS_IND_ALM;
	info->stn_id= point.stn_id;
	info->dev_id= point.dev_id;
	info->data_type= point.data_type;
	info->pt_num= 1;
	Swap2Byte(&info->pt_num);
	info->pt_id= point.pt_id;
	Swap2Byte(&info->pt_id);
	memcpy(&info->ind_status, &status, 2);
	Swap2Byte(&info->ind_status);
	info->ms= date_time->ms;
	Swap2Byte(&info->ms);
	GetLocalHostId(&hostId);
	info->host_id= (char)hostId;
	info->second= date_time->second;
	info->minute= date_time->minute;
	info->hour= date_time->hour;
	info->day= date_time->day;
	info->month= date_time->month;
	info->year= date_time->year;
	Swap2Byte(&info->year);
}

void FillIndAllSendInfo(POINTER point, IND_DATA status, DATE_TIME *date_time, SP_IND_ALLSEND_PKG *info)
{
	UCHAR			hostId;
	
	info->type_id= DPS_STATUS_ALL;
	info->stn_id= point.stn_id;
	info->dev_id= point.dev_id;
	info->data_type= point.data_type;
	info->pt_num= 1;
	Swap2Byte(&info->pt_num);
	info->pt_id= point.pt_id;
	Swap2Byte(&info->pt_id);
	memcpy(&info->ind_status, &status, 2);
	Swap2Byte(&info->ind_status);
	GetLocalHostId(&hostId);
	info->host_id= (char)hostId;
	info->second= date_time->second;
	info->minute= date_time->minute;
	info->hour= date_time->hour;
	info->day= date_time->day;
	info->month= date_time->month;
	info->year= date_time->year;
	Swap2Byte(&info->year);
}

int BroadCastIndPtState(POINTER point, UCHAR net, USHORT state)
{
	LAN_HEAD		head;
	SP_IND_ALM_PKG	info;
	UCHAR			hostId;

	head.length= sizeof(SP_IND_ALM_PKG);
	Swap2Byte(&head.length);
	head.dp_type= BJ_DB_TIME;
	head.dest_id= 0;
	if(-1==GetLocalHostId(&head.src_id))
		return -1;
	head.dest_stn= GetModuleId();
	
	info.type_id= DPS_IND_ALM;
	info.stn_id= point.stn_id;
	info.dev_id= point.dev_id;
	info.data_type= point.data_type;
	info.pt_num= 1;
	Swap2Byte(&info.pt_num);
	info.pt_id= point.pt_id;
	Swap2Byte(&info.pt_id);
	memset(&info.ind_status, 0, 2);
	info.ind_status.alm_type= IND_ALM_TYPE_CHANGE;
	info.ind_status.rt_state= state;
	Swap2Byte(&info.ind_status);
	GetLocalHostId(&hostId);
	info.host_id= (char)hostId;
	info.ms=FillTimeMsg(&info.second);
	Swap2Byte(&info.ms);

	lan_out((char*)&info, head, net);
	return 0;
}

int BroadCastIndPt(POINTER point, UCHAR net, IND_DATA status, DATE_TIME *date_time)
{
	LAN_HEAD		head;
	SP_IND_ALM_PKG	info;
	UCHAR			hostId;
	
	head.length= sizeof(SP_IND_ALM_PKG);
	Swap2Byte(&head.length);
	head.dp_type= BJ_DB_TIME;
	head.dest_id= 0;
	if(-1==GetLocalHostId(&head.src_id))
		return -1;
	head.dest_stn= GetModuleId();
	
	info.type_id= DPS_IND_ALM;
	info.stn_id= point.stn_id;
	info.dev_id= point.dev_id;
	info.data_type= point.data_type;
	info.pt_num= 1;
	Swap2Byte(&info.pt_num);
	info.pt_id= point.pt_id;
	Swap2Byte(&info.pt_id);
	memcpy(&info.ind_status, &status, 2);
	Swap2Byte(&info.ind_status);
	info.ms= date_time->ms;
	Swap2Byte(&info.ms);
	GetLocalHostId(&hostId);
	info.host_id= (char)hostId;
	info.second= date_time->second;
	info.minute= date_time->minute;
	info.hour= date_time->hour;
	info.day= date_time->day;
	info.month= date_time->month;
	info.year= date_time->year;
	Swap2Byte(&info.year);
	
	lan_out((char*)&info, head, net);
	return 0;
}


void FillAnaAlmInfo(POINTER point, float value, ANLG_STATUS status, DATE_TIME *date_time, SP_ANA_INFO_PKG *info)
{
	GULONG			data;
	UCHAR			hostId;

	if(status.alm_type)
		info->type_id= DPS_ANA_ALM;
	else
		info->type_id= DPS_ANALOG;
	
	info->stn_id= point.stn_id;
	info->dev_id= point.dev_id;
	info->data_type= point.data_type;
	info->pt_num= 1;
	Swap2Byte(&info->pt_num);
	info->pt_id= point.pt_id;
	Swap2Byte(&info->pt_id);
	memcpy(&info->ana_status, &status, 2);
	Swap2Byte(&info->ana_status);
	data= (GULONG)(value * 1000.);
	memcpy(info->data, &data, 4);
	Swap4Byte(&info->data);
	GetLocalHostId(&hostId);
	info->host_id= (char)hostId;
	info->second= date_time->second;
	info->minute= date_time->minute;
	info->hour= date_time->hour;
	info->day= date_time->day;
	info->month= date_time->month;
	info->year= date_time->year;
	Swap2Byte(&info->year);
}

void FillAnaAllSendInfo(POINTER point, float value, ANLG_STATUS status, DATE_TIME *date_time, SP_ANA_INFO_PKG *info)
{
	GULONG			data;
	UCHAR			hostId;
	
	info->type_id= DPS_ANA_ALL;
	info->stn_id= point.stn_id;
	info->dev_id= point.dev_id;
	info->data_type= point.data_type;
	info->pt_num= 1;
	Swap2Byte(&info->pt_num);
	info->pt_id= point.pt_id;
	Swap2Byte(&info->pt_id);
	memcpy(&info->ana_status, &status, 2);
	Swap2Byte(&info->ana_status);
	data= (GULONG)(value * 1000.);
	memcpy(info->data, &data, 4);
	Swap4Byte(&info->data);
	GetLocalHostId(&hostId);
	info->host_id= (char)hostId;
	info->second= date_time->second;
	info->minute= date_time->minute;
	info->hour= date_time->hour;
	info->day= date_time->day;
	info->month= date_time->month;
	info->year= date_time->year;
	Swap2Byte(&info->year);
}

int BroadCastAnaPtValue(POINTER point, UCHAR net, float value)
{
	LAN_HEAD		head;
	SP_ANA_INFO_PKG	info;
	UCHAR			hostId;
	GULONG			data;
	
	head.length= sizeof(SP_ANA_INFO_PKG);
	Swap2Byte(&head.length);
	head.dp_type= BJ_DB_TIME;
	head.dest_id= 0;
	if(-1==GetLocalHostId(&head.src_id))
		return -1;
	head.dest_stn= GetModuleId();
	
	info.type_id= DPS_ANALOG;
	info.stn_id= point.stn_id;
	info.dev_id= point.dev_id;
	info.data_type= point.data_type;
	info.pt_num= 1;
	Swap2Byte(&info.pt_num);
	info.pt_id= point.pt_id;
	Swap2Byte(&info.pt_id);
	memset(&info.ana_status, 0, 2);
	Swap2Byte(&info.ana_status);
	data= (GULONG)(value * 1000.);
	memcpy(info.data, &data, 4);
	Swap4Byte(&info.data);
	GetLocalHostId(&hostId);
	info.host_id= (char)hostId;
	FillTimeMsg(&info.second);
	
	lan_out((char*)&info, head, net);
	return 0;
}


int BroadCastAnaPt(POINTER point, UCHAR net, float value, ANLG_STATUS status, DATE_TIME *date_time)
{
	LAN_HEAD		head;
	SP_ANA_INFO_PKG	info;
	UCHAR			hostId;
	GULONG			data;
	
	head.length= sizeof(SP_ANA_INFO_PKG);
	Swap2Byte(&head.length);
	head.dp_type= BJ_DB_TIME;
	head.dest_id= 0;
	if(-1==GetLocalHostId(&head.src_id))
		return -1;
	head.dest_stn= GetModuleId();
	
	if(status.alm_type)
		info.type_id= DPS_ANA_ALM;
	else
		info.type_id= DPS_ANALOG;
	info.stn_id= point.stn_id;
	info.dev_id= point.dev_id;
	info.data_type= point.data_type;
	info.pt_num= 1;
	Swap2Byte(&info.pt_num);
	info.pt_id= point.pt_id;
	Swap2Byte(&info.pt_id);
	memcpy(&info.ana_status, &status, 2);
	Swap2Byte(&info.ana_status);
	data= (GULONG)(value * 1000.);
	memcpy(info.data, &data, 4);
	Swap4Byte(&info.data);
	GetLocalHostId(&hostId);
	info.host_id= (char)hostId;
	info.second= date_time->second;
	info.minute= date_time->minute;
	info.hour= date_time->hour;
	info.day= date_time->day;
	info.month= date_time->month;
	info.year= date_time->year;
	Swap2Byte(&info.year);
	
	lan_out((char*)&info, head, net);
	return 0;
}


int BroadCastDataInfo(UCHAR dp_type, char info[], int length, UCHAR net)
{
	LAN_HEAD		head;

	head.length= length;
	Swap2Byte(&head.length);
	head.dp_type= dp_type;
	head.dest_id= 0;
	if(-1==GetLocalHostId(&head.src_id))
		return -1;
	head.dest_stn= GetModuleId();
	if(lan_out(info, head, net))
		return 0;
	else
		return -1;
}


int BroadCastSms(char* message, BOOL IsAlm)
{
	LAN_HEAD	head;
	time_t     	tt_time;
	char 		smsbuf[SMS_MESSAGE_SIZE+6];
	int			send_bytes, length;

	head.length= SMS_MESSAGE_SIZE+6;
	Swap2Byte(&head.length);
	head.dp_type= BJ_DB_TIME;
	head.dest_id= 0;
	if(-1==GetLocalHostId(&head.src_id))
		return -1;
	head.dest_stn= GetModuleId();
	
	if(IsAlm)
		smsbuf[0]= DPS_SM_ALM;
	else
		smsbuf[0]= DPS_SMS;
	smsbuf[1]=head.src_id;

	time(&tt_time);
	Swap4Byte(&tt_time);
	memcpy(&smsbuf[SMS_MESSAGE_SIZE+2], &tt_time, 4);	

	length= strlen(message);
	for(send_bytes=0; send_bytes<length; send_bytes += SMS_MESSAGE_SIZE)
	{
		strncpy(&smsbuf[2], &message[send_bytes], SMS_MESSAGE_SIZE);
		lan_out((char *)smsbuf, head, 0);
	}
	return 0;
}

int BroadCastLms(short lms_id)
{
	LAN_HEAD	head;
	unsigned char 		lmsbuf[12]; 

	head.length= SMS_MESSAGE_SIZE+6;
	Swap2Byte(&head.length);
	head.dp_type= BJ_DB_TIME;
	head.dest_id= 0;
	if(-1==GetLocalHostId(&head.src_id))
		return -1;
	head.dest_stn= GetModuleId();
	
	lmsbuf[0]=DPS_LMS;
	lmsbuf[1]=head.src_id;
	memcpy(&lmsbuf[2],&lms_id,2);
	Swap2Byte(&lmsbuf[2]);
	lmsbuf[4]=head.src_id;
	FillTimeMsg(&lmsbuf[5]);
	lan_out((char *)lmsbuf,head,0);
	return TRUE;
}



void FilterAudStr(char *audStr)
{
	char	string[256];
	char	*pSrcStr= audStr;
	char	*pDesStr= string;
	char	*hanzi_hao= "号";
	char	*hanzifu_hao= "＃";
	char	*hanzifu_zuokuohao= "（"; 
	char	*hanzifu_youkuohao= "）"; 
	char	*hanzifu_KV= "千伏"; 
	char	*hanzifu_KA= "千安"; 
	char	*hanzifu_KW= "千瓦"; 
	char	*hanzifu_V= "伏"; 
	char	*hanzifu_A= "安"; 
	char	*hanzifu_W= "瓦"; 
	char	*jizuhao= "号机";
	char	*tiaosuqi= "调速器";
	char	*tiaosuqi_new= "条速器";
	char	*chonghezha= "重合闸";
	char	*chonghezha_new= "虫合闸";
	char	*dayu= "大于";
	char	*xiaoyu= "小于";
	char	*hanzi_xiang= "相";
	char	*hanzi_xiang_new= "向";
	char	*hanzi_xiandi= "现地";
	char	*hanzi_xiandi_new= "现弟";
	char	*hanzi_shan= "栅";
	char	*hanzi_shan_new= "衫";
	char	*hanzi_cha= "差";
	char	*hanzi_cha_new= "嚓";
	
	
	char	*hanzi_lianjie= "连接";//cxs
	char	*hanzi_lianjie_new= "联结";//cxs
	char	*hanzi_yitaiwang= "以太网";//cxs
	char	*hanzi_yitaiwang_new= "伊泰网";//cxs
	char	*hanzi_dianling= ".000";//cxs
	char	*hanzi_dianling_new= ".   ";//cxs


	while(*pSrcStr)
	{
		if(*pSrcStr=='#')
		{
			memcpy(pDesStr, hanzi_hao, 2);
			pSrcStr++;
			pDesStr += 2;
		}
		else if(*pSrcStr=='>')
		{
			memcpy(pDesStr, dayu, 4);
			pSrcStr++;
			pDesStr += 4;
		}
		else if(*pSrcStr=='<')
		{
			memcpy(pDesStr, xiaoyu, 4);
			pSrcStr++;
			pDesStr += 4;
		}
		else if(*pSrcStr==*hanzi_lianjie && *(pSrcStr+1)==*(hanzi_lianjie+1)//cxs  start
			&& *(pSrcStr+2)==*(hanzi_lianjie+2) && *(pSrcStr+3)==*(hanzi_lianjie+3))
		{
			memcpy(pDesStr, hanzi_lianjie_new, 4);
			pSrcStr +=4;
			pDesStr +=4;
		}	
		else if(*pSrcStr==*hanzi_dianling && *(pSrcStr+1)==*(hanzi_dianling+1)
			&& *(pSrcStr+2)==*(hanzi_dianling+2) && *(pSrcStr+3)==*(hanzi_dianling+3))
		{
			memcpy(pDesStr, hanzi_dianling_new, 4);
			pSrcStr +=4;
			pDesStr +=4;
		}
		else if(*pSrcStr==*hanzi_yitaiwang && *(pSrcStr+1)==*(hanzi_yitaiwang+1) 
			&& *(pSrcStr+2)==*(hanzi_yitaiwang+2) && *(pSrcStr+3)==*(hanzi_yitaiwang+3)
			&& *(pSrcStr+4)==*(hanzi_yitaiwang+4) && *(pSrcStr+5)==*(hanzi_yitaiwang+5))
		{
			memcpy(pDesStr, hanzi_yitaiwang_new, 6);
			pSrcStr +=6;
			pDesStr +=6;
		}	//cxs  end
		else if(*pSrcStr==*hanzifu_hao && *(pSrcStr+1)==*(hanzifu_hao+1))
		{
			memcpy(pDesStr, hanzi_hao, 2);
			pSrcStr += 2;
			pDesStr += 2;
		}
		else if(*pSrcStr==*hanzi_xiang && *(pSrcStr+1)==*(hanzi_xiang+1))
		{
			memcpy(pDesStr, hanzi_xiang_new, 2);
			pSrcStr += 2;
			pDesStr += 2;
		}
		else if(*pSrcStr==*hanzi_shan && *(pSrcStr+1)==*(hanzi_shan+1))
		{
			memcpy(pDesStr, hanzi_shan_new, 2);
			pSrcStr += 2;
			pDesStr += 2;
		}
		else if(*pSrcStr==*hanzi_cha && *(pSrcStr+1)==*(hanzi_cha+1))
		{
			memcpy(pDesStr, hanzi_cha_new, 2);
			pSrcStr += 2;
			pDesStr += 2;
		}
		else if((*pSrcStr=='k'||*pSrcStr=='K') && 
			(*(pSrcStr+1)=='v'||*(pSrcStr+1)=='V'))
		{
			strcpy(pDesStr, hanzifu_KV);
			pSrcStr += 2;
			pDesStr += 4;
		}
		else if((*pSrcStr=='k'||*pSrcStr=='K') && 
			(*(pSrcStr+1)=='a'||*(pSrcStr+1)=='A'))
		{
			strcpy(pDesStr, hanzifu_KA);
			pSrcStr += 2;
			pDesStr += 4;
		}
		else if((*pSrcStr=='k'||*pSrcStr=='K') && 
			(*(pSrcStr+1)=='w'||*(pSrcStr+1)=='W'))
		{
			strcpy(pDesStr, hanzifu_KW);
			pSrcStr += 2;
			pDesStr += 4;
		}
		else if(isdigit(*pSrcStr) && 
			(*(pSrcStr+1)=='v'||*(pSrcStr+1)=='V'))
		{
			*pDesStr= *pSrcStr;
			memcpy(pDesStr+1, hanzifu_V, 2);
			pSrcStr += 2;
			pDesStr += 3;
		}
		else if(isdigit(*pSrcStr) && 
			(*(pSrcStr+1)=='a'||*(pSrcStr+1)=='A'))
		{
			*pDesStr= *pSrcStr;
			memcpy(pDesStr+1, hanzifu_A, 2);
			pSrcStr += 2;
			pDesStr += 3;
		}
		else if(isdigit(*pSrcStr) && 
			(*(pSrcStr+1)=='w'||*(pSrcStr+1)=='W'))
		{
			*pDesStr= *pSrcStr;
			memcpy(pDesStr+1, hanzifu_W, 2);
			pSrcStr += 2;
			pDesStr += 3;
		}
		else if(isdigit(*pSrcStr) && (*(pSrcStr+1)=='F'))
		{
			*pDesStr= *pSrcStr;
			memcpy(pDesStr+1, jizuhao, 4);
			pSrcStr += 2;
			pDesStr += 5;
		}
		else if(*pSrcStr=='(' || *pSrcStr==')'// || *pSrcStr==' '
			|| *pSrcStr=='\\' || *pSrcStr=='/')
		{
			pSrcStr++;
		}
		else if(*pSrcStr==*hanzifu_zuokuohao && *(pSrcStr+1)==*(hanzifu_zuokuohao+1))
		{
			pSrcStr += 2;
		}
		else if(*pSrcStr==*hanzifu_youkuohao && *(pSrcStr+1)==*(hanzifu_youkuohao+1))
		{
			pSrcStr += 2;
		}
		else if(*pSrcStr=='I' && *(pSrcStr+1)=='I' && *(pSrcStr+2)=='I')
		{
			*pDesStr= '3';
			pSrcStr += 3;
			pDesStr += 1;
		}
		else if(*pSrcStr=='I' && *(pSrcStr+1)=='I')
		{
			*pDesStr= '2';
			pSrcStr += 2;
			pDesStr += 1;
		}
		else if(*pSrcStr=='I' && pSrcStr!=audStr 
			&& !isalnum(*(pSrcStr-1)) && !isalnum(*(pSrcStr+1)))
		{
			*pDesStr= '1';
			pSrcStr += 1;
			pDesStr += 1;
		}
		else if(*pSrcStr==*hanzi_xiandi && *(pSrcStr+1)==*(hanzi_xiandi+1)
			&& *(pSrcStr+2)==*(hanzi_xiandi+2) && *(pSrcStr+3)==*(hanzi_xiandi+3))
		{
			memcpy(pDesStr, hanzi_xiandi_new, 4);
			pSrcStr +=4;
			pDesStr +=4;
		}		
		else if(*pSrcStr==*tiaosuqi && *(pSrcStr+1)==*(tiaosuqi+1)
			&& *(pSrcStr+2)==*(tiaosuqi+2) && *(pSrcStr+3)==*(tiaosuqi+3)
			&& *(pSrcStr+4)==*(tiaosuqi+4) && *(pSrcStr+5)==*(tiaosuqi+5))
		{
			memcpy(pDesStr, tiaosuqi_new, 6);
			pSrcStr +=6;
			pDesStr +=6;
		}		
		else if(*pSrcStr==*chonghezha && *(pSrcStr+1)==*(chonghezha+1)
			&& *(pSrcStr+2)==*(chonghezha+2) && *(pSrcStr+3)==*(chonghezha+3)
			&& *(pSrcStr+4)==*(chonghezha+4) && *(pSrcStr+5)==*(chonghezha+5))
		{
			memcpy(pDesStr+1, chonghezha_new, 6);
			pSrcStr +=6;
			pDesStr +=6;
		}		
		else
		{
			*pDesStr= *pSrcStr;
			pSrcStr++;
			pDesStr++;
		}
	}
	*pDesStr= 0;
	strcpy(audStr, string);
}



BOOL IsAlmRecord(REC_ENTRY_UNION *rec)
{
	POINTER		point;
	GULONG		normalState;
	USHORT		state;

	if(rec->ana_rec.type_id==DPS_SM_ALM)
		return TRUE;

	if(-1==GetPointFromRec(rec, &point))
		return FALSE;

	if(rec->ana_rec.type_id==DPS_ANA_ALM)
	{
		if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_LMT)
		{
			state= rec->ana_rec.status.ana_status.lmt_state;
			if(state==ANA_ALM_HI_ALARM || state==ANA_ALM_HI_WARN
				|| state==ANA_ALM_LO_WARN || state==ANA_ALM_LO_ALARM)
				return TRUE;
			else
				return FALSE;
		}
		else if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_TR)
		{
			if(rec->ana_rec.status.ana_status.trend_state==ANA_ALM_TR_WARN)
				return TRUE;
			else
				return FALSE;
		}
		else if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_CHANFAIL)
		{
			if(rec->ana_rec.status.ana_status.chan_fail)
				return TRUE;
			else
				return FALSE;
		}
		else if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_OVERFLOW)
		{
			if(rec->ana_rec.status.ana_status.overflow)
				return TRUE;
			else
				return FALSE;
		}
	}
	else if(rec->ana_rec.type_id==DPS_IMP_ALM)
	{
		if(rec->ana_rec.status.imp_status.alm_type==IMP_ALM_TYPE_CHANFAIL)
		{
			if(rec->ana_rec.status.imp_status.chan_fail)
				return TRUE;
			else
				return FALSE;
		}
	}
	else if(rec->ana_rec.type_id==DPS_IND_ALM)
	{
		if(rec->ind_rec.ind_status.alm_type==IND_ALM_TYPE_CHANGE)
		{
			GetPtNormalStateById(&point, &normalState);
			if(rec->ind_rec.ind_status.rt_state!=normalState)
				return TRUE;
			else
				return FALSE;
		}
		else if(rec->ind_rec.ind_status.alm_type==IND_ALM_TYPE_CHANFAIL)
		{
			if(rec->ind_rec.ind_status.chan_fail)
				return TRUE;
			else
				return FALSE;
		}
	}
	return FALSE;
}


int	GetEventTxtString(REC_ENTRY_UNION *rec, char *text_str)
{
	ALM_STR		rec_str;
	BOOL		blink;
	char		*pStr= text_str;

	text_str[0]= 0;
	GetAlmStrings(rec, &rec_str, &blink, NULL);

/*	pStr= text_str;
	pStr += sprintf(pStr, "%s\t", rec_str.sn_str);
	pStr += sprintf(pStr, "%-26s\t", rec_str.time_str);
	pStr += sprintf(pStr, "%-30s\t", rec_str.logicname);
	if(rec_str.message[0])
		pStr += sprintf(pStr, "%-40s\t", rec_str.message);
	else
		pStr += sprintf(pStr, "%-40s\t", rec_str.longname);
	pStr += sprintf(pStr, "%16s\t", rec_str.state_str);
	pStr += sprintf(pStr, "%12s\t", rec_str.cmd_result);
	if(rec_str.usrname[0])
		pStr += sprintf(pStr, "%s@%s\t", rec_str.usrname, rec_str.hostname);
	else
		pStr += sprintf(pStr, "%s\t", rec_str.hostname);
*/
		
	pStr += sprintf(pStr, "%s\t", rec_str.sn_str);
	pStr += sprintf(pStr, "%s\t", rec_str.time_str);
	pStr += sprintf(pStr, "%s\t", rec_str.logicname);
	if(rec_str.message[0])
		pStr += sprintf(pStr, "%s\t", rec_str.message);
	else
		pStr += sprintf(pStr, "%s\t", rec_str.longname);
	pStr += sprintf(pStr, "%s\t", rec_str.state_str);
	pStr += sprintf(pStr, "%s\t", rec_str.cmd_result);
	if(rec_str.usrname[0])
		pStr += sprintf(pStr, "%s@%s\t", rec_str.usrname, rec_str.hostname);
	else
		pStr += sprintf(pStr, "%s\t", rec_str.hostname);
		

	return 0;
}


long	GetAbsTimeFromDateTime(DATE_TIME *date_time)
{
	struct	tm	tm_time;

	tm_time.tm_isdst= 0;
	tm_time.tm_year= date_time->year-1900;
	tm_time.tm_mon= date_time->month-1;
	tm_time.tm_mday= date_time->day;
	tm_time.tm_hour= date_time->hour;
	tm_time.tm_min= date_time->minute;
	tm_time.tm_sec= date_time->second;

	return mktime(&tm_time);
}

