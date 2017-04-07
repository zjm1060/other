
#define	PROG_DEF

#include	<stdio.h>

#include	"../src/resource.h"

#include	"../inc/ipm.h"
#include	"../inc/color_grp_def.h"
#include	"../inc/draw_def.h"
#include	"../inc/menu_def.h"
#include	"../inc/string_grp_def.h"
#include	"../inc/symbol_grp_def.h"
#include	"../inc/tmplt.h"
#include	"../inc/dyn_form_def.h"
#include	"../inc/dyn_prog_def.h"
#include	"../inc/field_def.h"

void 	DynGroupHander(int, int);
void 	DynMemberHander(int, int);
void	DynTextHandler ( HWND, int);
void ClearItemTag();
void CreateTmpltWgts( HWND, TMPLT *);
void CreateItemOptWgt( HWND, TMPLT *, int);
HWND CreateOptButton (HWND, GBFLD *);
HWND CreateMbrButton( HWND, GBFLD *, int); 
void CheckFldType( char *, int *);
void CheckGrpType( char *, int *); 
void CheckMbrType( char *, int *);
void CheckTextReqType( char *, int *); 
int GetCurMbrItemValue(int, int ); 
void CreateTextItemWgt( HWND, GBFLD *);	
void SetDynamicButton ( );
void CreateReqBtn(HWND, int);
BOOL CALLBACK DynTemplateDlgProc(HWND, UINT, WPARAM, LONG);
BOOL CenterWindow (HWND , HWND );
BOOL CenterAndResizeWindow (HWND , HWND , int , int );
void SetMbrCboList (HWND hcbo,int type, int items);
void CheckMbrDefPos( char *fld[], char *item_name, int *def_pos);


#define	HOUR_MIN	0
#define	HOUR_MAX	23
#define	DATE_MIN	0	
#define	DATE_MAX	31
#define	MONTH_MIN	0	
#define	MONTH_MAX	12

#define	REQ_TAG_ON	1
#define	REQ_TAG_OFF	0

#define	GRP_COMBO_ID_START 30000
#define	MBR_COMBO_ID_START 35000
#define	EDIT_ID_START	   39000
#define MIN_TMPLT_ID		30000  //resource id 30000-39999 used by tmplt 
#define MAX_TMPLT_ID		39999

NEW_GBRCD	*pCurrSelectedRcd;	  // add/modify  dyn/ctrl_act/colorgrp rcd
NEW_GBRCD	tmp_edit_dyn_rcd;

int	shell_height=0;	  // count Dyn Template Dlg height by add all sorts of fields
int	shell_width=0;

int	dyn_tmplt_param1, dyn_tmplt_param2, dyn_tmplt_param3;
short dyn_tmplt_param4;


NEW_GBRCD* GetFirstCurrSelectedRcd(void)
{
	int i;
	
	for (i=0; i<v4_gdbase.nextpos; i++)
	{
		if (v4_gdbase.buffer[i].bck.tag == ITEM_TAG_ON)
			return &v4_gdbase.buffer[i];
	}
	return NULL;
}

void PopupDynDlgBox (dyn_call_type, dsp_fmt_num, ctrl_type_num, add_color_type)
int	dyn_call_type;
int	dsp_fmt_num;
int	ctrl_type_num;
short	add_color_type;
{
	pCurrSelectedRcd= GetFirstCurrSelectedRcd();
	if(!pCurrSelectedRcd)
		return;
	
	dyn_tmplt_param1 = dyn_call_type;
	dyn_tmplt_param2 = dsp_fmt_num;
	dyn_tmplt_param3 = ctrl_type_num;
	dyn_tmplt_param4 = add_color_type;
	DialogBox( ghInst, MAKEINTRESOURCE(DLG_DYN_TEMPLATE),
		ghWnd, (DLGPROC)DynTemplateDlgProc );
}

void CreateDynTemplate (hdlg, dyn_call_type, dsp_fmt_num, ctrl_type_num, add_color_type)
HWND	hdlg;
int	dyn_call_type;
int	dsp_fmt_num;
int	ctrl_type_num;
short	add_color_type;
{
	int		fld_cnt;
	//	int		i;
	int	form_type;
	int	dyn_tag;
	
	ctrl_tag = CTRL_TAG_OFF;	
	
	shell_height = 0; 

	if(!pCurrSelectedRcd)
		return;
	tmp_edit_dyn_rcd	= *pCurrSelectedRcd;
	
	if( dsp_fmt_num != -1) 
	{ 
		cur_tmplt = tmplt_buff.tmplt[dsp_fmt_num];
		fld_cnt	= cur_tmplt.fld_cnt;
		dyn_format = cur_tmplt.format;
		
		switch ( dyn_call_type)
		{
		case	CREATE_DYN_TYPE:
			dyn_tag	= tmp_edit_dyn_rcd.lnk.dyn.dyn_tag;
			if ( dyn_tag == DYN_TAG_ON )
				SetDynamicButton ();
			tmp_edit_dyn_rcd.lnk.dyn.dyn_type = dyn_format; 		//from 1 beginning
			if ( dyn_format != 0 && dyn_format != DF_POINT_NAME)	//DF_POINT_NAME	only input db_ptr
			{
				tmp_edit_dyn_rcd.lnk.dyn.dyn_tag  = DYN_TAG_ON;
			}
			tmp_edit_dyn_rcd.lnk.act.ctrl_tag = CTRL_TAG_OFF;
			tmp_edit_dyn_rcd.lnk.cgrp.tag = DYN_CGRP_TAG_OFF;
			form_type = FORM_WGT_NORMAL;
			break;
			
		case	MODIFY_DYN_TYPE:
			break;
			
		case	ADD_CTRL_TYPE:
			if( dsp_fmt_num == DF_ADD_COLOR-1 )
			{
				tmp_edit_dyn_rcd.lnk.cgrp.tag		= DYN_CGRP_TAG_ADD_ON;
			}
			else
			{
				
				tmp_edit_dyn_rcd.lnk.act.ctrl_tag	= CTRL_TAG_ON;
			}
			break;
			
		default:
			break;
		}
		CreateTmpltWgts( hdlg, &cur_tmplt );	
	}
	else if( dsp_fmt_num == -1)	 
		/* this is only for the case of  dyn_format = NULL, only have act or color: CTRL+doubleclick, used display or modify*/
		fld_cnt = 0;
	
	shell_height += 10;
	switch (ctrl_type_num)
	{
	case	CTRL_REQ_DSP:
		ctrl_tag = CTRL_TAG_ON;	
		dsp_fmt_num	= DF_DSP_REQ;
		CreateReqBtn ( hdlg, dsp_fmt_num );
		break;
		
	case	CTRL_REQ_OPER_GUIDE:
		ctrl_tag = CTRL_TAG_ON;	
		dsp_fmt_num	= DF_OPER_GUIDE_REQ;
		CreateReqBtn ( hdlg, dsp_fmt_num );
		break;
		
		
	case	CTRL_REQ_PROG:
		ctrl_tag = CTRL_TAG_ON;	
		dsp_fmt_num	= DF_PROG_REQ;
		CreateReqBtn ( hdlg, dsp_fmt_num );
		break;
		
	case	CTRL_REQ_MENU:
		ctrl_tag = CTRL_TAG_ON;	
		dsp_fmt_num	= DF_MENU_REQ;
		CreateReqBtn ( hdlg, dsp_fmt_num );
		break;
		
	case	CTRL_REQ_NULL:
		break;
		
	default:
		MessageBox(hdlg, "CTRL_REQ Type Error.", "IPM", MB_OK);
		break;
	}
	if(  add_color_type == HAVE_ADD_COLOR )
	{
		ctrl_tag = CTRL_TAG_ON;
		dsp_fmt_num	= DF_ADD_COLOR;
		CreateReqBtn ( hdlg, dsp_fmt_num );
	}
	
}

void AddDlgTmplt(TMPLT *tmplt)	// add tmplt to cur_tmplt
{
	int i, cur_dlg_fld_cnt;
	
	cur_dlg_fld_cnt = cur_tmplt.fld_cnt;
	for (i=0; i<tmplt->fld_cnt; i++)
	{
		cur_tmplt.fld_buff[cur_dlg_fld_cnt] = tmplt->fld_buff[i];
		cur_dlg_fld_cnt++;
	}
	cur_tmplt.fld_cnt = cur_dlg_fld_cnt;
}

void CreateReqBtn(parent, dsp_fmt)
HWND	parent;
int	dsp_fmt;
{
	TMPLT	tmplt;
	
	//	cur_tmplt = tmplt_buff.tmplt[dsp_fmt-1];
	tmplt = tmplt_buff.tmplt[dsp_fmt-1];
	AddDlgTmplt(&tmplt);
	CreateTmpltWgts( parent, &tmplt);
}	


void CreateTmpltWgts( hwnd, tmplt )
HWND	hwnd;
TMPLT	*tmplt;
{
	FLD_DEF	cur_fld;
	int	i;
	int	fld_cnt;
	int	fld_type;
	char	fld_name[FLD_NAME_LEN];
	
	fld_cnt = tmplt->fld_cnt;
	for(i=0; i<fld_cnt; i++)
	{
		cur_fld = tmplt->fld_buff[i].fld_def;
		CheckFldType( tmplt->fld_buff[i].fld_type_name, &fld_type) ;
		strcpy( fld_name, tmplt->fld_buff[i].fld_name);
		switch (fld_type)
		{
		case	FLD_TYPE_OPT_GRP:
		case	FLD_TYPE_OPT_MBR:
			CreateItemOptWgt ( hwnd, tmplt, i);
			break;
		case	FLD_TYPE_TEXT:
			CreateTextItemWgt( hwnd, &(tmplt->fld_buff[i]) ); 
			break;
			
			/*		case	FLD_TYPE_BOOL:
			break;
			*/		default:
			break;
		}
	}	
}

void CreateItemOptWgt( parent, tmplt, wgt_num)
HWND	parent;
TMPLT	*tmplt;
int	wgt_num;
{
	int	 fld_type, opt_mbr_type;
	char	fld_name[FLD_NAME_LEN]= {"NULL"};
	SIZE	size;
	RECT	textrc, comborc;
	HDC	hdc;
	HWND	htext, hcombo;
	GBFLD	fld;
	
	fld = tmplt->fld_buff[wgt_num];
	strcpy( fld_name, fld.fld_name);
	CheckFldType( fld.fld_type_name, &fld_type) ;
	
	shell_width = fld.x;
	shell_height += fld.y;
	hdc = GetDC(parent);
	GetTextExtentPoint(hdc, fld_name, sizeof(fld_name), &size);
	ReleaseDC(parent, hdc);
	htext = CreateWindow("STATIC",fld_name,	WS_CHILD | WS_VISIBLE,
		shell_width,shell_height,size.cx,size.cy,parent,(HMENU)NULL,ghInst,NULL);
	
	GetClientRect(htext, &textrc);
	shell_width += textrc.right;
	
	switch( fld_type )
	{
	case	FLD_TYPE_OPT_GRP:
		hcombo = CreateOptButton( parent, &fld);
		break;
		
	case	FLD_TYPE_OPT_MBR:
		CheckMbrType( fld_name, &opt_mbr_type) ;
		hcombo = CreateMbrButton( parent, &fld, opt_mbr_type); 
		break;
	default:
		break;
	}
	
	if(hcombo) GetClientRect(hcombo, &comborc);
	shell_width += comborc.right;
	shell_height +=  max(textrc.bottom, comborc.bottom);
	
}		


HWND CreateMbrButton( parent, fld,  mbr_type) 
HWND	parent;
GBFLD	*fld;
int	mbr_type;
{
	int	i, list_cnt;
	int	default_pos, combo_id, y, width;
	HWND	hWndCombox;
	
	if (parent == NULL) return NULL;
	
	default_pos = 0;
	
	combo_id = MBR_COMBO_ID_START + mbr_type;
	y = fld->y;
	//	width = fld->width - shell_width;
	width = fld->width;
	
	hWndCombox = CreateWindow("COMBOBOX",NULL,
		CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
		shell_width,shell_height,width,200,parent,(HMENU)combo_id,ghInst,NULL);
	SendMessage(hWndCombox,CB_RESETCONTENT,0,0 );
	list_cnt = fld->fld_def.opt_mbr_fld.list_cnt;
	if (mbr_type == 15 )
	{
		if (tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id>=0 && tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id<=MAX_PROG)
		{
			/*for (i=0 ;Cmd_Mode[tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id -13][i]!=NULL; i++ ) zyp 4.18*/
			for (i=0 ;Cmd_Mode[tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id ][i]!=NULL; i++ )
				SendMessage(hWndCombox,CB_ADDSTRING,0,(LPARAM)(LPSTR)Cmd_Mode[tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id /*-13*/][i] );
		}else
			SendMessage(hWndCombox,CB_ADDSTRING,0,(LPARAM)(LPSTR)"NULL" );
		
	}
	else
	{
		for (i=0 ; i < list_cnt; i++)
		{
			SendMessage(hWndCombox,CB_ADDSTRING,0,(LPARAM)(LPSTR)fld->fld_def.opt_mbr_fld.list_name[i] );
		}
	}
	switch( mbr_type)
	{
	case	MBR_TYPE_DIO:
		if( tmp_edit_dyn_rcd.lnk.dyn.dio_type >= 0 &&
			tmp_edit_dyn_rcd.lnk.dyn.dio_type < list_cnt)
		{
			default_pos = tmp_edit_dyn_rcd.lnk.dyn.dio_type;
		}
		break;
	case	MBR_TYPE_TAG:
		if( tmp_edit_dyn_rcd.lnk.dyn.tagi_loc >= 0 &&
			tmp_edit_dyn_rcd.lnk.dyn.tagi_loc < list_cnt)
		{
			//default_pos = 2;//tmp_edit_dyn_rcd.lnk.dyn.tagi_loc;
			default_pos = tmp_edit_dyn_rcd.lnk.dyn.tagi_loc;	//	2000.11.2 llb
		}
		break;
	case	MBR_TYPE_QULI:
		if( tmp_edit_dyn_rcd.lnk.dyn.qi_loc >= 0 &&
			tmp_edit_dyn_rcd.lnk.dyn.qi_loc < list_cnt )
		{
			default_pos = tmp_edit_dyn_rcd.lnk.dyn.qi_loc;
		}
		break;
	case	MBR_TYPE_DATA_CLASS:
	case	MBR_TYPE_IMP_CLASS:
	case	MBR_TYPE_IMP_TIME:
		if( tmp_edit_dyn_rcd.lnk.dyn.data_class >= 0 &&
			tmp_edit_dyn_rcd.lnk.dyn.data_class < list_cnt)
		{
			default_pos = tmp_edit_dyn_rcd.lnk.dyn.data_class;
		}
		break;
	case	MBR_TYPE_DIR:
		if( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.bar_graph.direction >= 0 &&
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.bar_graph.direction < list_cnt)
		{
			default_pos = tmp_edit_dyn_rcd.lnk.dyn.fmt_def.bar_graph.direction;
		}
		break;
	case	MBR_TYPE_V_A_S:
		if( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.vibra_area.vibra_serial >= 0 &&
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.vibra_area.vibra_serial < list_cnt)
		{
			default_pos = tmp_edit_dyn_rcd.lnk.dyn.fmt_def.vibra_area.vibra_serial;
		}
		break;
		
	case	MBR_TYPE_SCHED:
		default_pos = 0;
		break;
		
	case	MBR_TYPE_STATUS:
		if ( tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.status != (char *) NULL)
		{
			
			//   &(fld->fld_def) 
			if (tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id>=1 && tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id<=MAX_PROG )
				CheckMbrDefPos(Cmd_Mode[tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id/* -13*/], tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.status, 
				&default_pos);
			if( default_pos < 0 || default_pos > list_cnt) 
				default_pos =0;
		}
		break;
		
	case	MBR_TYPE_REQ:
		default_pos = 0;
		break;
		
	case	MBR_TYPE_RPT_PERIOD:
		default_pos = tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.req_type;
		break;		
		
		
	case	MBR_TYPE_RPT_SIGN:
		default_pos = tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.sign_type;
		break;		
		
		
	case	MBR_TYPE_RPT_SPECIAL:
		default_pos = tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.special_type;
		break;	
		/*  follow added for v40  by chichi  */			
	case	MBR_TYPE_PT_STATE:
		default_pos = tmp_edit_dyn_rcd.lnk.state;
		break;
	case    MBR_TYPE_TMP_CHARA:
		default_pos = tmp_edit_dyn_rcd.lnk.dyn.fmt_def.temp_chara.temp_chara_id;
		break;
		
	default:
		break;
	}
	
	SendMessage(hWndCombox,CB_SETCURSEL,default_pos,0 );
	return(hWndCombox);
}

HWND	dyn_db_point_name_win, dyn_dspfile_name_win;

void CreateTextItemWgt( parent, fld)	
HWND	parent;
GBFLD	*fld;
{
	char	old_string[20];
	int		req_type, edit_id, width;
	int		wgt_height=0;
	char	fld_name[FLD_NAME_LEN];
	HWND	htext, hEdit;
	HDC	hdc;
	SIZE	size;
	RECT	textrc, editrc;
	int		button_x, button_y;
	
	strcpy( fld_name, fld->fld_name);
	CheckTextReqType( fld_name, &req_type);	
	
	shell_width = fld->x;
	shell_height += fld->y;
	hdc = GetDC(parent);
	GetTextExtentPoint(hdc, fld_name, sizeof(fld_name), &size);
	ReleaseDC(parent, hdc);
	htext = CreateWindow("STATIC",fld_name,	WS_CHILD | WS_VISIBLE,
		shell_width,shell_height,size.cx,size.cy,parent,(HMENU)NULL,ghInst,NULL);
	
	GetClientRect(htext, &textrc);
	shell_width += textrc.right;
	
	edit_id = EDIT_ID_START + req_type;
	//	width = fld->width - shell_width;
	width = fld->width;
	hEdit = CreateWindow("EDIT",NULL, WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL,
		shell_width,shell_height,width,30,parent,(HMENU)edit_id,ghInst,NULL);
	
	button_x= shell_width+width+20;
	button_y= shell_height;
	
	GetClientRect(hEdit, &editrc);
	shell_width += editrc.right;
	shell_height +=  max(textrc.bottom, editrc.bottom);
	
	switch ( req_type)
	{
	case	DB_ENTRY:
		if ( tmp_edit_dyn_rcd.lnk.dbname[0] != '\0')
		{ 
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)tmp_edit_dyn_rcd.lnk.dbname);
		}
		CreateWindow("BUTTON","选择", WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,
			button_x,button_y,50,30,parent,(HMENU)IDC_DB_SELECT_BUTTON,ghInst,NULL);
		dyn_db_point_name_win= hEdit;
		break;
   	case	DB_ENTRY_V_L:
		if ( tmp_edit_dyn_rcd.lnk.dbname[0] != '\0')
		{               
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)tmp_edit_dyn_rcd.lnk.dyn.fmt_def.vibra_area.dbname2 );
		}
		break;
		
	case	DSP_ENTRY:
		if ( tmp_edit_dyn_rcd.lnk.act.fmt_def.dsp.name[0] != '\0')
		{
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)tmp_edit_dyn_rcd.lnk.act.fmt_def.dsp.name);
		}
		CreateWindow("BUTTON","选择", WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,
			button_x,button_y,50,30,parent,(HMENU)IDC_DSPFILE_SELECT_BUTTON,ghInst,NULL);
		dyn_dspfile_name_win= hEdit;
		break;
		
	case	EQUAT_ENTRY:
		if ( tmp_edit_dyn_rcd.lnk.act.fmt_def.dsp.name[0] != '\0')
		{
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)tmp_edit_dyn_rcd.lnk.act.fmt_def.dsp.name);
		}
		break;
		
	case	HIGH_LIMIT:
		//		if ( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.bar_graph.hi_limit != 0)
		{
			sprintf ( old_string,"%*.1f\0", strlen(old_string),
				tmp_edit_dyn_rcd.lnk.dyn.fmt_def.bar_graph.hi_limit);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)old_string);
		}
		break;
		
	case	LOW_LIMIT:
		//		if ( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.bar_graph.low_limit != 0)
		{
			sprintf ( old_string, "%*.1f\0", strlen(old_string), 
				tmp_edit_dyn_rcd.lnk.dyn.fmt_def.bar_graph.low_limit);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)old_string);
		}
		break;
		/*	case	HIGH_LIMIT_V:
		{
		sprintf ( old_string,"%*.1f\0", strlen(old_string),
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.vibra_area.hi_limit);
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)old_string);
		}
		break;
		case	LOW_LIMIT_V:
		{
		sprintf ( old_string,"%*.1f\0", strlen(old_string),
		tmp_edit_dyn_rcd.lnk.dyn.fmt_def.vibra_area.low_limit);
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)old_string);
		}
		break;
		*/
	case	F_B_OFF:
		{
			sprintf ( old_string, "%*d\0",strlen(old_string), tmp_edit_dyn_rcd.lnk.dyn.f_by_off);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)old_string);
		}
		break;
		
	case	DEC_NUM:  //chichi
		//		if ( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.dec_pt != 0)
		{
			sprintf( old_string, "%*d\0", strlen(old_string), tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.dec_pt);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)old_string);
		}
		break;
		
	case	DATA_LEN:
		//		if ( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.length != 0)
		{
			sprintf( old_string, "%*d\0", strlen(old_string), tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.length);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)old_string);
		}
		break;
    case	DATA_LEN_TEMP:
		//		if ( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.length != 0)
		{
			sprintf( old_string, "%*d\0", strlen(old_string), tmp_edit_dyn_rcd.lnk.dyn.fmt_def.temp_chara.length);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)old_string);
		}
		break;
	case    CURVE_PT_NUM:
		//		if ( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.curve_point.point_id != 0)
		{
			sprintf( old_string, "%*d\0", strlen(old_string), tmp_edit_dyn_rcd.lnk.dyn.fmt_def.curve_point.point_id);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)old_string);
		}
		break ;
    case	LOAD_CUR_HI:
		sprintf( old_string, "%*.1f\0", strlen(old_string), tmp_edit_dyn_rcd.lnk.dyn.fmt_def.load_curve.hi_limit);
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)old_string);
		break;
		
    case    LOAD_CUR_LOW:
		sprintf( old_string, "%*.1f\0", strlen(old_string), tmp_edit_dyn_rcd.lnk.dyn.fmt_def.load_curve.low_limit);
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)old_string);
		break;
	case DB_ENTRY_R_C:   
		if ( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.load_curve.LoadDBName[0] != 0 )
		{ 
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)tmp_edit_dyn_rcd.lnk.dyn.fmt_def.load_curve.LoadDBName);
		}

/*		CreateWindow("BUTTON","选择", WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,
			button_x,button_y,50,30,parent,(HMENU)IDC_DB_SELECT_BUTTON,ghInst,NULL);
	    dyn_db_point_name_win= hEdit;  */
		break; 
		
	case START_TIME:  
		if  (cur_tmplt.format == DF_SCENE_REDISP )   
		{			  
			if ( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.scene_redisp.StartTime[0]   != 0 )
			{ 
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)tmp_edit_dyn_rcd.lnk.dyn.fmt_def.scene_redisp.StartTime);
			}
		}
		if  (cur_tmplt.format == DF_REAL_CURVE )   
		{			
			if ( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.real_curve.StartTime[0]   != 0 )
			{ 
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)tmp_edit_dyn_rcd.lnk.dyn.fmt_def.real_curve.StartTime);
			}
		}
		break;
		
	case DSP_FILE_NM:   
		if ( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.scene_redisp.ReDspFileNm[0]   != 0 )
		{ 
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)tmp_edit_dyn_rcd.lnk.dyn.fmt_def.scene_redisp.ReDspFileNm);
		}
		break;
		
	case    TIME_LENGTH:
		if  (cur_tmplt.format == DF_SCENE_REDISP )   
		{	
			sprintf( old_string, "%*d\0", strlen(old_string), tmp_edit_dyn_rcd.lnk.dyn.fmt_def.scene_redisp.TimeLength  );
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)old_string);
		}
		if  (cur_tmplt.format == DF_REAL_CURVE )   
		{
			sprintf( old_string, "%*d\0", strlen(old_string), tmp_edit_dyn_rcd.lnk.dyn.fmt_def.real_curve.TimeLength  );
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)old_string);

		}
		break;
		
	default:
		MessageBox(ghWnd,"wrong text_req_type!", "IPM", MB_OK);
		break;
	}
	
}
// added by chi chi ;;
int	SearchMenuId ( char	menu_name[MENU_NAME_LEN])
{
	int	i;
	
	for( i=0; i < v4_menudb.nextpos; i++)
	{
		if( strcmp( v4_menudb.menu[i].name , menu_name) == 0)
		{
			return(i);
			break;
		}
	} 
	return -1 ;
}


HWND CreateOptButton (parent, fld )
HWND	parent;
GBFLD	*fld;
{
	int	i;
	int	default_pos;
	char	*minstr ="min";
	char	*maxstr = "max";
	char	date[3];	
	
	int	fld_type;
	int	grp_type;
	char	fld_name[FLD_NAME_LEN];
	int	combo_id;
	int	y, width;
	HWND	hWndCombox,hcombo;
	
	strcpy( fld_name, fld->fld_name);
	CheckFldType(fld->fld_type_name, &fld_type) ;
	CheckGrpType( fld_name, &grp_type) ;
	default_pos = 0;
	
	combo_id = GRP_COMBO_ID_START + grp_type;
	y = fld->y;
	//	width = fld->width - shell_width;
	width = fld->width;
	
	hWndCombox = CreateWindow("COMBOBOX",NULL,
		CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
		shell_width,shell_height,width,200,parent,(HMENU)combo_id,ghInst,NULL);
	SendMessage(hWndCombox,CB_RESETCONTENT,0,0 );
	
	switch(grp_type)
	{
	case	GRP_TYPE_COLOR:
		if( tmp_edit_dyn_rcd.lnk.cgrp.cgrp_id >= 0 &&
			tmp_edit_dyn_rcd.lnk.cgrp.cgrp_id < cgdb_cnt )
		{
			default_pos = tmp_edit_dyn_rcd.lnk.cgrp.cgrp_id;
		}
		for (i = 0; i < cgdb_cnt; i++)
		{
			SendMessage(hWndCombox,CB_ADDSTRING,0,(LPARAM)(LPSTR)cgdb[i].name );
		}	
		break;
		
	case	GRP_TYPE_SYMBOL:
		if( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.sym.sym_grp >=0 &&
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.sym.sym_grp < symgdb_cnt )
		{
			default_pos = tmp_edit_dyn_rcd.lnk.dyn.fmt_def.sym.sym_grp;
		}
		for (i = 0; i < symgdb_cnt; i++)
		{
			SendMessage(hWndCombox,CB_ADDSTRING,0,(LPARAM)(LPSTR)symgdb[i].name );
		}
		break;
		
	case	GRP_TYPE_STRING:
		if( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.str.str_grp >=0 &&
			tmp_edit_dyn_rcd.lnk.dyn.fmt_def.str.str_grp < strg_cnt )
		{
			default_pos = tmp_edit_dyn_rcd.lnk.dyn.fmt_def.str.str_grp;
		}
		for (i = 0; i < strg_cnt; i++)
		{
			SendMessage(hWndCombox,CB_ADDSTRING,0,(LPARAM)(LPSTR)strgdb[i].name );
		}
		break;
		
	case	GRP_TYPE_CTRL_MENU:
	/*		if( tmp_edit_dyn_rcd.lnk.act.fmt_def.menu.id >= 0 &&
		tmp_edit_dyn_rcd.lnk.act.fmt_def.menu.id < v4_menudb.nextpos)  */
		{
			default_pos = tmp_edit_dyn_rcd.lnk.act.fmt_def.menu.id;
			if ( strcmp(tmp_edit_dyn_rcd.lnk.act.fmt_def.menu.name,"") != 0 )
			{
				default_pos =	SearchMenuId ( tmp_edit_dyn_rcd.lnk.act.fmt_def.menu.name) ;
			}
		}		
		for (i = 0; i < v4_menudb.nextpos; i++)
		{
			SendMessage(hWndCombox,CB_ADDSTRING,0,(LPARAM)(LPSTR)v4_menudb.menu[i].name );
		}
		break;	
		
	case	GRP_TYPE_CTRL_PROG:
		for (i = 0; i < MAX_PROG; i++)
		{
			SendMessage(hWndCombox,CB_ADDSTRING,0,(LPARAM)(LPSTR)prog_names[i] );
		}
		if( tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id > PrgN_NULL &&
			tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id < MAX_PROG)
		{
			default_pos = tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id;
			hcombo = GetDlgItem(parent, MBR_COMBO_ID_START +15  );
			if (hcombo != NULL)
				SetMbrCboList (hcombo, 4, default_pos);
		}
		break;
		
	case	GRP_TYPE_RPT_HOUR_ID:
		if( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.hour_id !=(short)TIME_NULL)
		{
			default_pos = tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.hour_id;
		}
		for ( i= HOUR_MIN; i <= HOUR_MAX; i++)
		{
			sprintf( date, "%d\0",i  );
			SendMessage(hWndCombox,CB_ADDSTRING,0,(LPARAM)(LPSTR)date );
		}
		SendMessage(hWndCombox,CB_ADDSTRING,0,(LPARAM)(LPSTR)minstr );
		SendMessage(hWndCombox,CB_ADDSTRING,0,(LPARAM)(LPSTR)maxstr );
		break;
		
	case	GRP_TYPE_RPT_DATE_ID:
		if( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.date_id != (short)TIME_NULL)
		{
			default_pos = tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.date_id;
		}
		for (i= DATE_MIN; i <= DATE_MAX ; i++)
		{
			sprintf( date, "%d\0",i  );
			SendMessage(hWndCombox,CB_ADDSTRING,0,(LPARAM)(LPSTR)date );
		}
		break;
		
		
	case	GRP_TYPE_RPT_MONTH_ID:
		if( tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.month_id !=(short) TIME_NULL)
		{
			default_pos = tmp_edit_dyn_rcd.lnk.dyn.fmt_def.rpt_num.month_id;
		}
		for (i= MONTH_MIN; i <= MONTH_MAX ; i++)
		{
			sprintf( date, "%d\0",i  );
			SendMessage(hWndCombox,CB_ADDSTRING,0,(LPARAM)(LPSTR)date );
		}
		break;
		
	default:
		break;	
	}
	
	SendMessage(hWndCombox,CB_SETCURSEL,default_pos,0 );
	
	
	return(hWndCombox);
}
//ADD  CHI 

void SetMbrCboList (HWND hcbo,int type, int items)
{    
	int i;
	if (type != 4 || hcbo ==NULL ) return ;
	if (items == 0 )
	{
		SendMessage(hcbo,CB_RESETCONTENT,0,0 );
		SendMessage(hcbo,CB_SETCURSEL,0,0 );
	}
	else if (items > 0  &&  items <= MAX_PROG)
    {
		SendMessage(hcbo,CB_RESETCONTENT,0,0 );
		
		for (i = 0 ; Cmd_Mode[items][i]!=NULL   ; i++)
		{
			SendMessage(hcbo,CB_ADDSTRING,0,(LPARAM)(LPSTR)Cmd_Mode[items][i] );
		}
		
		SendMessage(hcbo,CB_SETCURSEL,0,0 );
		
		DynMemberHander(MBR_TYPE_STATUS, 0);         
	}
	else
	{
		SendMessage(hcbo,CB_RESETCONTENT,0,0 );
		SendMessage(hcbo,CB_ADDSTRING,0,(LPARAM)(LPSTR)"NULL" );
		SendMessage(hcbo,CB_SETCURSEL,0,0 );
	}
}


BOOL APIENTRY DlgDynPointProc(HWND hDlg, UINT message, UINT wParam, LONG lParam);
BOOL DynDspFileList(HWND hWnd, char* filename);

BOOL CALLBACK DynTemplateDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LONG lParam)
{
	int	item_num, idc, type, item_value, wbtn, hbtn;
	HWND	hdlgitem ,hcombo;
	RECT	rc, rcwin;
	//	char	tmp[20];
	char	*button_titles[]={ "asd", "asdfasdf", "d1"	};	
	char	input_str[MAX_INPUT_TEXT_LEN];  //MAX_INPUT_LEN
	char	dspfile_name[256];
	
    switch(wMsg)
    {
	case WM_INITDIALOG:
		CreateDynTemplate( hDlg, dyn_tmplt_param1, dyn_tmplt_param2, dyn_tmplt_param3, dyn_tmplt_param4);
		shell_height += 20;
		wbtn = 120;	  //"OK", "CANCEL" button size
		hbtn = 30;
		GetClientRect(hDlg, &rc);
		shell_width = rc.right/2-wbtn-40;
		CreateWindow("BUTTON","OK", WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,
			shell_width,shell_height,wbtn,hbtn,hDlg,(HMENU)IDOK,ghInst,NULL);
		shell_width = rc.right/2 + 40;
		CreateWindow("BUTTON","Cancel", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
			shell_width,shell_height,wbtn,hbtn,hDlg,(HMENU)IDCANCEL,ghInst,NULL);
		shell_height = shell_height + hbtn + 50;
		GetWindowRect (hDlg, &rcwin);
		shell_width = rcwin.right - rcwin.left;
		CenterAndResizeWindow(hDlg, ghWnd, shell_width, shell_height);
		return TRUE;
		
	case WM_COMMAND:
		idc = LOWORD(wParam);
		
		if( idc >= GRP_COMBO_ID_START && idc < EDIT_ID_START )
		{
			switch( HIWORD(wParam) )
			{
			case CBN_SELCHANGE:
				hdlgitem = GetDlgItem(hDlg, idc);
				item_num = SendMessage(hdlgitem,CB_GETCURSEL,0,0);
				if( idc >= GRP_COMBO_ID_START && idc < MBR_COMBO_ID_START )
				{
					type = idc - GRP_COMBO_ID_START;
					
					DynGroupHander(type, item_num);
					hcombo = GetDlgItem(hDlg, MBR_COMBO_ID_START +15  );
					SetMbrCboList (hcombo,type,item_num);
					
					
					return TRUE;
				}
				else if( idc >=	MBR_COMBO_ID_START && idc < EDIT_ID_START)
				{
					type = idc - MBR_COMBO_ID_START;
					item_value = GetCurMbrItemValue(type, item_num);
					
					if(item_value == ERROR_MBR_ITEM_VALUE)
					{
						MessageBox(hDlg,"No Find Correct Item Value",NULL,MB_OK);
						break;
					}
					DynMemberHander(type, item_value);
					return TRUE; 
				}
				break;
			default:
				break;
			}
		}
		
		if( idc >= EDIT_ID_START && idc < MAX_TMPLT_ID )
		{
			switch( HIWORD(wParam) )
			{
			case EN_KILLFOCUS:
				type = idc - EDIT_ID_START;
				DynTextHandler ( (HWND)lParam, type);
				return TRUE;
			default:
				break;
			}
		}
		
		switch( idc )
		{
		case IDOK:
			if(dyn_db_point_name_win)
			{
				if(SendMessage(dyn_db_point_name_win, WM_GETTEXT, MAX_INPUT_TEXT_LEN, (LPARAM)input_str))
				{
					if(strlen(input_str)>=MAX_INPUT_TEXT_LEN)
						MessageBox(hDlg, "点名太长,最大50", "IPM", MB_OK);
					else					
						strcpy(tmp_edit_dyn_rcd.lnk.dbname, input_str);
				}
			}
			if(dyn_dspfile_name_win)
			{
				if(SendMessage(dyn_dspfile_name_win, WM_GETTEXT, sizeof(dspfile_name), (LPARAM)dspfile_name))
				{
					if(strlen(dspfile_name)>=DSP_SHORTNAME_LEN)
						MessageBox(hDlg, "文件名太长,最大29", "IPM", MB_OK);
					else					
						strcpy(tmp_edit_dyn_rcd.lnk.act.fmt_def.dsp.name, dspfile_name);
				}
			}
			if( strcmp(tmp_edit_dyn_rcd.lnk.dbname ,"") != 0 )
			{
				if( strchr( tmp_edit_dyn_rcd.lnk.dbname, '.') == strrchr( tmp_edit_dyn_rcd.lnk.dbname, '.') )
				{
					MessageBox (hDlg," 点名格式错误！应为: station_name.dev_name.datatype_name.entryname",NULL,MB_OK);
					EndDialog(hDlg, wParam);
					break;
				}
			}
			*pCurrSelectedRcd = tmp_edit_dyn_rcd;
			SetDynamicButton ();
		case IDCANCEL:
			ClearItemTag();	       //tag_rcd->bck.tag	= ITEM_TAG_OFF;	 clear ctrl+doubleclick rcd 
			EndDialog(hDlg, wParam);
			dyn_dspfile_name_win= 0;
			dyn_db_point_name_win= 0;
			return TRUE;
			
		case IDC_DB_SELECT_BUTTON:
			DialogBox( ghInst, MAKEINTRESOURCE(DLG_DYN_POINT),
				hDlg, (DLGPROC)DlgDynPointProc );
			return TRUE;
			
		case IDC_DSPFILE_SELECT_BUTTON:
			dspfile_name[0]= 0;
			SendMessage(dyn_dspfile_name_win, WM_GETTEXT, sizeof(dspfile_name), (LPARAM)dspfile_name);
			if(DynDspFileList(hDlg, dspfile_name))
				SendMessage(dyn_dspfile_name_win, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)dspfile_name);
			return TRUE;
			
		}
		break;
		
		case WM_CLOSE:
			ClearItemTag();			 //tag_rcd->bck.tag	= ITEM_TAG_OFF;		 	
			EndDialog(hDlg, wParam);
            return TRUE;
    }
    return FALSE;
}



void	DynamicsHandler (hwnd, menuid)
HWND	hwnd;
int		menuid;
{
	int	dsp_fmt_type;
	
	switch	(menuid)
	{
	case	ID_Add_Display:
		dsp_fmt_type = DF_DSP_REQ-1;	
		break;
		
	case	ID_Add_Prog:
		dsp_fmt_type = DF_PROG_REQ-1;
		break;
		
	case	ID_Add_CtrlMenu:
		dsp_fmt_type = DF_MENU_REQ-1;
		break;
		
	case	ID_Add_Dyn_Color:
		dsp_fmt_type = DF_ADD_COLOR-1;
		break;
		
	default:
		break;
	}
	
	//	CreateDynTemplate ( ADD_CTRL_TYPE, dsp_fmt_type, CTRL_REQ_NULL, NO_ADD_COLOR);
	PopupDynDlgBox(ADD_CTRL_TYPE, dsp_fmt_type, CTRL_REQ_NULL, NO_ADD_COLOR);
	
}






















