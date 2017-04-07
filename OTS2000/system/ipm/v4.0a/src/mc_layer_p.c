
#include	<stdio.h>

#include	"../src/resource.h"

#include	"../inc/ipm.h"
#include	"../inc/layer_def.h"
#include	"../inc/zoom_def.h"
int  iPageMode   =0  ; 

BOOL CALLBACK LayerSetProc(HWND, UINT, WPARAM, LPARAM);
void SetPageRage (int id )
{ 
 //MF_CHECKED 
	int res ; 
	HMENU	hmenu;
	int  i ;

	hmenu = GetSubMenu( GetSubMenu( GetMenu(ghWnd), 4), 7); //ҳ�淶Χ�趨
	for (i=0; i< 2; i++)
		res	= CheckMenuItem(hmenu, ID_NORMAL_PAGE +i, MF_BYCOMMAND | MF_UNCHECKED);
 
	
	res = CheckMenuItem(hmenu, ID_NORMAL_PAGE +id, MF_BYCOMMAND | MF_CHECKED);
	iPageMode  = id ; 
	DrawMenuBar(ghWnd);
	InvalidateRect( ghWnd, NULL, TRUE );
   	
}

void SetCurrentLayerBtn ( int layer_id )
{
	int	i;
	HMENU	hmenu;

	hmenu = GetSubMenu( GetSubMenu( GetMenu(ghWnd), 4), 6);
	for (i=0; i< MAXLAYERS; i++)
	{
		CheckMenuItem(hmenu, ID_Layer_01+i, MF_BYCOMMAND | MF_UNCHECKED);
	}

	CheckMenuItem(hmenu, ID_Layer_01+layer_id, MF_BYCOMMAND | MF_CHECKED);
	DrawMenuBar(ghWnd);
	cur.bck.layer = layer_id;
	InvalidateRect( ghWnd, NULL, TRUE );

}


void SetLayerValue (hwndDlg, layer)
HWND	hwndDlg;
int	layer;
{
	char	string[6];

		sprintf(string, "%5.2f", zoomdb.factor);
		SetDlgItemText(hwndDlg, IDC_ZOOM_SIZE, string);

		sprintf(string, "%5.2f", layerdb[layer].min_visible);
		SetDlgItemText(hwndDlg, IDC_ZOOM_MIN, string);

		sprintf(string, "%5.2f", layerdb[layer].max_visible);
		SetDlgItemText(hwndDlg, IDC_ZOOM_MAX, string);

//		SetCurrentLayerBtn (layer);

}


void	PopupLayerSetBox()
{

	DialogBox( ghInst, MAKEINTRESOURCE(DLG_LAYER_SET),
				 ghWnd, (DLGPROC)LayerSetProc );

}

BOOL CALLBACK LayerSetProc(hDlg, message, wParam, lParam)
HWND	hDlg;
UINT	message;
WPARAM	wParam;
LPARAM	lParam;
{
	char	layer[4];
	int		i, layer_id;
	HWND	hlayer;
	char string[10];

	switch (message)
	{
		case WM_INITDIALOG:
			hlayer = GetDlgItem(hDlg, IDC_LAYER_SELECT);
			for( i=0; i<MAXLAYERS; i++ )
			{
				sprintf(layer,"%d", i+1);
				SendMessage(hlayer, CB_ADDSTRING, 0, (LPARAM)layer);
			}
			SendMessage(hlayer, CB_SETCURSEL, cur.bck.layer, 0);

			SetLayerValue (hDlg, cur.bck.layer);
			CenterWindow(hDlg, ghWnd);
			return TRUE;
				
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case IDC_LAYER_SELECT:
					switch( HIWORD(wParam) )
					{
						case CBN_SELCHANGE:
//							cur.bck.layer = SendMessage(GetDlgItem(hDlg, IDC_LAYER_SELECT),
//												CB_GETCURSEL,0,0L);
							layer_id = SendMessage(GetDlgItem(hDlg, IDC_LAYER_SELECT),
												CB_GETCURSEL,0,0L);
//							InvalidateRect( ghWnd, NULL, TRUE );
							SetLayerValue (hDlg, layer_id);
							return TRUE;
						default:	
							break;
					}
					break;

				case IDOK:
					layer_id = SendMessage(GetDlgItem(hDlg, IDC_LAYER_SELECT),
												CB_GETCURSEL,0,0L);
					memset(string, 0, sizeof(string));
					GetDlgItemText(hDlg, IDC_ZOOM_MIN, string, sizeof(string));
					layerdb[layer_id].min_visible = (float)atof (string);
					GetDlgItemText(hDlg, IDC_ZOOM_MAX, string, sizeof(string));
					layerdb[layer_id].max_visible	= (float)atof (string);
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

void HideLockOkProc ();
BOOL CALLBACK LayerHideLockProc(HWND, UINT, WPARAM, LPARAM);

void	PopupHideLockBox()
{

	DialogBox( ghInst, MAKEINTRESOURCE(DLG_LAYER_HIDE_LOCK),
				 ghWnd, (DLGPROC)LayerHideLockProc );

}

BOOL CALLBACK LayerHideLockProc(hDlg, message, wParam, lParam)
HWND	hDlg;
UINT	message;
WPARAM	wParam;
LPARAM	lParam;
{

	switch (message)
	{
		case WM_INITDIALOG:
			if(	hide_all_btn_tag == VISIBLE	)
				CheckRadioButton(hDlg, IDC_HIDE_ALL, IDC_SHOW_ALL, IDC_SHOW_ALL);
			else
				CheckRadioButton(hDlg, IDC_HIDE_ALL, IDC_SHOW_ALL, IDC_HIDE_ALL);
			if( lock_all_btn_tag == UNLOCKED )
				CheckRadioButton(hDlg, IDC_LOCK_ALL, IDC_UNLOCK_ALL, IDC_UNLOCK_ALL);
			else
				CheckRadioButton(hDlg, IDC_LOCK_ALL, IDC_UNLOCK_ALL, IDC_LOCK_ALL);

			CenterWindow(hDlg, ghWnd);
			return TRUE;

		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case IDC_HIDE_ALL:
					CheckRadioButton(hDlg, IDC_HIDE_ALL, IDC_SHOW_ALL, IDC_HIDE_ALL);
					hide_all_btn_tag	= UNVISIBLE;
					return TRUE;

				case IDC_SHOW_ALL:
					CheckRadioButton(hDlg, IDC_HIDE_ALL, IDC_SHOW_ALL, IDC_SHOW_ALL);
					hide_all_btn_tag	= VISIBLE;
					return TRUE;

				case IDC_LOCK_ALL:
					CheckRadioButton(hDlg, IDC_LOCK_ALL, IDC_UNLOCK_ALL, IDC_LOCK_ALL);
					lock_all_btn_tag	= LOCKED;
					return TRUE;

				case IDC_UNLOCK_ALL:
					CheckRadioButton(hDlg, IDC_LOCK_ALL, IDC_UNLOCK_ALL, IDC_UNLOCK_ALL);
					lock_all_btn_tag	= UNLOCKED;
					return TRUE;

				case IDOK:
					HideLockOkProc ();
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

void HideLockOkProc ()
{
	int	i;

	if ( hide_all_btn_tag == UNVISIBLE )
	{
		for (i=0; i< MAXLAYERS; i++)
			layerdb[i].hide_tag	= UNVISIBLE;
	}
	else
	{
		for (i=0; i< MAXLAYERS; i++)
			layerdb[i].hide_tag	= VISIBLE;
	}

	if ( lock_all_btn_tag == LOCKED )
	{
		for (i=0; i< MAXLAYERS; i++)
			layerdb[i].lock_tag	= LOCKED;
	}
	else
	{
		for (i=0; i< MAXLAYERS; i++)
			layerdb[i].lock_tag	= UNLOCKED;
	}

}

