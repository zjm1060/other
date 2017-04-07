
#include <stdio.h>

#include	"resource.h"

#include	"../inc/ipm.h"
#include	"../inc/string_grp_def.h"
#include	"../inc/mode_def.h"

extern int MenuStateSave;

void AddNewStringGroup (HWND);
void SaveStringGroup (char *);
void DeleteStringGroup(HWND, int);
void	PopupGetSaveAsNameBox();

void	LoadStringGrpLibrary ()
{
	int	len, i;
	FILE	*fp;
	char	str_grp_full_name[MAX_FILE_NAME_LEN];
	char	*str_grp_part_name = {"/DSP/STRING_GRP_LIBRARY.DBIN"};
	char str[300];

	FormFullName ( home, str_grp_full_name, str_grp_part_name );
	fp = fopen( str_grp_full_name , "rb");
	if (fp == NULL)
	{
		sprintf(str, "file open error: %s", str_grp_full_name);
		MessageBox(NULL, str, "IPM", MB_OK | MB_ICONHAND);
		ExitProc();
        PostQuitMessage (0) ;
        return;
	}

	len     =sizeof(strg_cnt);
	fread (&strg_cnt, len, 1, fp);
	strg_cnt = min(strg_cnt, MAX_STR_GRPS);
	len     =sizeof(strgdb[0]);
	for (i=0; i<strg_cnt; i++)
	{
		fread (&strgdb[i], len, 1, fp);
	}	

	fclose (fp);

}

void	LoadSecurityFile ()
{
	FILE	*fp;
	char	*secu_name = {"/users/default/APILL.JKE"};
    return ;   
	/*rm by chichi */
	fp = fopen(secu_name,  "rb");
	if (fp == NULL)
	{
		MessageBox(NULL, "here :open APILL.JKE error.", "IPM", MB_OK | MB_ICONHAND);
		exit(0);
	}
}

void SaveStringGrpLibrary ()
{
	int	len, i;
	FILE	*fp;
	char	str_grp_full_name[MAX_FILE_NAME_LEN];
	char	*str_grp_part_name = {"/DSP/STRING_GRP_LIBRARY.DBIN"};

	FormFullName ( home, str_grp_full_name, str_grp_part_name );
	fp = fopen( str_grp_full_name , "wb");

	if (fp == NULL)
	{
		MessageBox(NULL, "open STRING_GRP_LIBRARY.DBIN error.", "IPM", MB_OK | MB_ICONHAND);
		ExitProc();
        PostQuitMessage (0) ;
        return;
	}

	len     =sizeof(strg_cnt);
	fwrite (&strg_cnt, len, 1, fp);
	len	=sizeof(strgdb[0]);
	for (i=0; i<strg_cnt; i++)
	{
		fwrite (&strgdb[i], len, 1, fp);
	}
	fclose (fp);

}

void	PutStringGrp (HWND hDlg)
{
	int	i;

	for (i=0; i < GRP_MAX_STRS; i++)
	{
		SetDlgItemText(hDlg, IDC_STGED_TXT0 + i, strgdb[info.str_grp].str[i] );
	}
}

BOOL CALLBACK StringGroupEditHandler(HWND hDlg, UINT wMsg, WPARAM wParam, LONG lParam)
{
	int	i;
	HWND	hStrGrpList;
    HMENU	hmenu;

    switch(wMsg)
    {
		case WM_INITDIALOG:
			hStrGrpList = GetDlgItem(hDlg, IDC_STGED_GRP_LIST);
			for( i=0; i<strg_cnt; i++ )
				SendMessage(hStrGrpList, LB_ADDSTRING, 0, (LPARAM)strgdb[i].name);
			SendMessage(hStrGrpList, LB_SETCURSEL, info.str_grp, 0);

			PutStringGrp (hDlg);
			CenterWindow(hDlg, ghWnd);
			return TRUE;

		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
			case IDC_STGED_GRP_LIST:
				switch( HIWORD(wParam) )
				{
				case LBN_SELCHANGE:
					hStrGrpList = GetDlgItem(hDlg, IDC_STGED_GRP_LIST);
					info.str_grp = SendMessage(hStrGrpList, LB_GETCURSEL, 0, 0);
					str_grp_edit_mode	=EDIT_EXISTING_STR_GRP;
					PutStringGrp (hDlg);
					EnableWindow(GetDlgItem(hDlg, IDC_STGED_SAVE), TRUE);
					return TRUE;
				default:	
					break;
				}
				break;
			case IDC_STGED_NEW:
				str_grp_edit_mode	=ADD_NEW_STR_GRP;
				EnableWindow(GetDlgItem(hDlg, IDC_STGED_SAVE), FALSE);
				AddNewStringGroup (hDlg);
				break;
			case IDC_STGED_SAVE:
				SaveStringGroup (strgdb[info.str_grp].name);
				break;
			case IDC_STGED_SAVE_AS:
				PopupGetSaveAsNameBox ();
               	break;
			case IDC_STGED_DEL:
				DeleteStringGroup (hDlg, info.str_grp);
				break;
			case IDC_STGED_SAVE_FILE:
				SaveStringGroup (strgdb[info.str_grp].name);  //added by chi chi
				SaveStringGrpLibrary ();
				break;
			case IDCANCEL:
					DestroyWindow (hDlg);
					ghWndStrGrpEdit = NULL;
					edit_mode	=	DISPLAY_EDIT;
					hmenu = GetMenu( ghWnd );
           		 	EnableMenuItem( hmenu, ID_Open_File, MF_BYCOMMAND | MF_ENABLED );
          		  	EnableMenuItem( hmenu, ID_SAVE, MF_BYCOMMAND | MenuStateSave );
            		EnableMenuItem( hmenu, ID_SaveAs, MF_BYCOMMAND | MF_ENABLED );
    				EnableMenuItem( GetSubMenu( hmenu, 0), 7, MF_BYPOSITION | MF_ENABLED );
            		EnableMenuItem( hmenu, ID_Grid_Set, MF_BYCOMMAND | MF_ENABLED );
            		DrawMenuBar( ghWnd);
					return TRUE;
			}
			break;
    }
    return FALSE;
}



void AddNewStringGroup (HWND hdlg)
{
	int	i;

	info.str_grp	=strg_cnt;
	strcpy (strgdb[strg_cnt].name, " ");
	for (i=0; i < GRP_MAX_STRS; i++)
	{
		strcpy (strgdb[strg_cnt].str[i], " ");
	}
	PutStringGrp (hdlg);

}


void SaveStringGroup (str_grp_name)
char	*str_grp_name;
{
	int		i;
	HWND	hStrGrpList;

	if (str_grp_edit_mode == EDIT_EXISTING_STR_GRP)
	{
		for (i=0; i<GRP_MAX_STRS; i++)
		{
			GetDlgItemText(ghWndStrGrpEdit, IDC_STGED_TXT0 + i, strgdb[info.str_grp].str[i], STR_LEN );
		}
	}
	else if (str_grp_edit_mode == ADD_NEW_STR_GRP)
	{
		if (strg_cnt < MAX_STR_GRPS)
		{
			strcpy (strgdb[info.str_grp].name, str_grp_name);
			for (i=0; i<GRP_MAX_STRS; i++)
			{
				GetDlgItemText(ghWndStrGrpEdit, IDC_STGED_TXT0 + i, strgdb[info.str_grp].str[i], STR_LEN );
			}
			strg_cnt++;
			 
			str_grp_edit_mode = EDIT_EXISTING_STR_GRP;
			hStrGrpList = GetDlgItem(ghWndStrGrpEdit, IDC_STGED_GRP_LIST);
			SendMessage(hStrGrpList, LB_ADDSTRING, 0, (LPARAM)str_grp_name);
			SendMessage(hStrGrpList, LB_SETCURSEL, info.str_grp, 0);
			EnableWindow(GetDlgItem(ghWndStrGrpEdit, IDC_STGED_SAVE), TRUE);
		}
		else
		{ MessageBox (ghWndStrGrpEdit, "String Group Buffer Over Flow", "IPM", MB_OK); }
	}
	else if (str_grp_edit_mode == COPY_EXISTING_STR_GRP)
	{
		if (strg_cnt < MAX_STR_GRPS)
		{ 
			info.str_grp		=strg_cnt;
			strcpy (strgdb[info.str_grp].name, str_grp_name);
			for (i=0; i<GRP_MAX_STRS; i++)
			{
				GetDlgItemText(ghWndStrGrpEdit, IDC_STGED_TXT0 + i, strgdb[info.str_grp].str[i], STR_LEN );
			}
			strg_cnt++;
			 
			hStrGrpList = GetDlgItem(ghWndStrGrpEdit, IDC_STGED_GRP_LIST);
			SendMessage(hStrGrpList, LB_ADDSTRING, 0, (LPARAM)str_grp_name);
			SendMessage(hStrGrpList, LB_SETCURSEL, info.str_grp, 0);
			EnableWindow(GetDlgItem(ghWndStrGrpEdit, IDC_STGED_SAVE), TRUE);
		}
		else
		{ MessageBox (ghWndStrGrpEdit, "String Group Buffer Over Flow", "IPM", MB_OK); }
		str_grp_edit_mode = EDIT_EXISTING_STR_GRP;
	}

}

void DeleteStringGroup(HWND hDlg, int strgrp_id)
{
	int	i;
	HWND hStrGrpList;
		
	for (i=strgrp_id; i < strg_cnt-1; i++)
	{
		strgdb[i]	=strgdb[i+1];
	}
	strg_cnt--;
	strg_cnt = max(strg_cnt, 0);  
	info.str_grp = min(info.str_grp, strg_cnt-1 );  

	hStrGrpList = GetDlgItem(hDlg, IDC_STGED_GRP_LIST);
	SendMessage(hStrGrpList, LB_DELETESTRING, strgrp_id, 0);
	SendMessage(hStrGrpList, LB_SETCURSEL, info.str_grp, 0);
	PutStringGrp (hDlg);
}

