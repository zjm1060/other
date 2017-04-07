
#include       	<stdio.h> 

#include	"resource.h"

#include	"../inc/ipm.h"
#include	"../inc/tmplt.h"
#include	"../inc/dyn_form_def.h"
#include	"../inc/draw_def.h"
#include	"../inc/field_def.h"
#include	"../inc/scaler.h"

NEW_GBRCD	*GetActItemRcd ( HWND, int, int, int, int, NEW_GDBUFF *);
void PopupDynDlgBox (int, int, int, short);
void SXPointToXPoint (int *, int *, int *, int *);
int	SXtoX(int);
int	SYtoY(int);


void CtrlBtn1DoubleClick (HWND hwnd, UINT message, WPARAM uParam, LPARAM lParam)
{
	int	width, height, xpos, ypos;
	int	xtmp, ytmp;
	int	dyn_call_type;
	int	dyn_type;
	int	act_type;
	int	cgrp_type;
	int	cgrp_tag;
	short	add_color_type;

	xpos	= LOWORD(lParam);	
	ypos	= HIWORD(lParam);
	xpos    = (int)(xpos / xscale) ;
	ypos    = (int)(ypos / yscale) ;
	SXPointToXPoint (&xpos, &ypos, &cur.bck.rect.x1, &cur.bck.rect.y1);
	cur.bck.rect.x2	= cur.bck.rect.x1;
	cur.bck.rect.y2	= cur.bck.rect.y1;
	CheckRectCoords (cur.bck.rect, &xtmp, &ytmp, &width, &height);

	ctrl_rcd = GetActItemRcd (hwnd, xtmp, ytmp, xtmp+width, ytmp+height, &v4_gdbase);
	if( ctrl_rcd != (NEW_GBRCD *)NULL)
	{
#ifdef DEBUG
		printf("ctrl_rcd->lnk.act.ctrl_type=%d, ctrl_rcd->lnk.act.ctrl_tag=%d\n", 
				ctrl_rcd->lnk.act.ctrl_type, ctrl_rcd->lnk.act.ctrl_tag);
		printf("ctrl_rcd->lnk.dyn.dyn_type=%d, ctrl_rcd->lnk.dyn.dyn_tag=%d\n", 
				ctrl_rcd->lnk.dyn.dyn_type, ctrl_rcd->lnk.dyn.dyn_tag);
#endif
		if ( (ctrl_rcd->lnk.act.ctrl_type != CTRL_REQ_NULL && 
				ctrl_rcd->lnk.act.ctrl_tag == CTRL_TAG_ON) ||
			ctrl_rcd->lnk.dyn.dyn_tag == DYN_TAG_ON ||
			ctrl_rcd->lnk.cgrp.tag  == DYN_CGRP_TAG_ON ||
			ctrl_rcd->lnk.cgrp.tag  == DYN_CGRP_TAG_ADD_ON )
		{
			tag_rcd	 = ctrl_rcd;
			dyn_type = ctrl_rcd->lnk.dyn.dyn_type-1; 
				// if DYN_TAG_ON: dyn_type>=0; if DYN_TAG_OFF: dyn_type=-1;
			if( ctrl_rcd->lnk.act.ctrl_tag == CTRL_TAG_ON )
				act_type = ctrl_rcd->lnk.act.ctrl_type;
			else act_type = CTRL_REQ_NULL;
			cgrp_type = ctrl_rcd->lnk.cgrp.cgrp_id;
			cgrp_tag = ctrl_rcd->lnk.cgrp.tag;	
			if( cgrp_type != 0  && cgrp_tag == DYN_CGRP_TAG_ADD_ON )
				add_color_type =  HAVE_ADD_COLOR;
			else if( cgrp_type != 0  && cgrp_tag == DYN_CGRP_TAG_ON )
			{
				if( ctrl_rcd->lnk.dyn.dyn_tag == DYN_TAG_ON && dyn_type != DF_DYN_COLOR-1 )
					add_color_type =  HAVE_ADD_COLOR;
			}
			else
				add_color_type = NO_ADD_COLOR;
			dyn_call_type = MODIFY_DYN_TYPE;
			DrawHandle ( hwnd, &(ctrl_rcd->bck), 0);	
			PopupDynDlgBox( dyn_call_type, dyn_type, act_type, add_color_type);
		}
	}
	else	// no dynamic
	{
	}
}

NEW_GBRCD	*GetActItemRcd ( hwnd, x1, y1, x2, y2, gdb)
HWND	hwnd;
int	x1, y1;
int	x2, y2;
NEW_GDBUFF	*gdb;
{
	int	i;
	int	point_offset	=2;
	int	ctrl_act_found;
	int	xtmp, ytmp;
	int	width, height;
	NEW_GBRCD	*rcd;

	ctrl_act_found	=ITEM_TAG_OFF;
 
	if (( abs(x1-x2) <= point_offset || abs(y1-y2) <= point_offset))
	{
		for (i=gdb->nextpos-1 ; i >= 0; i--)
		{
			rcd	=&gdb->buffer[i];

			if (rcd->bck.layer == cur.bck.layer)
			{
				if ((rcd->lnk.act.ctrl_type != CTRL_REQ_NULL && 
					rcd->lnk.act.ctrl_tag == CTRL_TAG_ON)	
					|| (rcd->lnk.dyn.dyn_tag == DYN_TAG_ON)
					|| (rcd->lnk.cgrp.tag  == DYN_CGRP_TAG_ON)
					|| (rcd->lnk.cgrp.tag  == DYN_CGRP_TAG_ADD_ON) )
				{
					switch (rcd->bck.draw_type)
					{
					case	DrwT_TEXT:
					case	DrwT_LINE:
					case	DrwT_LINE_DASH:
					case	DrwT_ARC:
					case	DrwT_RECTANGLE:
					case	DrwT_CIRCLE:
					case	DrwT_SYMBOL:
					case	DrwT_PLINE:
						CheckRectCoords (rcd->bck.rect, &xtmp, &ytmp, &width, &height);
						if ( (x1 >= xtmp) && (x1 <= xtmp+width) &&
							(y1 >= ytmp) && (y1 <= ytmp+height))
						{
							ctrl_act_found	=ITEM_TAG_ON;
							gdb->buffer[i].bck.tag = ITEM_TAG_ON;			
						}
						break;

					default:
						MessageBox (hwnd, "GetActItemRcd: DrwType Error.", "IPM",MB_OK);
						break;
					}
				}
			}
			if (ctrl_act_found == ITEM_TAG_ON) break;
		}
	}

	if (ctrl_act_found == ITEM_TAG_ON) 
	{
		return	rcd;
	}
	else
	{
		return	(NEW_GBRCD *)NULL;
	}
}


void DynButtonInit ()
{
	int	i;

	for(i=0; i<8; i++)
		EnableMenuItem( GetMenu( ghWnd ), ID_Add_Dynamic+i, MF_BYCOMMAND | MF_GRAYED );
	DrawMenuBar( ghWnd);

}


void ClearItemTag()
{
	if( tag_rcd != ( NEW_GBRCD *)NULL)		// ctrl + double click	rcd
	{	
		tag_rcd->bck.tag = ITEM_TAG_OFF;
	}
}

void SetDynamicButton ( )
{

	int	i;
	NEW_LINKRCD	*tmprcd;

	for (i=0; i<v4_gdbase.nextpos; i++)
	{
		if (v4_gdbase.buffer[i].bck.tag == ITEM_TAG_ON)
		{
			tmprcd	=&v4_gdbase.buffer[i].lnk;	
			break;
		}
	}

	if (tmprcd->dyn.dyn_tag == DYN_TAG_ON && tmprcd->dyn.dyn_type != 0)
	{
        EnableMenuItem( GetMenu( ghWnd ), ID_Add_Dynamic, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem( GetMenu( ghWnd ), ID_Del_Dynamics, MF_BYCOMMAND | MF_ENABLED);
	}
	else	
	{
        EnableMenuItem( GetMenu( ghWnd ), ID_Add_Dynamic, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem( GetMenu( ghWnd ), ID_Del_Dynamics, MF_BYCOMMAND | MF_GRAYED);
	}

	if (tmprcd->act.ctrl_tag == CTRL_TAG_OFF )
	{
        EnableMenuItem( GetMenu( ghWnd ), ID_Add_Display,	MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem( GetMenu( ghWnd ), ID_Add_Prog,	MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem( GetMenu( ghWnd ), ID_Add_CtrlMenu,	MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem( GetMenu( ghWnd ), ID_Del_Req,	MF_BYCOMMAND | MF_GRAYED);
	}
	else if(tmprcd->act.ctrl_tag == CTRL_TAG_ON)
	{
        EnableMenuItem( GetMenu( ghWnd ), ID_Del_Req, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem( GetMenu( ghWnd ), ID_Add_Display, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem( GetMenu( ghWnd ), ID_Add_Prog, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem( GetMenu( ghWnd ), ID_Add_CtrlMenu, MF_BYCOMMAND | MF_GRAYED);
	}

	if (tmprcd->cgrp.tag == DYN_CGRP_TAG_ON || tmprcd->cgrp.tag == DYN_CGRP_TAG_ADD_ON)
	{
        EnableMenuItem( GetMenu( ghWnd ), ID_Add_Dyn_Color,	MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem( GetMenu( ghWnd ), ID_Del_Dyn_Color,	MF_BYCOMMAND | MF_ENABLED);
	}
	else
	{
        EnableMenuItem( GetMenu( ghWnd ), ID_Add_Dyn_Color,	MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem( GetMenu( ghWnd ), ID_Del_Dyn_Color,	MF_BYCOMMAND | MF_GRAYED);
	}

	DrawMenuBar( ghWnd);
}

void CheckMbrDefPos( char *fld[], char *item_name, int *def_pos)
{
	int  i;

	for(i =0 ;fld[i]!=NULL;i++)
	{
		if( strcmp( fld[i], item_name) ==0 )
		{
			*def_pos =i;
			break;
		}

	}
}

void CheckFldType( fld_type_name, fld_type)
char	fld_type_name[FLD_TYPE_NAME_LEN];
int	* fld_type;
{

	if ( strcmp( fld_type_name, "OPT_GRP") ==0 )
		{  *fld_type = FLD_TYPE_OPT_GRP; }
	else if( strcmp( fld_type_name, "OPT_MBR")== 0) 
		{ *fld_type = FLD_TYPE_OPT_MBR; }
	else if( strcmp( fld_type_name, "TEXT")== 0) 
		{ *fld_type = FLD_TYPE_TEXT;}

}

void CheckGrpType( fld_name, grp_type) 
char	fld_name[FLD_NAME_LEN];
int	*grp_type;
{

	if ( strcmp( fld_name, "Color_Grp") == 0 )
		{	*grp_type = GRP_TYPE_COLOR;}
	else if( strcmp( fld_name, "Symbol_Grp") ==0)
		{	*grp_type = GRP_TYPE_SYMBOL; }
	else if( strcmp( fld_name, "String_Grp") ==0 )
		{	*grp_type = GRP_TYPE_STRING;}
	else if( strcmp( fld_name, "Ctrl_Menu_Grp") ==0 )
		{	*grp_type = GRP_TYPE_CTRL_MENU;}
	else if( strcmp( fld_name, "Ctrl_Prog_Grp") == 0)
		{	*grp_type = GRP_TYPE_CTRL_PROG; }
	else if( strcmp( fld_name, "Rpt_Hour_ID") == 0)
		{	*grp_type = GRP_TYPE_RPT_HOUR_ID; }
	else if( strcmp( fld_name, "Rpt_Date_ID") == 0)
		{	*grp_type = GRP_TYPE_RPT_DATE_ID; }
	else if( strcmp( fld_name, "Rpt_Month_ID") == 0)
		{	*grp_type = GRP_TYPE_RPT_MONTH_ID; }
} 


void CheckTextReqType( fld_name, req_type) 
char	*fld_name;
int	*req_type;
{
	if ( strcmp( fld_name, "High_Limit") == 0)
		{ *req_type = HIGH_LIMIT;}
	else if( strcmp( fld_name, "Low_Limit")== 0 )
		{ *req_type = LOW_LIMIT;}
    else if( strcmp( fld_name, "High_Limit_V")== 0 )
		{ *req_type = HIGH_LIMIT_V;}
	else if( strcmp( fld_name, "Low_Limit_V")== 0 )
		{ *req_type = LOW_LIMIT_V;}
	else if( strcmp( fld_name, "DB_Ptr" ) ==0 )
	{*req_type = DB_ENTRY;}  
    else if( strcmp( fld_name, "DB_Ptr_V_L" ) ==0 )
	{*req_type = DB_ENTRY_V_L;} 
	else if( strcmp( fld_name, "Dsp_Name" ) ==0 )
		{*req_type = DSP_ENTRY;}
	else if( strcmp( fld_name, "Equat_Name" ) ==0 )
		{*req_type = EQUAT_ENTRY;}
	else if( strcmp( fld_name, "F_B_Off" ) ==0 )
		{*req_type = F_B_OFF;}
	else if( strcmp( fld_name, "Data_Len" ) ==0 )
		{*req_type = DATA_LEN;}
	else if( strcmp( fld_name, "Data_Len_temp" ) ==0 )
		{*req_type = DATA_LEN_TEMP;}
	else if( strcmp( fld_name, "Dec_Num" ) ==0 )
		{*req_type = DEC_NUM;}
	else if( strcmp( fld_name, "Point_Num" ) ==0 )
		{*req_type = CURVE_PT_NUM;}
	else if( strcmp( fld_name, "Curve_Hi" ) ==0 )
		{*req_type = LOAD_CUR_HI;}
	else if( strcmp( fld_name, "Curve_Low" ) ==0 )
		{*req_type = LOAD_CUR_LOW;}
	else if( strcmp( fld_name, "DB_Ptr_R_C" ) ==0 )
		{*req_type = DB_ENTRY_R_C;}
	else if( strcmp( fld_name, "Start_Time" ) ==0 )
	{*req_type = START_TIME;}      //added chi
	else if( strcmp( fld_name, "Time_Length" ) ==0 )
	{*req_type = TIME_LENGTH;}		//added chi
	else if( strcmp( fld_name, "Dsp_File_Name" ) ==0 )
	{*req_type = DSP_FILE_NM;}		//added chi
	else
	{ *req_type = ERR_TEXT_REQ_TYPE;}
			 
}

void CheckMbrType( fld_name, mbr_type) 
char	*fld_name;
int	*mbr_type;
{
    *mbr_type =-1 ;
    
	if ( strcmp( fld_name, "Data_Class") == 0 )
		{	*mbr_type = MBR_TYPE_DATA_CLASS;}
	else if( strcmp( fld_name, "I/O_Class") ==0)
		{	*mbr_type = MBR_TYPE_DIO;}
	else if( strcmp( fld_name, "Qual_Loc") ==0 )
		{	*mbr_type = MBR_TYPE_QULI;}
	else if( strcmp( fld_name, "TagPosition") ==0 )
		{	*mbr_type = MBR_TYPE_TAG;}
	else if( strcmp( fld_name, "Dir") ==0 )
		{	*mbr_type = MBR_TYPE_DIR;}
	else if( strcmp( fld_name, "Vibra_Area_Serial") ==0 )
		{	*mbr_type = MBR_TYPE_V_A_S;}
	else if( strcmp( fld_name, "Req_Type") == 0)
		{	*mbr_type = MBR_TYPE_REQ;}
	else if( strcmp( fld_name, "Sched_ID") == 0)
		{	*mbr_type = MBR_TYPE_SCHED;}
	else if( strcmp( fld_name, "Imp_Class") == 0)
		{	*mbr_type = MBR_TYPE_IMP_CLASS;}
	else if( strcmp( fld_name, "Imp_Time") == 0)
		{	*mbr_type = MBR_TYPE_IMP_TIME;}
	else if( strcmp( fld_name, "Rpt_Period_Type") == 0)
		{	*mbr_type = MBR_TYPE_RPT_PERIOD;}
	else if( strcmp( fld_name, "Rpt_Sign_Type") == 0)
		{	*mbr_type = MBR_TYPE_RPT_SIGN;}
	else if( strcmp( fld_name, "Status_ID" ) ==0 )
		{*mbr_type = MBR_TYPE_STATUS;}
	else if( strcmp( fld_name, "RPT_Special" ) ==0 )
		{*mbr_type = MBR_TYPE_RPT_SPECIAL;}
	else if( strcmp( fld_name, "PointState" ) ==0 )   //add chichi  
		{*mbr_type = MBR_TYPE_PT_STATE;}
	else if( strcmp( fld_name, "TempCharaValue" ) ==0 )   //add chichi  
		{*mbr_type = MBR_TYPE_TMP_CHARA;}

/*	else if( strcmp( fld_name, "TempCharaValue" ) ==0 )   //add chichi  
		{*mbr_type = MBR_TYPE_TMP_CHARA;}
*/
}

int GetCurMbrItemValue(int type, int item_num ) 
{
	int	i, mbr_type, item_value;
	item_value = ERROR_MBR_ITEM_VALUE;
	for( i=0; i<MAX_FILEDS; i++ )
	{

		if(strcmp(cur_tmplt.fld_buff[i].fld_type_name, "OPT_MBR")==0)
		{
			CheckMbrType(cur_tmplt.fld_buff[i].fld_name, &mbr_type);
			if( mbr_type == type)
			{
				item_value = cur_tmplt.fld_buff[i].fld_def.opt_mbr_fld.list_value[item_num];
				break;
			}
		}
	}
	return(item_value);
}

void GetCurMbrItemName(int type, int item_value, char *item_name ) 
{
	int	i, mbr_type;

	item_name[0] = '\0';
	for( i=0; i<MAX_FILEDS; i++ )
	{
		if(strcmp(cur_tmplt.fld_buff[i].fld_type_name, "OPT_MBR")==0)
		{
			CheckMbrType(cur_tmplt.fld_buff[i].fld_name, &mbr_type);
			if( mbr_type == type)
			{
				strcpy(item_name, cur_tmplt.fld_buff[i].fld_def.opt_mbr_fld.list_name[item_value]);
				break;
			}
		}
	}
	return;
}

void SXPointToXPoint (xsrc, ysrc, xobj, yobj)
int     *xsrc, *xobj;
int     *ysrc, *yobj;
{
        *xobj   = SXtoX (*xsrc);
        *yobj   = SYtoY (*ysrc);
}
