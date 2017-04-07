
#include <stdio.h>

#include	"resource.h"
#include	"../inc/ipm.h"
#include	"../inc/mode_def.h"
#include	"../inc/symbol_grp_def.h"
#include	"../inc/symbol_def.h"
#include	"../inc/scaler.h"

BOOL PtInSubWin(HWND, HWND, POINT *, POINT *);
void DrawLine (HDC, int, int, int, int);
void ReExposeCanvasAreaInWin(HWND, HWND);
void PopupGetSaveAsNameBox();
void SymbolGroupInit (HWND);
void DrawSymbolGroupCanvas (HWND, HDC);
void DrawSymbolGroupGrid (HWND, HDC);
void SGED_DrawSymbol (HWND, HDC, SYMB_ID, int);
void AddNewSymbolGroup (HWND);
void SaveSymbolGroup (char *);
void DeleteSymbolGroup (HWND, int);

int MenuStateSave;

void	LoadSymbolGrpLibrary ()
{
	int	len, i;
	FILE	*fp;
	char	sym_grp_full_name[MAX_FILE_NAME_LEN];
	char	*sym_grp_part_name = {"/DSP/SYMBOL_GRP_LIBRARY.DBIN"};

	FormFullName ( home, sym_grp_full_name, sym_grp_part_name);
	fp = fopen( sym_grp_full_name , "rb");

	if (fp == NULL)
	{
		MessageBox(NULL, "open SYMBOL_GRP_LIBRARY.DBIN error.", "IPM", MB_OK | MB_ICONHAND);
		ExitProc();
        PostQuitMessage (0) ;
        return;
	}

	len	=4;
	fread (&symgdb_cnt, len, 1, fp);
	len     =sizeof(symgdb[0]);
	if ( symgdb_cnt > MAX_SYMBOL_GRPS ) symgdb_cnt = MAX_SYMBOL_GRPS;
	for (i=0; i<symgdb_cnt; i++)
	{
		fread (&symgdb[i], len, 1, fp);
	}	

	fclose (fp);

	symg_buff	=symgdb[info.sym_grp];

}


void SaveSymbolGrpLibrary ()
{
	int	len, i;
	FILE	*fp;
	char	sym_grp_full_name[MAX_FILE_NAME_LEN];
	char	*sym_grp_part_name = {"/DSP/SYMBOL_GRP_LIBRARY.DBIN"};

	FormFullName ( home, sym_grp_full_name, sym_grp_part_name);
	fp = fopen( sym_grp_full_name , "wb");

	if (fp == NULL)
	{
		MessageBox(NULL, "open SYMBOL_GRP_LIBRARY.DBIN error.", "IPM", MB_OK | MB_ICONHAND);
		ExitProc();
        PostQuitMessage (0) ;
        return;
	}

	len     =4;
	fwrite (&symgdb_cnt, len, 1, fp);
	len	=sizeof(symgdb[0]);
	for (i=0; i<symgdb_cnt; i++)
	{
		fwrite (&symgdb[i], len, 1, fp);
	}
	fclose (fp);

}


BOOL CALLBACK SymbolGroupEditHandler(HWND hDlg, UINT wMsg, WPARAM wParam, LONG lParam)
{
	int	row, col, i;
    HDC		hDC, hCanvasDC;
    PAINTSTRUCT ps, SymGrpPS;
	HWND	hSymGrpList, hSymGrpCanvas;
	POINT	pt, ptsub;
	float	xscalesave, yscalesave;
    HMENU	hmenu;

    switch(wMsg)
    {
		case WM_INITDIALOG:
			symg_buff	=symgdb[info.sym_grp];

			hSymGrpList = GetDlgItem(hDlg, IDC_SYM_GRP_LIST);
			for( i=0; i<symgdb_cnt; i++ )
				SendMessage(hSymGrpList, LB_ADDSTRING, 0, (LPARAM)symgdb[i].name);
			SendMessage(hSymGrpList, LB_SETCURSEL, info.sym_grp, 0);

			hSymGrpCanvas = GetDlgItem(hDlg, IDC_SYM_GRP_FRAME);
            SymbolGroupInit (hSymGrpCanvas);
			CenterWindow(hDlg, ghWnd);
			return TRUE;

        case WM_PAINT:
		    if(hDC = BeginPaint(hDlg,&ps))
    		{
				xscalesave = xscale;
				yscalesave = yscale;
				xscale = yscale = 1.0f;
				hSymGrpCanvas = GetDlgItem(hDlg, IDC_SYM_GRP_FRAME);
				hCanvasDC = BeginPaint(hSymGrpCanvas, &SymGrpPS);
   			 	if(ghPal)
    			{
    				SelectPalette (hCanvasDC, ghPal, FALSE);
    				RealizePalette (hCanvasDC);
				}
				DrawSymbolGroupCanvas (hSymGrpCanvas, hCanvasDC);
				EndPaint(hSymGrpCanvas, &SymGrpPS);
    			EndPaint(hDlg,&ps);
				xscale = xscalesave;
				yscale = yscalesave;
			}
            return TRUE;
		
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
			case IDC_SYM_GRP_LIST:
				switch( HIWORD(wParam) )
				{
				case LBN_SELCHANGE:
					hSymGrpList = GetDlgItem(hDlg, IDC_SYM_GRP_LIST);
					info.sym_grp = SendMessage(hSymGrpList,LB_GETCURSEL,0,0);
					symg_buff	=symgdb[info.sym_grp];
					sym_grp_edit_mode	=EDIT_EXISTING_SYMBOL_GRP;
					EnableWindow(GetDlgItem(hDlg, IDC_SAVE_SYM_GRP), TRUE);
					hSymGrpCanvas = GetDlgItem(hDlg, IDC_SYM_GRP_FRAME);
					ReExposeCanvasAreaInWin(hDlg, hSymGrpCanvas);
					return TRUE;
				default:	
					break;
				}
				break;
			case IDC_NEW_SYM_GRP:
				sym_grp_edit_mode	=ADD_NEW_SYMBOL_GRP;
				EnableWindow(GetDlgItem(hDlg, IDC_SAVE_SYM_GRP), FALSE);
				AddNewSymbolGroup (hDlg);
				break;
			case IDC_SAVE_SYM_GRP:
				SaveSymbolGroup (symg_buff.name);
				break;
			case IDC_SAVEAS_SYM_GRP:
				PopupGetSaveAsNameBox ();
               	break;
			case IDC_DEL_SYM_GRP:
				DeleteSymbolGroup (hDlg, info.sym_grp);
				break;
			case IDC_FSAVE_SYM_GRP:
				SaveSymbolGrpLibrary ();
				break;
			case IDCANCEL:
					DestroyWindow (hDlg);
					ghWndSymGrpEdit = NULL;
					edit_mode	=DISPLAY_EDIT;
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

		case WM_LBUTTONUP:
			pt.x = LOWORD(lParam);	
			pt.y = HIWORD(lParam);
			hSymGrpCanvas = GetDlgItem(hDlg, IDC_SYM_GRP_FRAME);
			if ( PtInSubWin(hDlg, hSymGrpCanvas, &pt, &ptsub) ) 
            {
            	col  = ptsub.x/SGED_sym_width;
            	row  = ptsub.y/SGED_sym_height;
           	 	//index   =col + row*SGED_xn;
				//symg_buff.symbol[index]	=info.sym;
				symg_buff.symbol[col][row]	=info.sym;/* Modified by zyp on 2001.5.16 */
				ReExposeCanvasAreaInWin(hDlg, hSymGrpCanvas);
			}
			break;

    }
    return FALSE;
}

void	SymbolGroupInit (HWND hSymGrpDrawArea)
{
	RECT    rc;

	//SGED_xn	=8;
	//SGED_yn	=2;

	SGED_xn	=16;/* Modified by zyp on 2001.5.16 */
	SGED_yn	=4;

	GetClientRect (hSymGrpDrawArea, &rc);
	SGED_canvas_width = rc.right;
	SGED_canvas_height = rc.bottom;

	SGED_sym_width	=SGED_canvas_width/SGED_xn;
	SGED_sym_height	=SGED_canvas_height/SGED_yn;
	SGED_canvas_width	=SGED_sym_width*SGED_xn;
	SGED_canvas_height	=SGED_sym_height*SGED_yn;

}

void DrawSymbolGroupCanvas (HWND hwnd, HDC hdc)
{
	int	i, j, pos;
	
	DrawSymbolGroupGrid (hwnd, hdc);


	for (i=0; i < GRP_MAX_SYMBOLS; i++)
	{
		for ( j=0; j<MAX_ANIMATE_SYMBOLS; j++ )/*Added by zyp on 2001.5.16 */
		{
			//if (symg_buff.symbol[i].name != 0)
			if (symg_buff.symbol[i][j].name != 0)
			{
			
				pos	= i+SGED_xn*j;
				SGED_DrawSymbol (hwnd, hdc, symg_buff.symbol[i][j], pos);
				//	SGED_DrawSymbol (hwnd, hdc, symg_buff.symbol[i], i);
			}
		}
	}

}

void DrawSymbolGroupGrid (HWND hwnd, HDC hdc)
{
	int	i;
	int	xtmp, ytmp;

	for (i=0; i<SGED_xn+1; i++)
	{
		xtmp    =i*SGED_sym_width;
		DrawLine (hdc, xtmp, 0, xtmp, SGED_canvas_height);
	}

	for (i=0; i<SGED_yn+1; i++)
	{
		ytmp    =i*SGED_sym_height;
		DrawLine (hdc, 0, ytmp, SGED_canvas_width, ytmp);
	}

}

void SGED_DrawSymbol (hwnd, hdc, symb, sym_pos)
HWND hwnd;
HDC hdc;
SYMB_ID	symb;
int	sym_pos;
{
	int	ix, iy;
    BACKRCD 	rcd;

    iy      = sym_pos/SGED_xn;
    ix	= sym_pos-iy*SGED_xn;

 	memset(&rcd, 0, sizeof(BACKRCD));  // must set initial
    rcd.rect.x1	= ix*SGED_sym_width;
    rcd.rect.y1	= iy*SGED_sym_height;
    rcd.rect.x2	= (ix+1)*SGED_sym_width;
    rcd.rect.y2	= (iy+1)*SGED_sym_height;
    rcd.data.sym	= symb;   

	DrawSymbol (hwnd, hdc, &rcd,symdb,1);

}


void AddNewSymbolGroup (hDlg)
HWND	hDlg;
{
	int	i,j;
	HWND hSymGrpCanvas;


	for (i=0; i < GRP_MAX_SYMBOLS; i++)
	{
		for ( j=0; j<MAX_ANIMATE_SYMBOLS; j++ )/*Added by zyp on 2001.5.16 */
		{
			symg_buff.symbol[i][j].class	=0;
			symg_buff.symbol[i][j].name	=0;
		}
	}
	info.sym_grp	=symgdb_cnt;
	hSymGrpCanvas = GetDlgItem(hDlg, IDC_SYM_GRP_FRAME);
	ReExposeCanvasAreaInWin(hDlg, hSymGrpCanvas);

}


void SaveSymbolGroup (sym_grp_name)
char	*sym_grp_name;
{
	HWND	hSymGrpList;
		
	if (sym_grp_edit_mode == EDIT_EXISTING_SYMBOL_GRP)
	{
		symgdb[info.sym_grp]	=symg_buff;
	}
	else if (sym_grp_edit_mode == ADD_NEW_SYMBOL_GRP)
	{
		if (symgdb_cnt < MAX_SYMBOL_GRPS)
		{
			strcpy (symg_buff.name, sym_grp_name);
			symgdb[info.sym_grp]	=symg_buff;
			symgdb_cnt++;

			sym_grp_edit_mode = EDIT_EXISTING_SYMBOL_GRP;
			hSymGrpList = GetDlgItem(ghWndSymGrpEdit, IDC_SYM_GRP_LIST);
			SendMessage(hSymGrpList, LB_ADDSTRING, 0, (LPARAM)sym_grp_name);
			SendMessage(hSymGrpList, LB_SETCURSEL, info.sym_grp, 0);
			EnableWindow(GetDlgItem(ghWndSymGrpEdit, IDC_SAVE_SYM_GRP), TRUE);
		}
		else
		{ 
			MessageBox (ghWndSymGrpEdit, "Symbol Group Buffer Over Flow", "IPM", MB_OK);
		}
	}
	else if (sym_grp_edit_mode == COPY_EXISTING_SYMBOL_GRP)
	{
		if (symgdb_cnt < MAX_SYMBOL_GRPS)
		{
			strcpy (symg_buff.name, sym_grp_name);
			info.sym_grp		=symgdb_cnt;
			symgdb[info.sym_grp]	=symg_buff;
			symgdb_cnt++;

			hSymGrpList = GetDlgItem(ghWndSymGrpEdit, IDC_SYM_GRP_LIST);
			SendMessage(hSymGrpList, LB_ADDSTRING, 0, (LPARAM)sym_grp_name);
			SendMessage(hSymGrpList, LB_SETCURSEL, info.sym_grp, 0);
			EnableWindow(GetDlgItem(ghWndSymGrpEdit, IDC_SAVE_SYM_GRP), TRUE);
		}
		else
		{ 
			MessageBox (ghWndSymGrpEdit, "Symbol Group Buffer Over Flow", "IPM", MB_OK);
		}
		sym_grp_edit_mode	=EDIT_EXISTING_SYMBOL_GRP;
	}

	return;
}

void DeleteSymbolGroup(HWND hDlg, int sym_grp_id)
{
	int	i;
	HWND hSymGrpList, hSymGrpCanvas;
		
	for (i=sym_grp_id; i < symgdb_cnt-1; i++)
	{
		symgdb[i]	=symgdb[i+1];
	}
	symgdb_cnt--;
	symgdb_cnt = max(symgdb_cnt, 0);  
	info.sym_grp = min(info.sym_grp, symgdb_cnt-1 );  
	symg_buff = symgdb[info.sym_grp];

	hSymGrpList = GetDlgItem(hDlg, IDC_SYM_GRP_LIST);
	SendMessage(hSymGrpList, LB_DELETESTRING, sym_grp_id, 0);
	SendMessage(hSymGrpList, LB_SETCURSEL, info.sym_grp, 0);
	hSymGrpCanvas = GetDlgItem(hDlg, IDC_SYM_GRP_FRAME);
	ReExposeCanvasAreaInWin(hDlg, hSymGrpCanvas);
}


