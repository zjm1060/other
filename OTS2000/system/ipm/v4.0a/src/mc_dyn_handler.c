
#include	<windows.h>
#include	<stdio.h>

#include	"resource.h"

#include	"../inc/draw_def.h"
#include	"../inc/ipm_k_def.h"
#include	"../inc/tmplt.h"
#include	"../inc/dyn_form_def.h"
#include	"../inc/dyn_prog_def.h"
#include	"../inc/menu_def.h"

void	DeleteReqHandler (int);
void GetCurMbrItemName(int, int, char *); 
extern NEW_GBRCD	tmp_edit_dyn_rcd;


void	DynTextHandler ( HWND	hWndEdit, int	req_type)
{

	char	input_str[MAX_INPUT_TEXT_LEN];
	float		input_data;
	int	data;
	short	short_data;
//  DMS_COMMON    	common_aa;
	SendMessage(hWndEdit, WM_GETTEXT, MAX_INPUT_TEXT_LEN, (LPARAM)input_str);

	switch( req_type)
	{
	case	DB_ENTRY:
		strcpy (tmp_edit_dyn_rcd.lnk.dbname, input_str);
		break;
	case   DB_ENTRY_V_L:
		strcpy (tmp_edit_dyn_rcd.lnk.dyn.fmt_def.vibra_area.dbname2   , input_str);
		break;
    case   DB_ENTRY_R_C:
		strcpy (tmp_edit_dyn_rcd.lnk.dyn.fmt_def.load_curve.LoadDBName   , input_str);
		break ;
	case	DSP_ENTRY:
		tmp_edit_dyn_rcd.lnk.act.ctrl_type	= CTRL_REQ_DSP;
		tmp_edit_dyn_rcd.lnk.act.ctrl_tag	= CTRL_TAG_ON;
		strcpy (tmp_edit_dyn_rcd.lnk.act.fmt_def.dsp.name , input_str);
		if( tmp_edit_dyn_rcd.lnk.dyn.dyn_tag == DYN_TAG_ON && tmp_edit_dyn_rcd.lnk.dyn.dyn_type == DF_DSP_REQ-1)
		if( tmp_edit_dyn_rcd.lnk.dyn.dyn_tag == DYN_TAG_ON && tmp_edit_dyn_rcd.lnk.dyn.dyn_type == DF_DSP_REQ)
		{
			tmp_edit_dyn_rcd.lnk.dyn.dyn_tag	= DYN_TAG_OFF;
			tmp_edit_dyn_rcd.lnk.dyn.dyn_type	= 0;
		}
		break;

	case	EQUAT_ENTRY:
		tmp_edit_dyn_rcd.lnk.act.ctrl_type	= CTRL_REQ_OPER_GUIDE;
		tmp_edit_dyn_rcd.lnk.act.ctrl_tag	= CTRL_TAG_ON;
		strcpy (tmp_edit_dyn_rcd.lnk.act.fmt_def.dsp.name , input_str);
		if( tmp_edit_dyn_rcd.lnk.dyn.dyn_tag == DYN_TAG_ON && 
			tmp_edit_dyn_rcd.lnk.dyn.dyn_type == CTRL_REQ_OPER_GUIDE-1)
		{
			tmp_edit_dyn_rcd.lnk.dyn.dyn_tag	= DYN_TAG_OFF;
			tmp_edit_dyn_rcd.lnk.dyn.dyn_type	= 0;
		}
		break;
	case	LOAD_CUR_HI	:
			input_data	=  (float)atof(input_str);
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.load_curve.hi_limit	= input_data;
		break;
    case	LOAD_CUR_LOW:
			input_data	=  (float)atof(input_str);
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.load_curve.low_limit	= input_data;
		break;

	case	HIGH_LIMIT:
		input_data	=  (float)atof(input_str);
		switch( dyn_format)
		{
		case	DF_LINE_PLOT:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.curve.hi_limit	= input_data;	
			break;
		case	DF_CUR_LOAD:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.load_curve.hi_limit	= input_data;	
			break;

		case	DF_PQ_CHART:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.curve.hi_limit	= input_data;	
			break;
        case    DF_VIBRA_ARAEA:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.vibra_area.hi_limit	= input_data;
			break;
		case	DF_BAR_CHART:
		case    DF_DIAL_METER:
	
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.bar_graph.hi_limit	= input_data;	
			break;

		case	DF_HIST_BAR:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.hist_bar.hi_limit	= input_data;	
			break;
			
		default:
			break;
		}
		break;

	case	LOW_LIMIT:
		input_data	= (float)atof(input_str);
		switch( dyn_format)
		{
		case	DF_LINE_PLOT:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.curve.low_limit	= input_data;
			break;
	   case	DF_CUR_LOAD:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.load_curve.low_limit	= input_data;
			break ;
		case	DF_PQ_CHART:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.curve.low_limit	= input_data;
			break;
        case    DF_VIBRA_ARAEA:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.vibra_area.low_limit	= input_data;
			break;
		case	DF_BAR_CHART:
		case    DF_DIAL_METER:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.bar_graph.low_limit	= input_data;
			break;
		case	DF_HIST_BAR:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.hist_bar.low_limit	= input_data;	
		default:
			break;	
		}
		break;

	case	HIGH_LIMIT_2:
		input_data	= (float)atof ( input_str );
		switch ( dyn_format )
		{
		case	DF_PQ_CHART:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.curve.hi_limit_2	= input_data;	
			break;

		default:
			break;	
		}
		break;

	case	LOW_LIMIT_2:
		input_data	= (float)atof ( input_str );
		switch ( dyn_format )
		{
		case	DF_PQ_CHART:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.curve.low_limit_2	= input_data;	
			break;

		default:
			break;	
		}
		break;

	case	F_B_OFF:
		data = atoi(input_str);
		tmp_edit_dyn_rcd.lnk.dyn.f_by_off = data;
		break;

	case	DEC_NUM:
		short_data =(short) atoi(input_str);
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.dec_pt =short_data; 	
		break;

	case	DATA_LEN:
		short_data = (short)atoi(input_str);
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.length = short_data;
		break;
   	case	DATA_LEN_TEMP:
		short_data = (short)atoi(input_str);
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.temp_chara.length = short_data;
		break;
	case	CURVE_PT_NUM:
		short_data = (short)atoi(input_str);      
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.curve_point.point_id = short_data;
		break;

	case	TIME_LENGTH	:
		input_data	=  (float)atof(input_str);
	//	tmp_edit_dyn_rcd.lnk.dyn.fmt_def.scene_redisp.TimeLength	= input_data;
		switch ( dyn_format )
		{
		case	DF_SCENE_REDISP:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.scene_redisp.TimeLength	= input_data;
			break;
		case    DF_REAL_CURVE:
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.real_curve .TimeLength 	= input_data;
			break;
		default:
			break;	
		}


		break;
	case	START_TIME:
		switch ( dyn_format )
		{
		case	DF_SCENE_REDISP:
			strcpy (tmp_edit_dyn_rcd.lnk.dyn.fmt_def.scene_redisp.StartTime   , input_str);	
			break;
		case    DF_REAL_CURVE:
			strcpy (tmp_edit_dyn_rcd.lnk.dyn.fmt_def.real_curve.StartTime     , input_str);
			break;
		default:
			break;	
		}
		
		break;
	case	DSP_FILE_NM:
		strcpy (tmp_edit_dyn_rcd.lnk.dyn.fmt_def.scene_redisp.ReDspFileNm , input_str);
		break;


	default:
		break;
		
	}		
	
}


void	DynMbrDioHandler (int	mbr_value)
{
	tmp_edit_dyn_rcd.lnk.dyn.dio_type = mbr_value; 
}	

void	DynMbrTagHandler (int	mbr_value)
{
	tmp_edit_dyn_rcd.lnk.dyn.tagi_loc = mbr_value;
}	

void	DynMbrQuliHandler (mbr_value)
int	mbr_value;
{
	tmp_edit_dyn_rcd.lnk.dyn.qi_loc = mbr_value;
}	

void	DynMbrDataClassHandler (int	mbr_value)
{
	tmp_edit_dyn_rcd.lnk.dyn.data_class = mbr_value;
}	


void	DynMbrDirHandler (int	mbr_value)
{
	tmp_edit_dyn_rcd.lnk.dyn.fmt_def.bar_graph.direction = mbr_value;
}	
void   DynMbrVibraHandler (int	mbr_value)
{
	tmp_edit_dyn_rcd.lnk.dyn.fmt_def.vibra_area.vibra_serial = mbr_value;
}
void	DynProgSchedHandler (option)
int	option;
{
									   
}

void	DynProgStatusHandler (char	*tag)
{
	strcpy (tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.status, tag);
}

void	DynRptPeriodHandler (int	period_type)
{
	tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.req_type = (short)period_type;
}

void	DynRptSpecialHandler (int	special_type)
{
	tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.special_type = (short)special_type;
}
// added  chichi
void	DynPtStateHandler (special_type)
int	special_type;
{
	tmp_edit_dyn_rcd.lnk.state = (char)special_type;
}

void	DynTmpCharaHandler (int	special_type)
{
	tmp_edit_dyn_rcd.lnk.dyn.fmt_def.temp_chara.temp_chara_id   = (short)special_type;
}


void	DynRptSignHandler (sign_type)
int	sign_type;
{
	tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.sign_type = (short)sign_type;
}

void DynMemberHander(int mbr_type, int item_value)
{ 
	char item_name[20];  //OPTION_NAME_LEN  chichi

	switch(mbr_type)
	{
		case	MBR_TYPE_DIO:
			DynMbrDioHandler(item_value);
			break;

		case	MBR_TYPE_TAG:
			DynMbrTagHandler(item_value);
			break;

		case	MBR_TYPE_QULI:
			DynMbrQuliHandler(item_value);
			break;

		case	MBR_TYPE_DATA_CLASS:
		case	MBR_TYPE_IMP_CLASS:
		case	MBR_TYPE_IMP_TIME:
			DynMbrDataClassHandler(item_value);
			break;

		case	MBR_TYPE_DIR:
			DynMbrDirHandler(item_value);
			break;
        /*   add for v40 by chichi */

        case    MBR_TYPE_V_A_S:
			DynMbrVibraHandler(item_value);
			break;

		case MBR_TYPE_SCHED:
			DynProgSchedHandler(item_value);
			break;

		case	MBR_TYPE_STATUS:

			if (tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id>=0   &&  tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id<=MAX_PROG )
				strcpy(item_name,Cmd_Mode[tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id][item_value]);
			else    
				strcpy(item_name, "");


			DynProgStatusHandler(item_name);
			break;

		case	MBR_TYPE_REQ:
			DynProgSchedHandler(item_value);
			break;

		case	MBR_TYPE_RPT_PERIOD:
			DynRptPeriodHandler(item_value);
			break;		
			
		case	MBR_TYPE_RPT_SIGN:
			DynRptSignHandler(item_value); 
			break;		

		case	MBR_TYPE_RPT_SPECIAL:
			DynRptSpecialHandler(item_value);
			break;
		/* following add for v40 by chichi */
		case	MBR_TYPE_PT_STATE:
			DynPtStateHandler(item_value);
			break;
		case	MBR_TYPE_TMP_CHARA:
			DynTmpCharaHandler(item_value);
			break;	
			
		default:	
			break;
	}
	return;
}


void	DynProgHandler (prog_id)
int	prog_id;
{
	tmp_edit_dyn_rcd.lnk.act.ctrl_type = CTRL_REQ_PROG;
	tmp_edit_dyn_rcd.lnk.act.ctrl_tag = CTRL_TAG_ON;
	tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id  =  prog_id;
//	if( tmp_edit_dyn_rcd.lnk.dyn.dyn_tag == DYN_TAG_ON && tmp_edit_dyn_rcd.lnk.dyn.dyn_type == DF_PROG_REQ-1)
	if( tmp_edit_dyn_rcd.lnk.dyn.dyn_tag == DYN_TAG_ON && tmp_edit_dyn_rcd.lnk.dyn.dyn_type == DF_PROG_REQ)
	{
		tmp_edit_dyn_rcd.lnk.dyn.dyn_tag = DYN_TAG_OFF;
		tmp_edit_dyn_rcd.lnk.dyn.dyn_type = 0;
	}
}

void	DynCtrlMenuHandler ( menu_id)
int	menu_id;
{	
	tmp_edit_dyn_rcd.lnk.act.ctrl_type = CTRL_REQ_MENU; 
	tmp_edit_dyn_rcd.lnk.act.ctrl_tag = CTRL_TAG_ON;
	tmp_edit_dyn_rcd.lnk.act.fmt_def.menu.id = menu_id;
	strcpy(tmp_edit_dyn_rcd.lnk.act.fmt_def.menu.name,v4_menudb.menu[menu_id].name);  //add chi  chi


	if( tmp_edit_dyn_rcd.lnk.dyn.dyn_tag == DYN_TAG_ON && tmp_edit_dyn_rcd.lnk.dyn.dyn_type == DF_MENU_REQ)
	{
		tmp_edit_dyn_rcd.lnk.dyn.dyn_tag = DYN_TAG_OFF;
		tmp_edit_dyn_rcd.lnk.dyn.dyn_type = 0;
	}

}


void	DynSymbolGrpOptHandler (symb_grp_id)
int	symb_grp_id;
{
	switch( dyn_format)
	{
	case	DF_SYMBOL_GRP:
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.sym.sym_grp  = symb_grp_id; 
		break;
	default:
		break;	
	}
}

void	DynStringGrpOptHandler (str_grp_id)
int	str_grp_id;
{
	switch( dyn_format)
	{
	case	DF_STRING_GRP:
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.str.str_grp  = str_grp_id; 
		break;
	default:
		break;	
	}

}


void	DynCgrpOptHandler (int	cgrp_id)
{
	tmp_edit_dyn_rcd.lnk.cgrp.cgrp_id		= cgrp_id;
	switch( dyn_format)
	{
	case	DF_ADD_COLOR:
		tmp_edit_dyn_rcd.lnk.cgrp.tag		= DYN_CGRP_TAG_ADD_ON;
		break;
	default:
		tmp_edit_dyn_rcd.lnk.cgrp.tag		= DYN_CGRP_TAG_ON;
		break;
	}
}


void	DynRptHourIDOptHandler (int time_id)
{

	switch( dyn_format)
	{
	case	DF_RPT_NUMERIC:
		if( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.req_type == REQ_HOURLY)
		{
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.hour_id =(short)time_id;
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.date_id =(short)TIME_NULL ;
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.month_id = (short) TIME_NULL;
		}
		break;

	case	DF_HIST_BAR:
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.hist_bar.hour_id =(short)time_id;
		break;

	default:
		break;
	}
}


void	DynRptDateIDOptHandler (time_id)
int	time_id;
{
	switch( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.req_type)
	{
	case	REQ_DAYLY:
	case	REQ_MONTHLY:	/* for monthly table, only date_id is valid for it . 96.11.4*/ 
	case	REQ_YEARLY:
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.date_id =(short) time_id;
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.hour_id = (short) TIME_NULL;
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.month_id = (short) TIME_NULL;
		break;

	default:
		break;
	}
}

void	DynRptMonthIDOptHandler (time_id)
int	time_id;
{
	switch( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.req_type)
	{
	case	REQ_MONTHLY:
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.month_id = (short) time_id;
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.date_id =(short) TIME_NULL;
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.hour_id = (short) TIME_NULL;
		break;
	default:
		break;
	}
}

void DynGroupHander(int grp_type, int item_num)
{
	switch(grp_type)
	{
		case	GRP_TYPE_COLOR:
			DynCgrpOptHandler(item_num);
			break;

		case	GRP_TYPE_SYMBOL:
			DynSymbolGrpOptHandler(item_num);
			break;

		case	GRP_TYPE_STRING:
			DynStringGrpOptHandler(item_num);
			break;

		case	GRP_TYPE_CTRL_MENU:
			DynCtrlMenuHandler(item_num);
			break;

		case	GRP_TYPE_CTRL_PROG:
			DynProgHandler(item_num);				
			break;

		case	GRP_TYPE_RPT_HOUR_ID:
			DynRptHourIDOptHandler(item_num);
			break;

		case	GRP_TYPE_RPT_DATE_ID:
			DynRptDateIDOptHandler(item_num);
			break;

		case	GRP_TYPE_RPT_MONTH_ID:
			DynRptMonthIDOptHandler(item_num);
			break;
		
		default:
			break;	
	}
	return;
}


void	NoDynamicsHandler (hwnd, menu_id)
HWND	hwnd;
int		menu_id;
{
	
	if (MessageBox(hwnd, "Delete Dynamic Request?", "IPM", MB_YESNOCANCEL) == IDYES)
	{
		DeleteReqHandler(menu_id);
	}
	else
		return;

}

void	DeleteReqHandler ( menu_id)
int		menu_id;
{
	
	int	i;
	NEW_GBRCD	*dynrcd;

	for (i=0; i<v4_gdbase.nextpos; i++)
	{
		if (v4_gdbase.buffer[i].bck.tag == ITEM_TAG_ON)
		{
			dynrcd = &v4_gdbase.buffer[i];
			break;
		}
	}
	
	switch	( menu_id)
	{
 		case	ID_Del_Dynamics:   // dynamic request
			dynrcd->lnk.dyn.dyn_tag = DYN_TAG_OFF;
			dynrcd->lnk.dyn.dyn_type = 0;
			 strcpy(dynrcd->lnk.dbname , "");



			break;

 		case	ID_Del_Req:	     // ctrl request
			dynrcd->lnk.act.ctrl_tag = CTRL_TAG_OFF;
			dynrcd->lnk.act.ctrl_type = 0;
			break;

 		case	ID_Del_Dyn_Color:
			dynrcd->lnk.cgrp.tag = DYN_CGRP_TAG_OFF;
			dynrcd->lnk.cgrp.cgrp_id = 0;
			break;

		default:
			break;
	}	
	
}


