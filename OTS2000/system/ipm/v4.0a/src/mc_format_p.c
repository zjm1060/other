
#include	<stdio.h>

#include	"../src/resource.h"

#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"
#include	"../inc/tmplt.h"
#include	"../inc/dyn_form_def.h"
#include	"../inc/field_def.h"

int	tmplt_lookup[100];		/* Template look up table */

int	cur_format_type;		/* Selected dynamic format */
     
extern NEW_GBRCD	*pCurrSelectedRcd;

BOOL CALLBACK FormatDlgProc(HWND, UINT, WPARAM, LPARAM);
void PopupDynDlgBox (int, int, int, short);
BOOL CenterWindow (HWND, HWND);
NEW_GBRCD* GetFirstCurrSelectedRcd(void);

void PopupFormatDialog ()
{
	pCurrSelectedRcd= GetFirstCurrSelectedRcd();
	if(!pCurrSelectedRcd)
		return;
	
	DialogBox( ghInst, MAKEINTRESOURCE(DLG_DYN_FORMAT),
				 ghWnd, (DLGPROC)FormatDlgProc );
}

BOOL CALLBACK FormatDlgProc(hDlg, msg, wParam, lParam)
HWND	hDlg;
UINT	msg;
WPARAM	wParam;
LPARAM	lParam;
{
	int	i, choice, idc;
	HWND	hlist;
	int	user_tmplt_cnt;			/* Number of user templates: ipm.tpl*/
	int	cgrp_type;
	int	cgrp_tag;
	short	add_color_type;

	switch (msg)
	{
		case WM_INITDIALOG:
			hlist = GetDlgItem(hDlg, IDC_FORMAT_LIST);
			SendMessage(hlist,LB_RESETCONTENT,0,0 ); 
			user_tmplt_cnt	=0;
			for (i=0; i<tmplt_def_cnt; i++)
			{
				if(tmplt_nm_ptrs[i].format_num < 100)
				{
					tmplt_lookup[user_tmplt_cnt++]  = i;
					SendMessage(hlist,LB_ADDSTRING,0,(LPARAM)(LPCSTR)tmplt_nm_ptrs[i].name);
				}
			}
			cur_format_type	= tmplt_lookup[0];
			SendMessage(hlist, LB_SETCURSEL, cur_format_type, 0);

			if( pCurrSelectedRcd->lnk.act.ctrl_tag == CTRL_TAG_ON )
				ctrl_req_type = pCurrSelectedRcd->lnk.act.ctrl_type;
			else ctrl_req_type = CTRL_REQ_NULL;
//			ctrl_req_type	= 0;
			CheckRadioButton(hDlg, IDC_NO_REQ, IDC_MENU_REQ, IDC_NO_REQ+ctrl_req_type);

			CenterWindow(hDlg, ghWnd);
			return TRUE;

		case WM_COMMAND:
			idc = LOWORD(wParam);
			if (HIWORD(wParam) == BN_CLICKED) 
			{
				switch( idc )
				{
					case IDC_NO_REQ:
					case IDC_DSP_REQ:
					case IDC_PROG_REQ:
					case IDC_MENU_REQ:
						CheckRadioButton(hDlg, IDC_NO_REQ, IDC_MENU_REQ, idc);
						ctrl_req_type = LOWORD(wParam) - IDC_NO_REQ; 
						return TRUE;
				}
			}

			switch( idc )
			{
				case IDC_FORMAT_LIST:
					switch( HIWORD(wParam) )
					{
						case LBN_SELCHANGE:
							choice = SendMessage(GetDlgItem(hDlg, IDC_FORMAT_LIST),
												LB_GETCURSEL,0,0L);
							cur_format_type = tmplt_lookup[choice];	 // from 0 beginning
							return TRUE;
						default:	
							break;
					}
					break;
				case IDOK:
					EndDialog(hDlg, wParam);
					cgrp_type = pCurrSelectedRcd->lnk.cgrp.cgrp_id;
					cgrp_tag = pCurrSelectedRcd->lnk.cgrp.tag;	
					if( cgrp_type != 0  && cgrp_tag == DYN_CGRP_TAG_ADD_ON )
						add_color_type =  HAVE_ADD_COLOR;
					else
						add_color_type = NO_ADD_COLOR;
//					add_color_type = NO_ADD_COLOR;
					PopupDynDlgBox (CREATE_DYN_TYPE, cur_format_type, ctrl_req_type, add_color_type);
					return TRUE;
				case IDCANCEL:
					EndDialog(hDlg, wParam);
					return TRUE;
			}

			break;
		
		default:
			break;
	}
	return FALSE;
}
 



