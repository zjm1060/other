#include <windows.h>
#include <Prsht.h>
#include <commctrl.h>
#include <stdio.h>

#include	"../src/resource.h"
#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"
#include	"../inc/zoom_def.h"
#include	"../inc/view_def.h"
#include	"../inc/mode_def.h"
#include	"../inc/menu_def.h"
#include	"../inc/toolkit.h"
#include	"../inc/symbol_grp_def.h"
#include	"../inc/symbol_def.h"
#include	"../inc/color_grp_def.h"
#include	"../inc/string_grp_def.h"
#include	"../inc/pr.h"
#include	"../inc/tmplt.h"
#include	"../inc/dyn_form_def.h"
#include	"../inc/dyn_prog_def.h"
#include	"../../../dms/v4.0a/inc/dms_com.h"

#pragma	comment(lib, "comctl32.lib")

extern NEW_GBRCD	tmp_edit_dyn_rcd;
extern NEW_GBRCD	*pCurrSelectedRcd;

void	InitDbTreeView(HWND hTreeView, char *selected_pt_name)
{
	INITCOMMONCONTROLSEX	InitCtrls;
	TVINSERTSTRUCT	tvinsert;
	HTREEITEM	hStn, hDev, hType, hEntry;
	char	stn_comment[STATION_COMMENT_SIZE];
	char	dev_comment[GROUP_COMMENT_SIZE];
	char	datatype_longname[RECORD_LONGNAME_SIZE];
	char	entry_longname[ENTRY_LONGNAME_SIZE];
	char	text[128];
	UCHAR	stn_id, dev_id, data_type;
	int		stn_num,grp_num,rec_num,entry_num;
	int		id;
	POINTER	pt, selected_pt;
				
	
	memset(&selected_pt, 0, sizeof(selected_pt));
	if(selected_pt_name[0])
		GetPtIdByNameStr(selected_pt_name, &selected_pt);
	
	memset(&InitCtrls, 0, sizeof(InitCtrls));
	InitCtrls.dwSize= sizeof(InitCtrls);
	InitCtrls.dwICC= ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	GetStationNum(&stn_num);
	for(stn_id=1; stn_id<=stn_num; stn_id++) 
	{  		
		if( GetStnCommentById(stn_id,(char*)( stn_comment))==-1 )
			continue;
		
		sprintf(text, "%d. %s", stn_id, stn_comment);
		tvinsert.hParent=NULL;
		tvinsert.hInsertAfter=TVI_ROOT;
		tvinsert.item.mask=TVIF_TEXT;
		tvinsert.item.pszText=text;
		hStn= TreeView_InsertItem(hTreeView, (LPARAM)&tvinsert);
		
		if(-1==GetGroupNumById(stn_id, &grp_num))
			continue;
		
		for(dev_id=1; dev_id<=grp_num; dev_id++) 
		{  	
			int	j;
			if(-1==GetGroupCommentById(stn_id, dev_id, dev_comment))
				continue;
			
			sprintf(text, "%d. %s", dev_id, dev_comment);
			tvinsert.hParent=hStn;          
			tvinsert.hInsertAfter=TVI_LAST;   
			tvinsert.item.pszText=text;
			hDev= TreeView_InsertItem(hTreeView, (LPARAM)&tvinsert);
			

			if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
				continue;
			
			for(data_type=1,j=0; j<rec_num; data_type++) 
			{
				if(data_type>=MAX_DATATYPE_NUM)
					break;
				if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)data_type, &entry_num))
					continue;
				
				GetRecLongnameByType(data_type, datatype_longname);
				j++;
				
				sprintf(text, "%d. %s", data_type, datatype_longname);
				tvinsert.hParent=hDev;          
				tvinsert.hInsertAfter=TVI_LAST;   
				tvinsert.item.pszText=text;
				hType= TreeView_InsertItem(hTreeView, (LPARAM)&tvinsert);

				pt.stn_id= stn_id;
				pt.dev_id= dev_id;
				pt.data_type= data_type;
				for(id=0; id<entry_num; id++)
				{
					pt.pt_id= id;
					if(-1==GetEntryLongnameById(&pt, entry_longname))
						continue;
					
					sprintf(text, "%d. %s", id, entry_longname);
					tvinsert.hParent=hType;          
					tvinsert.hInsertAfter=TVI_LAST;   
					tvinsert.item.pszText=text;
					tvinsert.item.cChildren= 0;
					hEntry= TreeView_InsertItem(hTreeView, (LPARAM)&tvinsert);

					if(pt.stn_id==selected_pt.stn_id && pt.dev_id==selected_pt.dev_id
						&& pt.data_type==selected_pt.data_type && pt.pt_id==selected_pt.pt_id)
						TreeView_SelectItem(hTreeView, hEntry);
				}
			}
		}
	}
}

void	InitDyncolorList(HWND hList, int color_grp_id)
{
	int	i;

	for(i=0; i<cgdb_cnt; i++)
		SendMessage(hList,CB_ADDSTRING, 0, (LPARAM)(LPSTR)cgdb[i].name);

	SendMessage(hList, CB_SETCURSEL, color_grp_id, 0);
}	

void	InitDynViewList(HWND hList, int dyn_view_type)
{
	int	i;
	
	for(i=0; i<tmplt_def_cnt; i++)
		SendMessage(hList,CB_ADDSTRING,0,(LPARAM)(LPCSTR)tmplt_nm_ptrs[i].name);

	SendMessage(hList, CB_SETCURSEL, dyn_view_type, 0);
}	


void	InitDynCtrlTypeList(HWND hList, int dyn_ctrl_type)
{
	int	i;
	char *dynCtrlType[]={"无","控制画面","控制程序","控制菜单"};
	
	for(i=0; i<4; i++)
		SendMessage(hList,CB_ADDSTRING,0,(LPARAM)(LPCSTR)dynCtrlType[i]);
	
	SendMessage(hList, CB_SETCURSEL, dyn_ctrl_type, i);
}


void	InitDynCtrlMenuList(HWND hList, NEW_GBRCD *rcd)
{
	int	i;
	int	default_pos;
	
	SendMessage(hList,CB_RESETCONTENT,0,0 );

	if(rcd->lnk.act.ctrl_type==CTRL_REQ_MENU)
	{
		for(i = 0; i<v4_menudb.nextpos; i++)
			SendMessage(hList, CB_ADDSTRING,0,(LPARAM)(LPSTR)v4_menudb.menu[i].name);
		default_pos= rcd->lnk.act.fmt_def.menu.id;
		if(strcmp(rcd->lnk.act.fmt_def.menu.name,""))
			default_pos= SearchMenuId(rcd->lnk.act.fmt_def.menu.name);
		SendMessage(hList,CB_SETCURSEL,default_pos,0);
	}
	else if(rcd->lnk.act.ctrl_type==CTRL_REQ_PROG)
	{
		if(rcd->lnk.act.fmt_def.prog.id > PrgN_NULL &&	rcd->lnk.act.fmt_def.prog.id < MAX_PROG)
		{
			default_pos= 0;
			for(i=0; Cmd_Mode[rcd->lnk.act.fmt_def.prog.id][i]!=NULL; i++)
			{
				SendMessage(hList,CB_ADDSTRING, 0, (LPARAM)(LPSTR)Cmd_Mode[rcd->lnk.act.fmt_def.prog.id][i]);
				if(!strcmp(Cmd_Mode[rcd->lnk.act.fmt_def.prog.id][i], rcd->lnk.act.fmt_def.prog.status))
					default_pos= i;
			}
			SendMessage(hList,CB_SETCURSEL,default_pos,0);			
		}
	}
}


void	InitDynCtrlProgList(HWND hList)
{
	int	i;
	
	for(i = 0; i<MAX_PROG; i++)
		SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)(LPSTR)prog_names[i]);
}


void UpdateDynCtrlWindow(HWND hDlg, NEW_GBRCD *rcd)
{	
	HWND hDynCtrlList= GetDlgItem(hDlg,IDC_DYN_CTRL_TYPE_LIST);
	int	dynCtrlList= SendMessage(hDynCtrlList, CB_GETCURSEL, 0, 0);

	if(dynCtrlList != rcd->lnk.act.ctrl_type)
		PropSheet_Changed(GetParent(hDlg), hDlg);
	
	if(dynCtrlList ==CTRL_REQ_DSP)
	{
		EnableWindow(GetDlgItem(hDlg,IDC_DSP_FILE_SEL_BUTTON), TRUE);
		EnableWindow(GetDlgItem(hDlg,IDC_DYN_CTRL_DSP_FILENAME), TRUE);
		EnableWindow(GetDlgItem(hDlg,IDC_DYN_CTRL_PROG_LIST), FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_DYN_CTRL_MENU_LIST), FALSE);
		SendMessage(GetDlgItem(hDlg,IDC_DYN_CTRL_DSP_FILENAME), WM_SETTEXT, 0, (LPARAM)rcd->lnk.act.fmt_def.dsp.name);
		rcd->lnk.act.ctrl_type= dynCtrlList;
		rcd->lnk.act.ctrl_tag= CTRL_TAG_ON;
	}
	else if(dynCtrlList==CTRL_REQ_PROG)
	{
		HWND menu_list= GetDlgItem(hDlg,IDC_DYN_CTRL_MENU_LIST);
		
		EnableWindow(GetDlgItem(hDlg,IDC_DSP_FILE_SEL_BUTTON), FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_DYN_CTRL_DSP_FILENAME), FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_DYN_CTRL_PROG_LIST), TRUE);
		SetWindowText(GetDlgItem(hDlg,IDC_DYN_CTRL_MENU_TEXT), "操作类型:");
		EnableWindow(menu_list, TRUE);
		rcd->lnk.act.ctrl_type= dynCtrlList;
		rcd->lnk.act.ctrl_tag= CTRL_TAG_ON;

		SendMessage(GetDlgItem(hDlg,IDC_DYN_CTRL_PROG_LIST),CB_SETCURSEL,rcd->lnk.act.fmt_def.prog.id,0);			
		InitDynCtrlMenuList(menu_list, rcd);
	}
	else if(dynCtrlList==CTRL_REQ_MENU)
	{
		HWND menu_list= GetDlgItem(hDlg,IDC_DYN_CTRL_MENU_LIST);

		EnableWindow(GetDlgItem(hDlg,IDC_DSP_FILE_SEL_BUTTON), FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_DYN_CTRL_DSP_FILENAME), FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_DYN_CTRL_PROG_LIST), FALSE);
		SetWindowText(GetDlgItem(hDlg,IDC_DYN_CTRL_MENU_TEXT), "控制菜单:");
		EnableWindow(menu_list, TRUE);
		rcd->lnk.act.ctrl_type= dynCtrlList;
		rcd->lnk.act.ctrl_tag= CTRL_TAG_ON;

		InitDynCtrlMenuList(menu_list, rcd);
	}
	else
	{
		EnableWindow(GetDlgItem(hDlg,IDC_DSP_FILE_SEL_BUTTON), FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_DYN_CTRL_DSP_FILENAME), FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_DYN_CTRL_PROG_LIST), FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_DYN_CTRL_MENU_LIST), FALSE);
		rcd->lnk.act.ctrl_type= 0;
		rcd->lnk.act.ctrl_tag= CTRL_TAG_OFF;
	}
	
}	

char	currDynPtName[256];

BOOL APIENTRY DlgDynPointProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	HWND			hTreeView;
	LPNMHDR			pnmh;
	TVITEM			currItem;
	char			text[256];
	char			ptName[POINT_NAME_SIZE];
	#define MAX_TREE_LAYER	4
	int				id[MAX_TREE_LAYER], layer_num;
	POINTER			pt;
	HTREEITEM		Listected;
	extern	HWND	dyn_db_point_name_win;
	
	hTreeView= GetDlgItem(hDlg,IDC_DATABASE_TREE);
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, IDC_SEL_PT_NAME, currDynPtName);
		InitDbTreeView(hTreeView, currDynPtName);
		break;
	  
	case   WM_NOTIFY:
		pnmh= (LPNMHDR)lParam;   
		switch(pnmh->idFrom)
		{
		case IDC_DATABASE_TREE:
			switch(pnmh->code)
			{
			case NM_DBLCLK:
				Listected=(HTREEITEM)TreeView_GetNextItem(hTreeView,NULL,TVGN_CARET);
				if(Listected==NULL)
				{
					MessageBox(hDlg,"No Items in TreeView","Error",MB_OK|MB_ICONINFORMATION);
					break;
				}

				currItem.hItem= Listected;
				layer_num= 0;
				while(currItem.hItem!=NULL && currItem.hItem!=TVI_ROOT)
				{
					currItem.mask=TVIF_TEXT;
					currItem.pszText=text;
					currItem.cchTextMax=256;
					if(!TreeView_GetItem(hTreeView,(LPARAM)&currItem))
						break;
					if(1 != sscanf(text, "%d", &id[layer_num]))
						break;
					layer_num++;
					if(layer_num>MAX_TREE_LAYER)
					{
						MessageBox(hDlg,"数据库树错误","错误",MB_OK|MB_ICONINFORMATION);
						return FALSE;
					}
					currItem.hItem= TreeView_GetParent(hTreeView, currItem.hItem);						
				}
				
				if(layer_num==4)
				{
					pt.stn_id= id[3];
					pt.dev_id= id[2];
					pt.data_type= id[1];
					pt.pt_id= id[0];
					if(-1!=GetPtNameById(&pt, ptName, NULL))
						SetDlgItemText(hDlg, IDC_SEL_PT_NAME, ptName);					
				}						
				break;
			}					
			break;				
			
		default:
			break;
		}			
		break;
	  
	case WM_COMMAND:
		switch(wParam) 
		{
		case IDC_OK:
			GetDlgItemText(hDlg, IDC_SEL_PT_NAME, ptName, sizeof(ptName));
			SetWindowText(dyn_db_point_name_win, ptName);
			strcpy(currDynPtName, ptName);
			EndDialog(hDlg, 0);
			break;

		case IDC_CANCEL:
			EndDialog(hDlg, -1);
			break;
		default:
			;
		}
      break;
	default:;			
	}
	return(FALSE);
	UNREFERENCED_PARAMETER(lParam);
} 
  
BOOL APIENTRY DlgBaseAttrProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	return FALSE;
} 

BOOL DynPointList(HWND hWnd, char* pt_name)
{
	strcpy(currDynPtName, pt_name);
	if(!DialogBox(ghInst, MAKEINTRESOURCE(DLG_DYN_POINT_SELECT), hWnd, (DLGPROC)DlgDynPointProc))
	{
		strcpy(pt_name, currDynPtName);
		return TRUE;
	}
	else
		return FALSE;	
}

BOOL DynDspFileList(HWND hWnd, char* filename)
{
	OPENFILENAME OpenFileName;
	char	pathfilename[256];

	if(filename[0])
		sprintf(pathfilename, "\\home\\ems\\h9000\\dsp\\%s", filename);
	else
		pathfilename[0]= 0;
	filename[0]= 0;
	memset(&OpenFileName, 0, sizeof(OpenFileName));
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
	OpenFileName.hwndOwner         = hWnd;
	OpenFileName.hInstance         = NULL;
	OpenFileName.lpstrFilter		= "DSP Files (*.dbin)\0*.dbin\0";
	OpenFileName.nFilterIndex      = 1;
	OpenFileName.lpstrFile         = pathfilename;
	OpenFileName.nMaxFile          = 256;
	OpenFileName.lpstrFileTitle    = filename;
	OpenFileName.nMaxFileTitle     = 256;
	OpenFileName.lpstrInitialDir   = "\\home\\ems\\h9000\\dsp";
	OpenFileName.lpstrTitle		   = "画面文件选择窗口";
	OpenFileName.Flags			   = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	OpenFileName.lpstrDefExt	   = "*.dbin";
	
	if(GetOpenFileName(&OpenFileName))
		return TRUE;
	else
		return FALSE;
}



BOOL APIENTRY DlgDynAttrProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	HWND			hDynPtName, hDynColorList, hDynViewList, hDynCtrlTypeList, hDynCtrlMenuList, hDynCtrlProgList;
	LPPSHNOTIFY		pnmh;
	char			dsp_file_name[256], dyn_pt_name[256];
	
	hDynPtName= GetDlgItem(hDlg,IDC_DYN_LINK_POINT_NAME);
	hDynColorList= GetDlgItem(hDlg,IDC_DYN_COLOR_LIST);
	hDynViewList= GetDlgItem(hDlg,IDC_DYN_VIEW_LIST);
	hDynCtrlTypeList= GetDlgItem(hDlg,IDC_DYN_CTRL_TYPE_LIST);
	hDynCtrlMenuList= GetDlgItem(hDlg,IDC_DYN_CTRL_MENU_LIST);
	hDynCtrlProgList= GetDlgItem(hDlg,IDC_DYN_CTRL_PROG_LIST);
	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowText(hDynPtName, tmp_edit_dyn_rcd.lnk.dbname);
		InitDyncolorList(hDynColorList, tmp_edit_dyn_rcd.lnk.cgrp.cgrp_id);
		InitDynViewList(hDynViewList, tmp_edit_dyn_rcd.lnk.dyn.dyn_type);
		InitDynCtrlTypeList(hDynCtrlTypeList, tmp_edit_dyn_rcd.lnk.act.ctrl_type);
		InitDynCtrlMenuList(hDynCtrlMenuList, &tmp_edit_dyn_rcd);
		InitDynCtrlProgList(hDynCtrlProgList);
		UpdateDynCtrlWindow(hDlg, &tmp_edit_dyn_rcd);
		break;
		
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_LINK_POINT_SEL_BUTTON:
			strcpy(dyn_pt_name, tmp_edit_dyn_rcd.lnk.dbname);
			if(DynPointList(hDlg, dyn_pt_name))
				SetWindowText(hDynPtName, dyn_pt_name);
			break;

		case IDC_DYN_LINK_POINT_NAME:
			if(HIWORD(wParam)==EN_CHANGE)
			{
				SendMessage(hDynPtName, WM_GETTEXT, sizeof(dyn_pt_name), (LPARAM)dyn_pt_name);
				if(strlen(dyn_pt_name)>=POINT_NAME_SIZE)
				{
					MessageBox(hDlg, "点名太长,最大51", "IPM", MB_OK);
					break;
				}
				if(strcmp(dyn_pt_name, tmp_edit_dyn_rcd.lnk.dbname))
				{
					strcpy(tmp_edit_dyn_rcd.lnk.dbname, dyn_pt_name);
					PropSheet_Changed(GetParent(hDlg), hDlg);
				}
			}
			break;
			
		case IDC_DYN_CTRL_TYPE_LIST:
			if(HIWORD(wParam)==LBN_SELCHANGE)
			{
				UpdateDynCtrlWindow(hDlg, &tmp_edit_dyn_rcd);			
			}
			break;

		case IDC_DYN_CTRL_MENU_LIST:
			if(HIWORD(wParam)==LBN_SELCHANGE)
			{
				int menu_id= SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				if(tmp_edit_dyn_rcd.lnk.act.ctrl_type==CTRL_REQ_MENU)
				{
					if(menu_id<0 || menu_id>=v4_menudb.nextpos)
					{
						char	msg[256];
						sprintf(msg, "菜单ID错误: %d", menu_id);
						MessageBox(hDlg, msg, "IPM", MB_OK);
						break;
					}
					if(tmp_edit_dyn_rcd.lnk.act.fmt_def.menu.id!=menu_id)
					{
						tmp_edit_dyn_rcd.lnk.act.fmt_def.menu.id= menu_id;							
						strcpy(tmp_edit_dyn_rcd.lnk.act.fmt_def.menu.name, v4_menudb.menu[menu_id].name);  
						PropSheet_Changed(GetParent(hDlg), hDlg);
					}
				}
				else if(tmp_edit_dyn_rcd.lnk.act.ctrl_type==CTRL_REQ_PROG)
				{
					if(Cmd_Mode[tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id][menu_id]==NULL)
					{
						char	msg[256];
						sprintf(msg, "操作类型ID错误: %d", menu_id);
						MessageBox(hDlg, msg, "IPM", MB_OK);
						break;
					}
					if(strcmp(tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.status, 
						Cmd_Mode[tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id][menu_id]))
					{
						strcpy(tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.status, Cmd_Mode[tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id][menu_id]);  
						PropSheet_Changed(GetParent(hDlg), hDlg);
					}
				}
			}
			break;
			
		case IDC_DYN_CTRL_PROG_LIST:
			if(HIWORD(wParam)==LBN_SELCHANGE)
			{
				int prog_id= SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				if(tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id!=prog_id)
				{
					tmp_edit_dyn_rcd.lnk.act.fmt_def.prog.id= prog_id;							
					InitDynCtrlMenuList(hDynCtrlMenuList, &tmp_edit_dyn_rcd);
					PropSheet_Changed(GetParent(hDlg), hDlg);
				}
			}
			break;
			
		case IDC_DSP_FILE_SEL_BUTTON:
			strcpy(dsp_file_name, tmp_edit_dyn_rcd.lnk.act.fmt_def.dsp.name);
			if(DynDspFileList(hDlg, dsp_file_name))
				SendMessage(GetDlgItem(hDlg,IDC_DYN_CTRL_DSP_FILENAME), WM_SETTEXT, 0, (LPARAM)dsp_file_name);
			break;
			
		case IDC_DYN_CTRL_DSP_FILENAME:
			if(HIWORD(wParam)==EN_CHANGE)
			{
				SendMessage(GetDlgItem(hDlg,IDC_DYN_CTRL_DSP_FILENAME), WM_GETTEXT, sizeof(dsp_file_name), (LPARAM)dsp_file_name);
				if(strlen(dsp_file_name)>=DSP_SHORTNAME_LEN)
				{
					MessageBox(hDlg, "文件名太长,最大30", "IPM", MB_OK);
					break;
				}
				if(strcmp(tmp_edit_dyn_rcd.lnk.act.fmt_def.dsp.name, dsp_file_name))
				{
					strcpy(tmp_edit_dyn_rcd.lnk.act.fmt_def.dsp.name, dsp_file_name);
					PropSheet_Changed(GetParent(hDlg), hDlg);
				}
			}
			break;
		}
		break;
		
	case WM_NOTIFY:
		pnmh= (LPPSHNOTIFY)lParam;   
		switch(pnmh->hdr.code)
		{
		case PSN_KILLACTIVE:
			SetWindowLong(hDlg, DWL_MSGRESULT, FALSE);
			break;
			
		case PSN_APPLY:
			*pCurrSelectedRcd= tmp_edit_dyn_rcd;
//			PropSheet_CancelToClose(GetParent(hDlg));
			break;
		}
		
	default:;	
		break;
	}
	return(FALSE);
	UNREFERENCED_PARAMETER(lParam);
} 



NEW_GBRCD* GetFirstCurrSelectedRcd(void);

int PopupAttrDialog(HWND hParent)
{
	HPROPSHEETPAGE	hPage[2];
	PROPSHEETPAGE		page;
	PROPSHEETHEADER	sheetheader;

	pCurrSelectedRcd= GetFirstCurrSelectedRcd();
	if(!pCurrSelectedRcd)
		return -1;
	tmp_edit_dyn_rcd	= *pCurrSelectedRcd;
	
	memset(&page, 0, sizeof(page));
	page.dwSize= sizeof(page);
	page.pszTemplate= MAKEINTRESOURCE(DLG_BASE_ATTR);
	page.pfnDlgProc= DlgBaseAttrProc;
	hPage[0]= CreatePropertySheetPage(&page);

	memset(&page, 0, sizeof(page));
	page.dwSize= sizeof(page);
	page.pszTemplate= MAKEINTRESOURCE(DLG_DYN_ATTR);
	page.pfnDlgProc= DlgDynAttrProc;
	hPage[1]= CreatePropertySheetPage(&page);

	memset(&sheetheader, 0, sizeof(sheetheader));
	sheetheader.dwSize= sizeof(sheetheader);
	sheetheader.hwndParent= hParent;
	sheetheader.pszCaption= "图元属性";
	sheetheader.nPages= sizeof(hPage)/sizeof(HPROPSHEETPAGE);
	sheetheader.phpage= hPage;
	sheetheader.nStartPage= 1;
	PropertySheet(&sheetheader);	  
	return 0;
}












































