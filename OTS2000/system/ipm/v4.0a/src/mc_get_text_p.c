
#include <stdio.h>

#include	"resource.h"

#include	"../inc/ipm.h"
#include	"../inc/color_grp_def.h"
#include	"../inc/menu_def.h"
#include	"../inc/mode_def.h"
#include	"../inc/string_grp_def.h"
#include	"../inc/symbol_def.h"
#include	"../inc/symbol_grp_def.h"

void GetStringProc (char *);
int GetNameLen();
BOOL CALLBACK GetSaveAsNameProc(HWND, UINT, WPARAM, LPARAM);
void SaveSymbolGroup (char *);
void SaveColorGroup (char *);
void SaveStringGroup (char *);
void SaveMenu (char *);
void SaveSymbol (char *);
void AddSymbolClass (char *);

void	PopupGetSaveAsNameBox()
{

	DialogBox( ghInst, MAKEINTRESOURCE(DLG_GET_NAME),
				 ghWnd, (DLGPROC)GetSaveAsNameProc );

}

BOOL CALLBACK GetSaveAsNameProc(hDlg, message, wParam, lParam)
HWND	hDlg;
UINT	message;
WPARAM	wParam;
LPARAM	lParam;
{
	HWND	hlabel,hText;
	char string[MAX_FILE_NAME_LEN];
	char label_string[25];

	switch (message)
	{
		case WM_INITDIALOG:
			hlabel = GetDlgItem(hDlg, IDC_NAME_LABLE);
        	if (edit_mode == SYMBOL_EDIT)
        	{
				if (sym_edit_mode == ADD_NEW_SYMBOL)
				{ 
					strcpy(label_string, "Symbol Name:");
				}
				else if (sym_edit_mode == ADD_SYMBOL_CLASS)
				{ 
					strcpy(label_string, "Symbol Class Name:");
				}
				else if (sym_edit_mode == EDIT_EXISTING_SYMBOL)
				{ 
					strcpy(label_string, "Symbol Name:");
				}
        	}
			else if (edit_mode == SYMBOL_GRP_EDIT)
        	{ 
				strcpy(label_string, "Symbol Group Name:");
			}
        	else if (edit_mode == COLOR_GRP_EDIT)
        	{ 
				strcpy(label_string, "Color Group Name:");
			}
			else if (edit_mode == MENU_EDIT)
			{ 
				strcpy(label_string, "Menu Name:");
			}
			else if (edit_mode == STRING_GRP_EDIT)
			{ 
				strcpy(label_string, "String Group Name:");
			}
			else
			{
				strcpy(label_string, "Name:");
			}
			SetWindowText(hlabel, label_string);
			hText = GetDlgItem(hDlg, IDC_SAVEAS_NAME);
            SetFocus(hText);
			CenterWindow(hDlg, ghWnd);
			return TRUE;
				
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case IDOK:
					memset(string, 0, sizeof(string));
					GetDlgItemText(hDlg, IDC_SAVEAS_NAME, string, GetNameLen());
					GetStringProc(string);
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

int GetNameLen()
{
    int	name_len;
    
    if (edit_mode == DISPLAY_EDIT)
    { 
		name_len	= MAX_FILE_NAME_LEN;
	}
    else if (edit_mode == SUB_DSP_EDIT)
    { 
		name_len	= MAX_FILE_NAME_LEN;
	}
    else if (edit_mode == SYMBOL_EDIT)
    {
		if (sym_edit_mode == ADD_NEW_SYMBOL)
		{ 
			name_len	= SYMBOL_NAME_LEN;
		}
		else if (sym_edit_mode == ADD_SYMBOL_CLASS)
		{ 
			name_len	= SYMBOL_CLASS_NAME_LEN;
		}
		else if (sym_edit_mode == EDIT_EXISTING_SYMBOL)
		{ 
			name_len	= SYMBOL_NAME_LEN;
		}
    }
	else if (edit_mode == SYMBOL_GRP_EDIT)
    { 
		name_len	= SYMBOL_GRP_NAME_LEN;
	}
    else if (edit_mode == COLOR_GRP_EDIT)
    { 
		name_len	= COLOR_GRP_NAME_LEN;
	}
	else if (edit_mode == MENU_EDIT)
	{ 
		name_len	= MENU_NAME_LEN;
	}
	else if (edit_mode == STRING_GRP_EDIT)
	{ 
		name_len	= STR_GRP_NAME_LEN;
	}
	return name_len;
}


void GetStringProc (char *text_str)
{

		if (edit_mode == SYMBOL_EDIT)
		{
			if (sym_edit_mode == ADD_NEW_SYMBOL)
			{ SaveSymbol (text_str); }
			else if (sym_edit_mode == EDIT_EXISTING_SYMBOL)
			{
				sym_edit_mode	=COPY_EXISTING_SYMBOL;
				SaveSymbol (text_str);
			}
			else if (sym_edit_mode == ADD_SYMBOL_CLASS) 
			{ AddSymbolClass (text_str); }
		}
		else if (edit_mode == SYMBOL_GRP_EDIT)
		{
			if (sym_grp_edit_mode == ADD_NEW_SYMBOL_GRP)
			{ SaveSymbolGroup (text_str); }
			else if (sym_grp_edit_mode == EDIT_EXISTING_SYMBOL_GRP)
			{
				sym_grp_edit_mode	=COPY_EXISTING_SYMBOL_GRP;
				SaveSymbolGroup (text_str);
			}
		}
		else if (edit_mode == STRING_GRP_EDIT)
		{
			if (str_grp_edit_mode == ADD_NEW_STR_GRP)
			{ SaveStringGroup (text_str); }
			else if (str_grp_edit_mode == EDIT_EXISTING_STR_GRP)
			{
				str_grp_edit_mode	=COPY_EXISTING_STR_GRP;
				SaveStringGroup (text_str);
			}
		}
		else if (edit_mode == COLOR_GRP_EDIT)
		{
			if (cgrp_edit_mode == ADD_NEW_COLOR_GRP)
			{ SaveColorGroup (text_str); }
			else if (cgrp_edit_mode == EDIT_EXISTING_COLOR_GRP)
			{
				cgrp_edit_mode	=COPY_EXISTING_COLOR_GRP;
				SaveColorGroup (text_str);
			}
		}
		else if (edit_mode == MENU_EDIT)
		{
			if (menu_edit_mode == ADD_NEW_MENU)
			{ 
				SaveMenu (text_str); 
			}
			else if (menu_edit_mode == EDIT_EXISTING_MENU)
			{
				menu_edit_mode	=COPY_EXISTING_MENU;
				SaveMenu (text_str);
			}
		}	

}




